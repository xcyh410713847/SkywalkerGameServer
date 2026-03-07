/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Module\SFModule_NetworkClient.cpp
**作者: shyfan
**日期: 2023/08/26 15:37:47
**功能: 网络客户端模块
*************************************************************************/

#include "SFModule_NetworkClient.h"

#include "Include/SFCore.h"
#include "Include/SFILog.h"

#include "SkywalkerScript/Include/SkywalkerScriptParse.h"

#if defined(SKYWALKER_PLATFORM_WINDOWS)
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#endif

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_NetworkClient, Framework);

namespace
{
    SFUInt64 GetSteadyNowMS()
    {
        return static_cast<SFUInt64>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                         std::chrono::steady_clock::now().time_since_epoch())
                                         .count());
    }
}

#pragma region Object

void SFModule_NetworkClient::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);

    RegisterRouteHandlers();
}

void SFModule_NetworkClient::Awake(SFObjectErrors &Errors)
{
    SSFModule::Awake(Errors);
}

void SFModule_NetworkClient::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

    StartNetworkClient(Errors);
    if (Errors.IsValid())
    {
        return;
    }

    const char *ConfigPath = nullptr;
#if defined(_WIN32) || defined(_WIN64)
    char *ConfigPathBuffer = nullptr;
    size_t ConfigPathLen = 0;
    _dupenv_s(&ConfigPathBuffer, &ConfigPathLen, SF_ENV_PROGRAM_CONFIG_DIR);
    ConfigPath = ConfigPathBuffer;
#else
    ConfigPath = getenv(SF_ENV_PROGRAM_CONFIG_DIR);
#endif
    SFString ClientConfigPath = ConfigPath ? ConfigPath : "ClientConfig.skywalkerC";
#if defined(_WIN32) || defined(_WIN64)
    if (ConfigPathBuffer != nullptr)
    {
        free(ConfigPathBuffer);
        ConfigPathBuffer = nullptr;
    }
#endif

    SKYWALKER_PTR_SCRIPT_PARSE ConfigParse = new SKYWALKER_SCRIPT_NAMESPACE::CSkywalkerScriptParse();
    if (ConfigParse->LoadScript(ClientConfigPath.c_str()))
    {
        SKYWALKER_PTR_SCRIPT_NODE RootNode = ConfigParse->GetRootNode();
        if (RootNode != nullptr)
        {
            for (size_t i = 0; i < RootNode->GetChildNodeNum(); i++)
            {
                SKYWALKER_PTR_SCRIPT_NODE ConfigNode = RootNode->GetChildNodeFromIndex(i);
                if (ConfigNode == nullptr)
                {
                    continue;
                }

                SKYWALKER_PTR_SCRIPT_NODE IPNode = ConfigNode->GetChildNodeFromName("IP");
                if (IPNode != nullptr)
                {
                    ServerIP = IPNode->GetNodeValueString();
                }

                SKYWALKER_PTR_SCRIPT_NODE PortNode = ConfigNode->GetChildNodeFromName("Port");
                if (PortNode != nullptr)
                {
                    ServerPort = std::stoi(PortNode->GetNodeValueString());
                }

                SKYWALKER_PTR_SCRIPT_NODE ReconnectIntervalNode = ConfigNode->GetChildNodeFromName("ReconnectIntervalMS");
                if (ReconnectIntervalNode != nullptr)
                {
                    ReconnectIntervalMS = std::stoull(ReconnectIntervalNode->GetNodeValueString());
                }

                SKYWALKER_PTR_SCRIPT_NODE HeartbeatIntervalNode = ConfigNode->GetChildNodeFromName("HeartbeatIntervalMS");
                if (HeartbeatIntervalNode != nullptr)
                {
                    HeartbeatIntervalMS = std::stoull(HeartbeatIntervalNode->GetNodeValueString());
                }

                SKYWALKER_PTR_SCRIPT_NODE MaxReconnectCountNode = ConfigNode->GetChildNodeFromName("MaxReconnectCount");
                if (MaxReconnectCountNode != nullptr)
                {
                    MaxReconnectCount = static_cast<SFUInt32>(std::stoul(MaxReconnectCountNode->GetNodeValueString()));
                }

                SKYWALKER_PTR_SCRIPT_NODE PlayerIdNode = ConfigNode->GetChildNodeFromName("PlayerId");
                if (PlayerIdNode != nullptr)
                {
                    LoginPayload.PlayerId = std::stoull(PlayerIdNode->GetNodeValueString());
                }

                SKYWALKER_PTR_SCRIPT_NODE WorldIdNode = ConfigNode->GetChildNodeFromName("WorldId");
                if (WorldIdNode != nullptr)
                {
                    LoginPayload.WorldId = std::stoull(WorldIdNode->GetNodeValueString());
                }

                SKYWALKER_PTR_SCRIPT_NODE LoginTokenNode = ConfigNode->GetChildNodeFromName("LoginToken");
                if (LoginTokenNode != nullptr)
                {
                    LoginPayload.Token = LoginTokenNode->GetNodeValueString();
                }
            }
        }
    }

    SF_LOG_FRAMEWORK("Network Client Config IP " << ServerIP
                                                 << " Port " << ServerPort
                                                 << " ReconnectIntervalMS " << ReconnectIntervalMS
                                                 << " HeartbeatIntervalMS " << HeartbeatIntervalMS
                                                 << " MaxReconnectCount " << MaxReconnectCount
                                                 << " PlayerId " << LoginPayload.PlayerId
                                                 << " WorldId " << LoginPayload.WorldId);

    if (ServerIP.empty() || ServerPort <= 0)
    {
        SF_ERROR_DESC_TRACE(Errors,
                            ESFError::Network_Start_Failed,
                            "Network client config invalid");
        return;
    }

    if (!Connect(ServerIP.c_str(), ServerPort))
    {
        ++ReconnectAttemptCount;
        SF_LOG_FRAMEWORK("Connect server failed at startup, will retry " << ServerIP << ":" << ServerPort);
    }
}

void SFModule_NetworkClient::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);

    if (bIsConnected && ClientNetworkSocket != nullptr)
    {
        HandleReceive(Errors);
        TickHeartbeat();
        return;
    }

    if (ServerIP.empty() || ServerPort <= 0)
    {
        return;
    }

    if (MaxReconnectCount > 0 && ReconnectAttemptCount >= MaxReconnectCount)
    {
        return;
    }

    SFUInt64 NowMS = GetSteadyNowMS();
    if (LastReconnectAttemptMS != 0 && (NowMS - LastReconnectAttemptMS) < ReconnectIntervalMS)
    {
        return;
    }
    LastReconnectAttemptMS = NowMS;

    SF_LOG_FRAMEWORK("Try connect server " << ServerIP << ":" << ServerPort);

    if (Connect(ServerIP.c_str(), ServerPort))
    {
        SF_LOG_FRAMEWORK("Reconnect server success " << ServerIP << ":" << ServerPort);
    }
    else
    {
        ++ReconnectAttemptCount;
    }
}

void SFModule_NetworkClient::Stop(SFObjectErrors &Errors)
{
    StopNetworkClient(Errors);

    SSFModule::Stop(Errors);
}

void SFModule_NetworkClient::Sleep(SFObjectErrors &Errors)
{
    SSFModule::Sleep(Errors);
}

void SFModule_NetworkClient::Destroy(SFObjectErrors &Errors)
{
    SSFModule::Destroy(Errors);
}

#pragma endregion Object

bool SFModule_NetworkClient::Connect(const char *IP, int Port)
{
    SFObjectErrors SocketErrors;

    if (bIsConnected && ClientNetworkSocket != nullptr)
    {
        SF_LOG_ERROR("Already connected to server");
        return false;
    }

    ServerIP = IP;
    ServerPort = Port;

    SSFNetworkSocketCreatorContext Context;
    Context.Socket = SSF_SOCKET_CREATE(AF_INET, SOCK_STREAM);

    if (Context.Socket == SSF_INVALID_SOCKET)
    {
        SF_LOG_ERROR("Failed to create socket");
        return false;
    }

    struct sockaddr_in ServerAddr = {};
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(static_cast<u_short>(Port));
#if defined(SKYWALKER_PLATFORM_WINDOWS)
    if (inet_pton(AF_INET, IP, &ServerAddr.sin_addr) != 1)
    {
        SF_LOG_ERROR("Invalid server ip " << IP);
        SSF_CLOSE_SOCKET(Context.Socket);
        return false;
    }
#else
    if (inet_pton(AF_INET, IP, &ServerAddr.sin_addr) != 1)
    {
        SF_LOG_ERROR("Invalid server ip " << IP);
        SSF_CLOSE_SOCKET(Context.Socket);
        return false;
    }
#endif

    if (connect(Context.Socket, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) < 0)
    {
#if defined(SKYWALKER_PLATFORM_WINDOWS)
        int Error = WSAGetLastError();
        SF_LOG_ERROR("Failed to connect to server " << IP << ":" << Port << " Error " << Error);
#else
        SF_LOG_ERROR("Failed to connect to server " << IP << ":" << Port << " Error " << errno);
#endif
        SSF_CLOSE_SOCKET(Context.Socket);
        return false;
    }

    // 设置为非阻塞模式
#if defined(SKYWALKER_PLATFORM_WINDOWS)
    u_long mode = 1;
    if (ioctlsocket(Context.Socket, FIONBIO, &mode) == SSF_SOCKET_ERROR)
    {
        SSF_CLOSE_SOCKET(Context.Socket);
        SF_LOG_ERROR("Failed to set socket to non-blocking mode");
        return false;
    }
#else
    int flags = fcntl(Context.Socket, F_GETFL, 0);
    fcntl(Context.Socket, F_SETFL, flags | O_NONBLOCK);
#endif

    auto pClientSocket = NewObject<SSFObject_NetworkSocket>(Context, SocketErrors);
    ClientNetworkSocket = static_cast<SSF_PRT_NETWORK_SOCKET>(pClientSocket);

    if (SocketErrors.IsValid() || ClientNetworkSocket == nullptr)
    {
        SSF_CLOSE_SOCKET(Context.Socket);
        ClientNetworkSocket = nullptr;
        SF_LOG_ERROR("Failed to create client socket object");
        return false;
    }

    bIsConnected = true;
    ReconnectAttemptCount = 0;
    LastReconnectAttemptMS = 0;
    SF_LOG_FRAMEWORK("Connected to server " << IP << ":" << Port << " Socket " << ClientNetworkSocket->GetSocket());

    SendLoginPacket();

    return true;
}

void SFModule_NetworkClient::Disconnect()
{
    if (ClientNetworkSocket != nullptr && !ClientNetworkSocket->IsSocketInvalid())
    {
        SSF_CLOSE_SOCKET(ClientNetworkSocket->GetSocket());
        ClientNetworkSocket = nullptr;
    }

    bIsConnected = false;

    SF_LOG_FRAMEWORK("Disconnected from server");
}

bool SFModule_NetworkClient::IsConnected() const
{
    return bIsConnected && ClientNetworkSocket != nullptr && ClientNetworkSocket->IsSocketValid();
}

int SFModule_NetworkClient::Send(const char *Data, int Length)
{
    if (!IsConnected())
    {
        SF_LOG_ERROR("Not connected to server");
        return -1;
    }

    int SentBytes = SSF_SOCKET_WRITE(ClientNetworkSocket->GetSocket(), Data, Length, 0);
    if (SentBytes < 0)
    {
        SF_LOG_ERROR("Failed to send data");
        return -1;
    }

    return SentBytes;
}

bool SFModule_NetworkClient::SendPacket(const SSFNetworkPacket &Packet)
{
    std::vector<SFByte> Buffer;
    if (!SSFNetworkCodec::Encode(Packet, Buffer))
    {
        SF_LOG_ERROR("Encode network packet failed, MsgId " << Packet.MsgId);
        return false;
    }

    int SentBytes = Send(reinterpret_cast<const char *>(Buffer.data()), static_cast<int>(Buffer.size()));
    if (SentBytes != static_cast<int>(Buffer.size()))
    {
        SF_LOG_ERROR("Send network packet failed, MsgId " << Packet.MsgId << " SentBytes " << SentBytes << " PacketBytes " << Buffer.size());
        return false;
    }

    return true;
}

void SFModule_NetworkClient::StartNetworkClient(SFObjectErrors &Errors)
{
    if (Errors.IsValid())
    {
        return;
    }

    SFObjectErrors &InErrors = Errors;
    SSF_NETWORK_STARTUP();

    SF_LOG_FRAMEWORK("Network client started");
}

void SFModule_NetworkClient::StopNetworkClient(SFObjectErrors &Errors)
{
    Disconnect();

    SSF_NETWORK_CLEANUP();

    SF_LOG_FRAMEWORK("Network client stopped");
}

void SFModule_NetworkClient::HandleReceive(SFObjectErrors &Errors)
{
    if (ClientNetworkSocket == nullptr || ClientNetworkSocket->IsSocketInvalid())
    {
        return;
    }

    char Buffer[4096];
    int ReceivedBytes = SSF_SOCKET_READ(ClientNetworkSocket->GetSocket(), Buffer, sizeof(Buffer), 0);

    if (ReceivedBytes > 0)
    {
        SF_LOG_DEBUG("Received " << ReceivedBytes << " bytes from server");

        NetworkCodec.Append(reinterpret_cast<const SFByte *>(Buffer), static_cast<SFUInt32>(ReceivedBytes));

        SSFNetworkPacket Packet;
        while (NetworkCodec.TryDecode(Packet))
        {
            if (!NetworkRouter.Route(Packet))
            {
                SF_LOG_FRAMEWORK("Unhandled packet MsgId " << Packet.MsgId << " Seq " << Packet.Seq << " BodyLen " << Packet.BodyLen);
            }
        }
    }
    else if (ReceivedBytes == 0)
    {
        SF_LOG_DEBUG("Server closed connection");
        Disconnect();
    }
    else
    {
#if defined(SKYWALKER_PLATFORM_WINDOWS)
        int Error = WSAGetLastError();
        if (Error != WSAEWOULDBLOCK)
        {
            SF_LOG_ERROR("Socket error: " << Error);
            Disconnect();
        }
#else
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            SF_LOG_ERROR("Socket error: " << errno);
            Disconnect();
        }
#endif
    }
}

void SFModule_NetworkClient::RegisterRouteHandlers()
{
    NetworkRouter.RegisterHandler(static_cast<SFUInt16>(ESFNetworkMsg::S2C_HeartbeatAck),
                                  [this](const SSFNetworkPacket &Packet)
                                  {
                                      SF_LOG_FRAMEWORK("Recv HeartbeatAck Seq " << Packet.Seq);
                                  });

    NetworkRouter.RegisterHandler(static_cast<SFUInt16>(ESFNetworkMsg::S2C_LoginAck),
                                  [this](const SSFNetworkPacket &Packet)
                                  {
                                      SF_LOG_FRAMEWORK("Recv LoginAck Seq " << Packet.Seq);
                                  });
}

void SFModule_NetworkClient::TickHeartbeat()
{
    if (!IsConnected())
    {
        return;
    }

    const SFUInt64 NowMS = GetSteadyNowMS();
    if (LastHeartbeatSendMS != 0 && (NowMS - LastHeartbeatSendMS) < HeartbeatIntervalMS)
    {
        return;
    }

    LastHeartbeatSendMS = NowMS;

    SSFNetworkPacket Packet;
    Packet.MsgId = static_cast<SFUInt16>(ESFNetworkMsg::C2S_Heartbeat);
    Packet.Seq = ++SendSeq;

    if (SendPacket(Packet))
    {
        SF_LOG_FRAMEWORK("Send Heartbeat Seq " << Packet.Seq);
    }
}

void SFModule_NetworkClient::SendLoginPacket()
{
    if (!IsConnected())
    {
        return;
    }

    SSFNetworkPacket Packet;
    Packet.MsgId = static_cast<SFUInt16>(ESFNetworkMsg::C2S_Login);
    Packet.Seq = ++SendSeq;

    if (!SSFNetworkLoginPayloadCodec::Encode(LoginPayload, Packet.Body))
    {
        SF_LOG_ERROR("Encode login payload failed");
        return;
    }

    if (SendPacket(Packet))
    {
        SF_LOG_FRAMEWORK("Send Login Seq " << Packet.Seq);
    }
}
