/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Module\SFModule_NetworkServer.cpp
**作者: shyfan
**日期: 2023/08/26 15:37:24
**功能: 网络服务器模块
*************************************************************************/

#include "SFModule_NetworkServer.h"

#include "Include/SFCore.h"
#include "Include/SFILog.h"

#include "../Session/SFNetworkBlacklistStore.h"
#include "../Protocol/SFNetworkLoginPayload.h"

#include "SkywalkerTools/SkywalkerScript/SkywalkerScript.h"

#include <chrono>
#include <vector>
#include <filesystem>

#if defined(SKYWALKER_PLATFORM_WINDOWS)
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_NetworkServer, Framework);

namespace
{
    SFUInt64 GetSteadyNowMS_Server()
    {
        return static_cast<SFUInt64>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                         std::chrono::steady_clock::now().time_since_epoch())
                                         .count());
    }
}

#pragma region Object

void SFModule_NetworkServer::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);

    RegisterRouteHandlers();
}

void SFModule_NetworkServer::Awake(SFObjectErrors &Errors)
{
    SSFModule::Awake(Errors);
}

void SFModule_NetworkServer::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

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
                    continue;

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

                SKYWALKER_PTR_SCRIPT_NODE SessionTimeoutNode = ConfigNode->GetChildNodeFromName("SessionTimeoutMS");
                if (SessionTimeoutNode != nullptr)
                {
                    SessionTimeoutMS = std::stoull(SessionTimeoutNode->GetNodeValueString());
                }

                SKYWALKER_PTR_SCRIPT_NODE MaxMsgPerSecondNode = ConfigNode->GetChildNodeFromName("MaxMsgPerSecond");
                if (MaxMsgPerSecondNode != nullptr)
                {
                    MaxMsgPerSecond = static_cast<SFUInt32>(std::stoul(MaxMsgPerSecondNode->GetNodeValueString()));
                }

                SKYWALKER_PTR_SCRIPT_NODE StatsLogIntervalNode = ConfigNode->GetChildNodeFromName("StatsLogIntervalMS");
                if (StatsLogIntervalNode != nullptr)
                {
                    StatsLogIntervalMS = std::stoull(StatsLogIntervalNode->GetNodeValueString());
                }

                SKYWALKER_PTR_SCRIPT_NODE ViolationThresholdNode = ConfigNode->GetChildNodeFromName("ViolationThreshold");
                if (ViolationThresholdNode != nullptr)
                {
                    ViolationThreshold = std::stoull(ViolationThresholdNode->GetNodeValueString());
                }

                SKYWALKER_PTR_SCRIPT_NODE BlacklistDurationNode = ConfigNode->GetChildNodeFromName("BlacklistDurationMS");
                if (BlacklistDurationNode != nullptr)
                {
                    BlacklistDurationMS = std::stoull(BlacklistDurationNode->GetNodeValueString());
                }

                SKYWALKER_PTR_SCRIPT_NODE BlacklistFilePathNode = ConfigNode->GetChildNodeFromName("BlacklistFilePath");
                if (BlacklistFilePathNode != nullptr)
                {
                    BlacklistFilePath = BlacklistFilePathNode->GetNodeValueString();
                }

                SKYWALKER_PTR_SCRIPT_NODE BlacklistPersistenceNode = ConfigNode->GetChildNodeFromName("BlacklistPersistence");
                if (BlacklistPersistenceNode != nullptr)
                {
                    const SFString Value = BlacklistPersistenceNode->GetNodeValueString();
                    BlacklistPersistenceEnabled = (Value == "1" || Value == "true" || Value == "TRUE");
                }

                SKYWALKER_PTR_SCRIPT_NODE BlacklistHotReloadNode = ConfigNode->GetChildNodeFromName("BlacklistHotReload");
                if (BlacklistHotReloadNode != nullptr)
                {
                    const SFString Value = BlacklistHotReloadNode->GetNodeValueString();
                    BlacklistHotReloadEnabled = (Value == "1" || Value == "true" || Value == "TRUE");
                }

                SKYWALKER_PTR_SCRIPT_NODE BlacklistReloadIntervalNode = ConfigNode->GetChildNodeFromName("BlacklistReloadIntervalMS");
                if (BlacklistReloadIntervalNode != nullptr)
                {
                    BlacklistReloadIntervalMS = std::stoull(BlacklistReloadIntervalNode->GetNodeValueString());
                }
            }
        }
    }

    if (!ServerConfigPath.empty())
    {
        std::filesystem::path ServerConfigObj(ServerConfigPath);
        std::filesystem::path BlacklistPathObj(BlacklistFilePath);
        if (!BlacklistPathObj.is_absolute())
        {
            BlacklistFilePath = (ServerConfigObj.parent_path() / BlacklistPathObj).lexically_normal().string();
        }
    }

    LoadBlacklistFromFile(GetSteadyNowMS_Server());

    SF_LOG_FRAMEWORK("Network Config IP " << ServerIP
                                          << " Port " << ServerPort
                                          << " SessionTimeoutMS " << SessionTimeoutMS
                                          << " MaxMsgPerSecond " << MaxMsgPerSecond
                                          << " StatsLogIntervalMS " << StatsLogIntervalMS
                                          << " ViolationThreshold " << ViolationThreshold
                                          << " BlacklistDurationMS " << BlacklistDurationMS
                                          << " BlacklistFilePath " << BlacklistFilePath
                                          << " BlacklistPersistence " << BlacklistPersistenceEnabled
                                          << " BlacklistHotReload " << BlacklistHotReloadEnabled
                                          << " BlacklistReloadIntervalMS " << BlacklistReloadIntervalMS);

    StartNetworkServer(Errors);
}

void SFModule_NetworkServer::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);

    CreateNetworkClient(Errors);

    std::vector<SSFSOCKET> ClosedSockets;
    SF_COMMON_ITERATOR(Iter, ClientNetworkSocketMap)
    {
        Iter->second->Tick(Errors, DelayMS);
        if (Iter->second->IsClosed())
        {
            ClosedSockets.push_back(Iter->first);
        }
    }

    for (SSFSOCKET Socket : ClosedSockets)
    {
        CleanupClientSocket(Socket, Errors);
    }

    DispatchClientPackets(Errors);

    TickSessions(Errors);
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

void SFModule_NetworkServer::StartNetworkServer(SFObjectErrors &InErrors)
{
    if (InErrors.IsValid())
    {
        return;
    }

    SSF_NETWORK_STARTUP();

    // 创建服务器套接字
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

    SF_LOG_FRAMEWORK("Create ServerSocket ObjectGUID " << ServerNetworkSocket->GetObjectGUID() << " Socket " << ServerNetworkSocket->GetSocket());
}

void SFModule_NetworkServer::StopNetworkServer(SFObjectErrors &Errors)
{
    std::vector<SSFSOCKET> AllSockets;
    SF_COMMON_ITERATOR(IterClient, ClientNetworkSocketMap)
    {
        AllSockets.push_back(IterClient->first);
    }

    for (SSFSOCKET Socket : AllSockets)
    {
        CleanupClientSocket(Socket, Errors);
    }

    ServerNetworkSocket->Stop(Errors);
    ServerNetworkSocket = nullptr;

    SaveBlacklistToFile();

    SSF_NETWORK_CLEANUP();
}

void SFModule_NetworkServer::CreateNetworkClient(SFObjectErrors &Errors)
{
    if (ServerNetworkSocket->IsSocketInvalid())
    {
        SF_ERROR_DESC_TRACE(
            Errors,
            ESFError::Network_Socket_CreateFailed,
            "Create ClientSocket Failed, Because ServerSocket is Invalid")

        return;
    }

    sockaddr_in ClientAddr = {};
#if defined(SKYWALKER_PLATFORM_WINDOWS)
    int ClientAddrLen = sizeof(ClientAddr);
#else
    socklen_t ClientAddrLen = sizeof(ClientAddr);
#endif

    SSFSOCKET ClientSocket = accept(ServerNetworkSocket->GetSocket(), reinterpret_cast<sockaddr *>(&ClientAddr), &ClientAddrLen);
    if (ClientSocket == SSF_INVALID_SOCKET)
    {
        return;
    }

    char IpBuffer[64] = {};
    const char *IpPtr = inet_ntop(AF_INET, &ClientAddr.sin_addr, IpBuffer, sizeof(IpBuffer));
    SFString ClientIP = (IpPtr == nullptr) ? "Unknown" : SFString(IpPtr);

    SFUInt64 NowMS = GetSteadyNowMS_Server();
    if (IsBlacklisted(ClientIP, NowMS))
    {
        SF_LOG_FRAMEWORK("Reject blacklisted client, IP " << ClientIP);
        SSF_CLOSE_SOCKET(ClientSocket);
        return;
    }

    // 创建客户端套接字
    SSFNetworkSocketCreatorContext Context;
    Context.Socket = ClientSocket;
    SSF_PRT_CLIENT_SOCKET ClientNetworkSocket = NewObject<SSFObject_ClientSocket>(Context, Errors);
    if (Errors.IsValid() || ClientNetworkSocket == nullptr)
    {
        SSF_CLOSE_SOCKET(ClientSocket);
        return;
    }

    ClientNetworkSocketMap.insert(std::make_pair(ClientSocket, ClientNetworkSocket));

    SSFNetworkSession NewSession;
    NewSession.SessionId = NextSessionId++;
    NewSession.Socket = ClientSocket;
    NewSession.ClientIP = ClientIP;
    NewSession.LastRecvMS = ClientNetworkSocket->GetLastRecvMS();
    NewSession.LastHeartbeatMS = ClientNetworkSocket->GetLastHeartbeatMS();
    SessionManager.AddSession(NewSession);

    SF_LOG_FRAMEWORK("New ClientSocket: " << ClientSocket << " IP " << ClientIP);
}

void SFModule_NetworkServer::TickSessions(SFObjectErrors &Errors)
{
    SFUInt64 NowMS = GetSteadyNowMS_Server();

    ReloadBlacklistIfNeeded(NowMS);

    std::vector<SSFSOCKET> TimeoutSockets;
    auto &AllSessions = SessionManager.GetAllSessions();
    SF_COMMON_ITERATOR(IterSession, AllSessions)
    {
        SSFSOCKET Socket = IterSession->first;
        SSFNetworkSession &Session = IterSession->second;

        auto IterClient = ClientNetworkSocketMap.find(Socket);
        if (IterClient == ClientNetworkSocketMap.end() || IterClient->second == nullptr)
        {
            TimeoutSockets.push_back(Socket);
            continue;
        }

        Session.LastRecvMS = IterClient->second->GetLastRecvMS();
        Session.LastHeartbeatMS = IterClient->second->GetLastHeartbeatMS();

        if ((Session.InvalidPacketCount + Session.DroppedMsgCount) >= ViolationThreshold)
        {
            ++Session.ViolationCount;
            AddToBlacklist(Session.ClientIP, NowMS, "invalid or rate limit exceeded");
            TimeoutSockets.push_back(Socket);
            continue;
        }

        if (Session.LastRecvMS != 0 && (NowMS - Session.LastRecvMS) > SessionTimeoutMS)
        {
            TimeoutSockets.push_back(Socket);
        }
    }

    for (SSFSOCKET Socket : TimeoutSockets)
    {
        SF_LOG_FRAMEWORK("Client session timeout, Socket " << Socket);
        CleanupClientSocket(Socket, Errors);
    }

    if (LastStatsLogMS == 0)
    {
        LastStatsLogMS = NowMS;
    }
    else if ((NowMS - LastStatsLogMS) >= StatsLogIntervalMS)
    {
        LastStatsLogMS = NowMS;

        auto &Sessions = SessionManager.GetAllSessions();
        SF_COMMON_ITERATOR(IterSessionStats, Sessions)
        {
            SSFNetworkSession &Session = IterSessionStats->second;
            if (Session.DroppedMsgCount > 0 || Session.InvalidPacketCount > 0)
            {
                SF_LOG_FRAMEWORK("Session Stats Socket " << Session.Socket
                                                         << " IP " << Session.ClientIP
                                                         << " DroppedMsg " << Session.DroppedMsgCount
                                                         << " InvalidPacket " << Session.InvalidPacketCount
                                                         << " Violations " << Session.ViolationCount);
            }
        }

        SaveBlacklistToFile();
    }
}

void SFModule_NetworkServer::CleanupClientSocket(SSFSOCKET Socket, SFObjectErrors &Errors)
{
    auto IterClient = ClientNetworkSocketMap.find(Socket);
    if (IterClient != ClientNetworkSocketMap.end())
    {
        if (IterClient->second != nullptr)
        {
            SSF_CLOSE_SOCKET(Socket);
            IterClient->second->Release(Errors);
        }
        ClientNetworkSocketMap.erase(IterClient);
    }

    SessionManager.RemoveSession(Socket);
}

void SFModule_NetworkServer::RegisterRouteHandlers()
{
    RouteHandlerMap[static_cast<SFUInt16>(ESFNetworkMsg::C2S_Heartbeat)] =
        [this](SSFSOCKET Socket, const SSFNetworkPacket &Packet)
    {
        HandleHeartbeat(Socket, Packet);
    };

    RouteHandlerMap[static_cast<SFUInt16>(ESFNetworkMsg::C2S_Login)] =
        [this](SSFSOCKET Socket, const SSFNetworkPacket &Packet)
    {
        HandleLogin(Socket, Packet);
    };

    RouteHandlerMap[static_cast<SFUInt16>(ESFNetworkMsg::C2S_PlayerInput)] =
        [this](SSFSOCKET Socket, const SSFNetworkPacket &Packet)
    {
        HandlePlayerInput(Socket, Packet);
    };
}

void SFModule_NetworkServer::DispatchClientPackets(SFObjectErrors &Errors)
{
    const SFUInt64 NowMS = GetSteadyNowMS_Server();

    SF_COMMON_ITERATOR(IterClient, ClientNetworkSocketMap)
    {
        SSFSOCKET Socket = IterClient->first;
        SSF_PRT_CLIENT_SOCKET ClientSocket = IterClient->second;
        if (ClientSocket == nullptr)
        {
            continue;
        }

        SSFNetworkSession *Session = SessionManager.GetSession(Socket);
        if (Session == nullptr)
        {
            continue;
        }

        Session->InvalidPacketCount += ClientSocket->ConsumeInvalidPacketCount();

        std::vector<SSFNetworkPacket> Packets;
        ClientSocket->FetchPackets(Packets);
        for (const SSFNetworkPacket &Packet : Packets)
        {
            if (Session->RateWindowStartMS == 0 || (NowMS - Session->RateWindowStartMS) >= 1000)
            {
                Session->RateWindowStartMS = NowMS;
                Session->RateWindowMsgCount = 0;
            }

            ++Session->RateWindowMsgCount;
            if (Session->RateWindowMsgCount > MaxMsgPerSecond)
            {
                ++Session->DroppedMsgCount;
                SF_LOG_FRAMEWORK("Drop client msg by rate limit, Socket " << Socket
                                                                          << " MsgId " << Packet.MsgId
                                                                          << " WindowCount " << Session->RateWindowMsgCount
                                                                          << " MaxMsgPerSecond " << MaxMsgPerSecond);
                continue;
            }

            auto IterHandler = RouteHandlerMap.find(Packet.MsgId);
            if (IterHandler == RouteHandlerMap.end())
            {
                SF_LOG_FRAMEWORK("Unhandled client msg, Socket " << Socket << " MsgId " << Packet.MsgId);
                continue;
            }

            IterHandler->second(Socket, Packet);
        }
    }
}

void SFModule_NetworkServer::HandleHeartbeat(SSFSOCKET Socket, const SSFNetworkPacket &Packet)
{
    SSFNetworkSession *Session = SessionManager.GetSession(Socket);
    if (Session != nullptr)
    {
        Session->LastHeartbeatMS = GetSteadyNowMS_Server();
    }

    SSFNetworkPacket AckPacket;
    AckPacket.MsgId = static_cast<SFUInt16>(ESFNetworkMsg::S2C_HeartbeatAck);
    AckPacket.Seq = Packet.Seq;
    SendToClient(Socket, AckPacket);
}

void SFModule_NetworkServer::HandleLogin(SSFSOCKET Socket, const SSFNetworkPacket &Packet)
{
    SSFNetworkSession *Session = SessionManager.GetSession(Socket);
    if (Session == nullptr)
    {
        SF_LOG_FRAMEWORK("Client login failed, session not found, Socket " << Socket);
        return;
    }

    if (Session->IsAuthed == TRUE)
    {
        SF_LOG_FRAMEWORK("Client login ignored, already authed, Socket " << Socket
                                                                         << " PlayerId " << Session->PlayerId
                                                                         << " WorldId " << Session->WorldId);
        return;
    }

    SSFNetworkLoginPayload LoginPayload;
    if (!SSFNetworkLoginPayloadCodec::Decode(Packet.Body, LoginPayload))
    {
        SF_LOG_FRAMEWORK("Client login payload decode failed, Socket " << Socket << " BodyLen " << Packet.Body.size());
        return;
    }

    if (LoginPayload.PlayerId == 0 || LoginPayload.WorldId == 0 || LoginPayload.Token.empty())
    {
        SF_LOG_FRAMEWORK("Client login payload invalid, Socket " << Socket
                                                                 << " PlayerId " << LoginPayload.PlayerId
                                                                 << " WorldId " << LoginPayload.WorldId
                                                                 << " TokenEmpty " << LoginPayload.Token.empty());
        return;
    }

    Session->IsAuthed = TRUE;
    Session->PlayerId = LoginPayload.PlayerId;
    Session->WorldId = LoginPayload.WorldId;
    Session->AuthToken = LoginPayload.Token;

    SSFNetworkPacket AckPacket;
    AckPacket.MsgId = static_cast<SFUInt16>(ESFNetworkMsg::S2C_LoginAck);
    AckPacket.Seq = Packet.Seq;

    SF_LOG_FRAMEWORK("Client login success, Socket " << Socket
                                                     << " PlayerId " << Session->PlayerId
                                                     << " WorldId " << Session->WorldId);
    SendToClient(Socket, AckPacket);
}

void SFModule_NetworkServer::HandlePlayerInput(SSFSOCKET Socket, const SSFNetworkPacket &Packet)
{
    SSFNetworkSession *Session = SessionManager.GetSession(Socket);
    if (Session == nullptr)
    {
        SF_LOG_FRAMEWORK("Reject player input, session not found, Socket " << Socket);
        return;
    }

    if (Session->IsAuthed == FALSE)
    {
        SF_LOG_FRAMEWORK("Reject player input before login, Socket " << Socket);
        return;
    }

    SSFNetworkPacket StatePacket;
    StatePacket.MsgId = static_cast<SFUInt16>(ESFNetworkMsg::S2C_PlayerState);
    StatePacket.Seq = Packet.Seq;
    StatePacket.Body = Packet.Body;
    SendToClient(Socket, StatePacket);
}

bool SFModule_NetworkServer::SendToClient(SSFSOCKET Socket, const SSFNetworkPacket &Packet)
{
    auto IterClient = ClientNetworkSocketMap.find(Socket);
    if (IterClient == ClientNetworkSocketMap.end() || IterClient->second == nullptr)
    {
        SF_LOG_FRAMEWORK("SendToClient failed, client not found, Socket " << Socket << " MsgId " << Packet.MsgId);
        return false;
    }

    if (!IterClient->second->SendPacket(Packet))
    {
        SF_LOG_FRAMEWORK("SendToClient failed, send packet error, Socket " << Socket << " MsgId " << Packet.MsgId);
        return false;
    }

    return true;
}

bool SFModule_NetworkServer::IsBlacklisted(const SFString &ClientIP, SFUInt64 NowMS)
{
    auto Iter = BlacklistUntilMap.find(ClientIP);
    if (Iter == BlacklistUntilMap.end())
    {
        return false;
    }

    if (NowMS >= Iter->second)
    {
        BlacklistUntilMap.erase(Iter);
        BlacklistDirty = TRUE;
        return false;
    }

    return true;
}

void SFModule_NetworkServer::AddToBlacklist(const SFString &ClientIP, SFUInt64 NowMS, const SFString &Reason)
{
    if (ClientIP.empty() || ClientIP == "Unknown")
    {
        return;
    }

    SFUInt64 UntilMS = NowMS + BlacklistDurationMS;
    BlacklistUntilMap[ClientIP] = UntilMS;
    BlacklistDirty = TRUE;
    SaveBlacklistToFile();
    SF_LOG_FRAMEWORK("Add client to blacklist, IP " << ClientIP << " UntilMS " << UntilMS << " Reason " << Reason);
}

void SFModule_NetworkServer::LoadBlacklistFromFile(SFUInt64 NowMS)
{
    if (!BlacklistPersistenceEnabled)
    {
        return;
    }

    SSFNetworkBlacklistSnapshot Snapshot;
    Snapshot.FormatVersion = BlacklistFormatVersion;

    if (!SSFNetworkBlacklistStore::LoadFromFile(BlacklistFilePath, NowMS, Snapshot, &BlacklistFileLastWriteTime))
    {
        return;
    }

    BlacklistFormatVersion = Snapshot.FormatVersion;
    BlacklistUntilMap = Snapshot.Entries;

    if (!BlacklistUntilMap.empty())
    {
        SF_LOG_FRAMEWORK("Load blacklist success, Count " << BlacklistUntilMap.size() << " File " << BlacklistFilePath);
    }
}

void SFModule_NetworkServer::SaveBlacklistToFile()
{
    if (!BlacklistPersistenceEnabled || !BlacklistDirty)
    {
        return;
    }

    SSFNetworkBlacklistSnapshot Snapshot;
    Snapshot.FormatVersion = BlacklistFormatVersion;
    Snapshot.Entries = BlacklistUntilMap;

    if (!SSFNetworkBlacklistStore::SaveToFile(BlacklistFilePath, Snapshot, &BlacklistFileLastWriteTime))
    {
        SF_LOG_ERROR("Save blacklist failed, File " << BlacklistFilePath);
        return;
    }

    BlacklistDirty = FALSE;
}

void SFModule_NetworkServer::ReloadBlacklistIfNeeded(SFUInt64 NowMS)
{
    if (!BlacklistPersistenceEnabled || !BlacklistHotReloadEnabled)
    {
        return;
    }

    if (LastBlacklistReloadMS != 0 && (NowMS - LastBlacklistReloadMS) < BlacklistReloadIntervalMS)
    {
        return;
    }
    LastBlacklistReloadMS = NowMS;

    if (!std::filesystem::exists(BlacklistFilePath))
    {
        return;
    }

    auto CurrentWriteTime = std::filesystem::last_write_time(BlacklistFilePath);
    if (BlacklistFileLastWriteTime == std::filesystem::file_time_type{} || CurrentWriteTime != BlacklistFileLastWriteTime)
    {
        LoadBlacklistFromFile(NowMS);
        BlacklistDirty = FALSE;
        SF_LOG_FRAMEWORK("Hot reload blacklist, File " << BlacklistFilePath);
    }
}
