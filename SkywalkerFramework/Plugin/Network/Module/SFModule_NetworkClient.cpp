/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Module\SFModule_NetworkClient.cpp
**作者: shyfan
**日期: 2023/08/26 15:37:47
**功能: 网络客户端模块
*************************************************************************/

#include "SFModule_NetworkClient.h"

#include "Include/SFCore.h"
#include "Include/SFILog.h"

#include "SkywalkerTools/SkywalkerScript/SkywalkerScript.h"

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

/** 心跳消息ID */
static const SFMsgID SF_MSGID_HEARTBEAT = 0x0001;

#pragma region Object

void SFModule_NetworkClient::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);

    /* 注册心跳回复处理（收到服务端心跳回复 → 日志） */
    Dispatcher.RegisterHandler(SF_MSGID_HEARTBEAT,
        [](SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen)
        {
            /* 心跳回复，无需特殊处理 */
        });
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
            }
        }
    }
    delete ConfigParse;

    SF_LOG_FRAMEWORK("Network Client Config IP " << ServerIP << " Port " << ServerPort);

    if (ServerIP.empty() || ServerPort <= 0)
    {
        SF_ERROR_DESC_TRACE(Errors,
                            ESFError::Network_Start_Failed,
                            "Network client config invalid");
        return;
    }

    if (!Connect(ServerIP.c_str(), ServerPort))
    {
        SF_LOG_FRAMEWORK("Connect server failed at startup, will retry " << ServerIP << ":" << ServerPort);
    }

    LastHeartbeatMS = GetSteadyNowMS();
}

void SFModule_NetworkClient::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);

    if (bIsConnected && ClientNetworkSocket != nullptr)
    {
        ProcessRecv();
        FlushSend();
        SendHeartbeat();
        return;
    }

    if (ServerIP.empty() || ServerPort <= 0)
    {
        return;
    }

    SF_LOG_FRAMEWORK("Try connect server " << ServerIP << ":" << ServerPort);

    if (Connect(ServerIP.c_str(), ServerPort))
    {
        SF_LOG_FRAMEWORK("Reconnect server success " << ServerIP << ":" << ServerPort);
        LastHeartbeatMS = GetSteadyNowMS();
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

    /* 设置为非阻塞模式 */
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
    RecvBuffer.Clear();
    SendBuffer.Clear();
    SF_LOG_FRAMEWORK("Connected to server " << IP << ":" << Port << " Socket " << ClientNetworkSocket->GetSocket());

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
    RecvBuffer.Clear();
    SendBuffer.Clear();

    SF_LOG_FRAMEWORK("Disconnected from server");
}

bool SFModule_NetworkClient::IsConnected() const
{
    return bIsConnected && ClientNetworkSocket != nullptr && ClientNetworkSocket->IsSocketValid();
}

bool SFModule_NetworkClient::SendMsg(SFMsgID MsgID, const char *Payload, SFUInt32 PayloadLen)
{
    if (!IsConnected())
    {
        return false;
    }
    return SFMessageCodec::Encode(SendBuffer, MsgID, Payload, PayloadLen);
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

void SFModule_NetworkClient::StartNetworkClient(SFObjectErrors &InErrors)
{
    if (InErrors.IsValid())
    {
        return;
    }

    SSF_NETWORK_STARTUP();

    SF_LOG_FRAMEWORK("Network client started");
}

void SFModule_NetworkClient::StopNetworkClient(SFObjectErrors &InErrors)
{
    Disconnect();

    SSF_NETWORK_CLEANUP();

    SF_LOG_FRAMEWORK("Network client stopped");
}

void SFModule_NetworkClient::ProcessRecv()
{
    if (ClientNetworkSocket == nullptr || ClientNetworkSocket->IsSocketInvalid())
    {
        return;
    }

    SSFSOCKET Sock = ClientNetworkSocket->GetSocket();
    char TempBuf[4096];
    int BytesRead = SSF_SOCKET_READ(Sock, TempBuf, sizeof(TempBuf), 0);

    if (BytesRead > 0)
    {
        RecvBuffer.Append(TempBuf, static_cast<SFSize>(BytesRead));

        std::vector<SFDecodedMessage> Messages;
        SFUInt32 MsgCount = SFMessageCodec::Decode(RecvBuffer, Messages);

        for (SFUInt32 i = 0; i < MsgCount; i++)
        {
            const SFDecodedMessage &Msg = Messages[i];
            if (!Dispatcher.Dispatch(0, Msg.MsgID, Msg.Payload, Msg.PayloadLen))
            {
                SF_LOG_FRAMEWORK("Client unhandled MsgID 0x"
                                 << std::hex << Msg.MsgID << std::dec);
            }
        }
    }
    else if (BytesRead == 0)
    {
        /* 服务器断开 */
        SF_LOG_FRAMEWORK("Server disconnected");
        Disconnect();
    }
    else
    {
#if defined(SKYWALKER_PLATFORM_WINDOWS)
        int Error = WSAGetLastError();
        if (Error != WSAEWOULDBLOCK)
        {
            SF_LOG_ERROR("Recv error " << Error);
            Disconnect();
        }
#else
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            SF_LOG_ERROR("Recv error " << errno);
            Disconnect();
        }
#endif
    }
}

void SFModule_NetworkClient::FlushSend()
{
    if (SendBuffer.ReadableBytes() == 0)
    {
        return;
    }

    if (ClientNetworkSocket == nullptr || ClientNetworkSocket->IsSocketInvalid())
    {
        return;
    }

    SSFSOCKET Sock = ClientNetworkSocket->GetSocket();
    int BytesSent = SSF_SOCKET_WRITE(Sock,
                                      SendBuffer.ReadPtr(),
                                      static_cast<int>(SendBuffer.ReadableBytes()),
                                      0);
    if (BytesSent > 0)
    {
        SendBuffer.Consume(static_cast<SFSize>(BytesSent));
    }
    else if (BytesSent < 0)
    {
#if defined(SKYWALKER_PLATFORM_WINDOWS)
        int Error = WSAGetLastError();
        if (Error != WSAEWOULDBLOCK)
        {
            SF_LOG_ERROR("Send error " << Error);
            Disconnect();
        }
#else
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            SF_LOG_ERROR("Send error " << errno);
            Disconnect();
        }
#endif
    }
}

void SFModule_NetworkClient::SendHeartbeat()
{
    SFUInt64 NowMS = GetSteadyNowMS();
    if ((NowMS - LastHeartbeatMS) >= HeartbeatIntervalMS)
    {
        SendMsg(SF_MSGID_HEARTBEAT, nullptr, 0);
        LastHeartbeatMS = NowMS;
    }
}
