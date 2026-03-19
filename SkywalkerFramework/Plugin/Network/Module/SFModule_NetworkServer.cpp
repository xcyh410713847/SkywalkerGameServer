/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Module\SFModule_NetworkServer.cpp
**作者: shyfan
**日期: 2023/08/26 15:37:24
**功能: 网络服务器模块
*************************************************************************/

#include "SFModule_NetworkServer.h"

#include "Include/SFCore.h"
#include "Include/SFILog.h"

#include "SkywalkerTools/SkywalkerScript/SkywalkerScript.h"

#include <chrono>
#include <vector>

#if defined(SKYWALKER_PLATFORM_WINDOWS)
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_NetworkServer, Framework);

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

void SFModule_NetworkServer::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);

    /* 注册心跳处理器 */
    Dispatcher.RegisterHandler(SF_MSGID_HEARTBEAT,
        [this](SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen)
        {
            /* 收到心跳 → 回复心跳 */
            SendTo(SessionId, SF_MSGID_HEARTBEAT, nullptr, 0);
        });
}

void SFModule_NetworkServer::Awake(SFObjectErrors &Errors)
{
    SSFModule::Awake(Errors);
}

void SFModule_NetworkServer::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

    /* 从 ServerConfig.skywalkerC 读取网络配置 */
    const char *ConfigPath = nullptr;
#if defined(_WIN32) || defined(_WIN64)
    char *ConfigPathBuffer = nullptr;
    size_t ConfigPathLen = 0;
    _dupenv_s(&ConfigPathBuffer, &ConfigPathLen, SF_ENV_PROGRAM_CONFIG_DIR);
    ConfigPath = ConfigPathBuffer;
#else
    ConfigPath = getenv(SF_ENV_PROGRAM_CONFIG_DIR);
#endif
    SFString ServerConfigPath = ConfigPath ? ConfigPath : "ServerConfig.skywalkerC";
#if defined(_WIN32) || defined(_WIN64)
    if (ConfigPathBuffer != nullptr)
    {
        free(ConfigPathBuffer);
        ConfigPathBuffer = nullptr;
    }
#endif

    SKYWALKER_PTR_SCRIPT_PARSE ConfigParse = new SKYWALKER_SCRIPT_NAMESPACE::CSkywalkerScriptParse();
    if (ConfigParse->LoadScript(ServerConfigPath.c_str()))
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

                SKYWALKER_PTR_SCRIPT_NODE TimeoutNode = ConfigNode->GetChildNodeFromName("SessionTimeoutMS");
                if (TimeoutNode != nullptr)
                {
                    SessionTimeoutMS = std::stoull(TimeoutNode->GetNodeValueString());
                }
            }
        }
    }
    delete ConfigParse;

    SF_LOG_FRAMEWORK("Network Server Config IP " << ServerIP << " Port " << ServerPort
                     << " SessionTimeoutMS " << SessionTimeoutMS);

    if (ServerPort <= 0)
    {
        SF_ERROR_DESC_TRACE(Errors,
                            ESFError::Network_Start_Failed,
                            "Network server config invalid: Port not configured");
        return;
    }

    StartNetworkServer(Errors);
}

void SFModule_NetworkServer::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);

    AcceptNewConnections(Errors);
    ProcessSessionRecv(Errors);
    ProcessSessionSend(Errors);
    CleanupClosedSessions(Errors);
    CheckSessionTimeout();
}

void SFModule_NetworkServer::Stop(SFObjectErrors &Errors)
{
    StopNetworkServer(Errors);

    SSFModule::Stop(Errors);
}

void SFModule_NetworkServer::Sleep(SFObjectErrors &Errors)
{
    SSFModule::Sleep(Errors);
}

void SFModule_NetworkServer::Destroy(SFObjectErrors &Errors)
{
    SSFModule::Destroy(Errors);
}

#pragma endregion Object

#pragma region Network Server

void SFModule_NetworkServer::StartNetworkServer(SFObjectErrors &InErrors)
{
    if (InErrors.IsValid())
    {
        return;
    }

    SSF_NETWORK_STARTUP();

    SSFNetworkSocketCreatorContext Context;
    Context.IP = ServerIP;
    Context.Port = ServerPort;
    auto pServerSocket = NewObject<SSFObject_ServerSocket>(Context, InErrors);
    ServerNetworkSocket = SF_UNIQUE_PTR_CAST(SSFObject_ServerSocket, pServerSocket);

    if (InErrors.IsValid())
    {
        SF_ERROR_DESC_TRACE(InErrors,
                            ESFError::Network_Start_Failed,
                            "Failed to start network server");
        SSF_NETWORK_CLEANUP();

        return;
    }

    SF_LOG_FRAMEWORK("Create ServerSocket ObjectGUID " << ServerNetworkSocket->GetObjectGUID()
                     << " Socket " << ServerNetworkSocket->GetSocket());
}

void SFModule_NetworkServer::StopNetworkServer(SFObjectErrors &Errors)
{
    /* 清理所有 Session */
    std::vector<SFUInt32> AllSessionIds;
    SF_COMMON_ITERATOR(IterSession, SessionMap)
    {
        AllSessionIds.push_back(IterSession->first);
    }
    for (SFUInt32 SessionId : AllSessionIds)
    {
        DestroySession(SessionId, Errors);
    }

    if (ServerNetworkSocket != nullptr)
    {
        ServerNetworkSocket->Stop(Errors);
        ServerNetworkSocket = nullptr;
    }

    SSF_NETWORK_CLEANUP();
}

void SFModule_NetworkServer::AcceptNewConnections(SFObjectErrors &Errors)
{
    if (ServerNetworkSocket == nullptr || ServerNetworkSocket->IsSocketInvalid())
    {
        return;
    }

    sockaddr_in ClientAddr = {};
#if defined(SKYWALKER_PLATFORM_WINDOWS)
    int ClientAddrLen = sizeof(ClientAddr);
#else
    socklen_t ClientAddrLen = sizeof(ClientAddr);
#endif

    SSFSOCKET ClientSocket = accept(ServerNetworkSocket->GetSocket(),
                                     reinterpret_cast<sockaddr *>(&ClientAddr),
                                     &ClientAddrLen);
    if (ClientSocket == SSF_INVALID_SOCKET)
    {
        return;
    }

    /* 设置为非阻塞 */
#if defined(SKYWALKER_PLATFORM_WINDOWS)
    u_long mode = 1;
    ioctlsocket(ClientSocket, FIONBIO, &mode);
#else
    int flags = fcntl(ClientSocket, F_GETFL, 0);
    fcntl(ClientSocket, F_SETFL, flags | O_NONBLOCK);
#endif

    SSFNetworkSocketCreatorContext Context;
    Context.Socket = ClientSocket;
    SSF_PRT_CLIENT_SOCKET ClientSocketObj = NewObject<SSFObject_ClientSocket>(Context, Errors);
    if (Errors.IsValid() || ClientSocketObj == nullptr)
    {
        SSF_CLOSE_SOCKET(ClientSocket);
        return;
    }

    /* 创建 Session */
    SFUInt32 SessionId = NextSessionId++;
    SFSession *Session = new SFSession(SessionId, ClientSocketObj);
    SessionMap.insert(std::make_pair(SessionId, Session));
    SocketToSessionMap.insert(std::make_pair(ClientSocket, SessionId));

    SF_LOG_FRAMEWORK("New Session " << SessionId << " Socket " << ClientSocket);
}

void SFModule_NetworkServer::ProcessSessionRecv(SFObjectErrors &Errors)
{
    SF_COMMON_ITERATOR(IterSession, SessionMap)
    {
        SFSession *Session = IterSession->second;
        if (Session == nullptr || Session->IsClosing())
        {
            continue;
        }

        SSF_PRT_CLIENT_SOCKET ClientSocket = Session->GetClientSocket();
        if (ClientSocket == nullptr)
        {
            continue;
        }

        SSFSOCKET Sock = ClientSocket->GetSocket();
        if (Sock == SSF_INVALID_SOCKET)
        {
            continue;
        }

        /* 从 Socket 读数据到 RecvBuffer */
        SFNetworkBuffer &RecvBuffer = Session->GetRecvBuffer();
        /* 确保有足够可写空间 */
        if (RecvBuffer.WritableBytes() < 4096)
        {
            RecvBuffer.Compact();
        }

        char TempBuf[4096];
        int BytesRead = SSF_SOCKET_READ(Sock, TempBuf, sizeof(TempBuf), 0);
        if (BytesRead > 0)
        {
            RecvBuffer.Append(TempBuf, static_cast<SFSize>(BytesRead));
            Session->TouchActive();

            /* 解码消息 */
            std::vector<SFDecodedMessage> Messages;
            SFUInt32 MsgCount = SFMessageCodec::Decode(RecvBuffer, Messages);

            for (SFUInt32 i = 0; i < MsgCount; i++)
            {
                const SFDecodedMessage &Msg = Messages[i];
                if (!Dispatcher.Dispatch(Session->GetSessionId(), Msg.MsgID,
                                         Msg.Payload, Msg.PayloadLen))
                {
                    SF_LOG_FRAMEWORK("Unhandled MsgID 0x"
                                     << std::hex << Msg.MsgID << std::dec
                                     << " from Session " << Session->GetSessionId());
                }
            }
        }
        else if (BytesRead == 0)
        {
            /* 对端关闭 */
            Session->MarkClosing();
        }
        else
        {
            /* BytesRead < 0 */
#if defined(SKYWALKER_PLATFORM_WINDOWS)
            int Error = WSAGetLastError();
            if (Error != WSAEWOULDBLOCK)
            {
                Session->MarkClosing();
            }
#else
            if (errno != EAGAIN && errno != EWOULDBLOCK)
            {
                Session->MarkClosing();
            }
#endif
        }
    }
}

void SFModule_NetworkServer::ProcessSessionSend(SFObjectErrors &Errors)
{
    SF_COMMON_ITERATOR(IterSession, SessionMap)
    {
        SFSession *Session = IterSession->second;
        if (Session == nullptr)
        {
            continue;
        }

        SFNetworkBuffer &SendBuffer = Session->GetSendBuffer();
        if (SendBuffer.ReadableBytes() == 0)
        {
            continue;
        }

        SSFSOCKET Sock = Session->GetSocket();
        if (Sock == SSF_INVALID_SOCKET)
        {
            continue;
        }

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
                Session->MarkClosing();
            }
#else
            if (errno != EAGAIN && errno != EWOULDBLOCK)
            {
                Session->MarkClosing();
            }
#endif
        }
    }
}

void SFModule_NetworkServer::CleanupClosedSessions(SFObjectErrors &Errors)
{
    std::vector<SFUInt32> ClosedSessionIds;

    SF_COMMON_ITERATOR(IterSession, SessionMap)
    {
        SFSession *Session = IterSession->second;
        if (Session == nullptr || Session->IsClosed() || Session->IsClosing())
        {
            ClosedSessionIds.push_back(IterSession->first);
        }
    }

    for (SFUInt32 SessionId : ClosedSessionIds)
    {
        DestroySession(SessionId, Errors);
    }
}

void SFModule_NetworkServer::CheckSessionTimeout()
{
    if (SessionTimeoutMS == 0)
    {
        return;
    }

    std::vector<SFUInt32> TimeoutSessionIds;
    SF_COMMON_ITERATOR(IterSession, SessionMap)
    {
        SFSession *Session = IterSession->second;
        if (Session != nullptr && Session->IsTimeout(SessionTimeoutMS))
        {
            SF_LOG_FRAMEWORK("Session " << IterSession->first << " timed out");
            TimeoutSessionIds.push_back(IterSession->first);
        }
    }

    SFObjectErrors Errors;
    for (SFUInt32 SessionId : TimeoutSessionIds)
    {
        DestroySession(SessionId, Errors);
    }
}

void SFModule_NetworkServer::DestroySession(SFUInt32 SessionId, SFObjectErrors &Errors)
{
    auto IterSession = SessionMap.find(SessionId);
    if (IterSession == SessionMap.end())
    {
        return;
    }

    SFSession *Session = IterSession->second;
    if (Session != nullptr)
    {
        SSFSOCKET Sock = Session->GetSocket();
        if (Sock != SSF_INVALID_SOCKET)
        {
            SSF_CLOSE_SOCKET(Sock);
            SocketToSessionMap.erase(Sock);
        }

        SF_LOG_FRAMEWORK("Destroy Session " << SessionId
                         << " PlayerId " << Session->GetPlayerId());

        delete Session;
    }

    SessionMap.erase(IterSession);
}

#pragma endregion Network Server

#pragma region Public API

bool SFModule_NetworkServer::SendTo(SFUInt32 SessionId, SFMsgID MsgID,
                                     const char *Payload, SFUInt32 PayloadLen)
{
    auto IterSession = SessionMap.find(SessionId);
    if (IterSession == SessionMap.end() || IterSession->second == nullptr)
    {
        return false;
    }

    SFSession *Session = IterSession->second;
    return SFMessageCodec::Encode(Session->GetSendBuffer(), MsgID, Payload, PayloadLen);
}

void SFModule_NetworkServer::Broadcast(SFMsgID MsgID,
                                        const char *Payload, SFUInt32 PayloadLen)
{
    SF_COMMON_ITERATOR(IterSession, SessionMap)
    {
        SFSession *Session = IterSession->second;
        if (Session != nullptr && Session->IsAuthenticated() && !Session->IsClosing())
        {
            SFMessageCodec::Encode(Session->GetSendBuffer(), MsgID, Payload, PayloadLen);
        }
    }
}

void SFModule_NetworkServer::BroadcastTo(const std::vector<SFUInt32> &SessionIds,
                                          SFMsgID MsgID,
                                          const char *Payload, SFUInt32 PayloadLen)
{
    for (SFUInt32 SessionId : SessionIds)
    {
        SendTo(SessionId, MsgID, Payload, PayloadLen);
    }
}

SFSession *SFModule_NetworkServer::GetSession(SFUInt32 SessionId)
{
    auto IterSession = SessionMap.find(SessionId);
    if (IterSession == SessionMap.end())
    {
        return nullptr;
    }
    return IterSession->second;
}

SFUInt32 SFModule_NetworkServer::FindSessionByPlayerId(SFUInt32 PlayerId)
{
    SF_COMMON_ITERATOR(IterSession, SessionMap)
    {
        SFSession *Session = IterSession->second;
        if (Session != nullptr && Session->GetPlayerId() == PlayerId)
        {
            return IterSession->first;
        }
    }
    return 0;
}

void SFModule_NetworkServer::CloseSession(SFUInt32 SessionId)
{
    auto IterSession = SessionMap.find(SessionId);
    if (IterSession != SessionMap.end() && IterSession->second != nullptr)
    {
        IterSession->second->MarkClosing();
    }
}

#pragma endregion Public API
