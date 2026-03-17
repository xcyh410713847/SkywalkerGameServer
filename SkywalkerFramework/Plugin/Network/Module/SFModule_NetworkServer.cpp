/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Module\SFModule_NetworkServer.cpp
**作者: shyfan
**日期: 2023/08/26 15:37:24
**功能: 网络服务器模块
*************************************************************************/

#include "SFModule_NetworkServer.h"

#include "Include/SFCore.h"
#include "Include/SFILog.h"

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

#pragma region Object

void SFModule_NetworkServer::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
}

void SFModule_NetworkServer::Awake(SFObjectErrors &Errors)
{
    SSFModule::Awake(Errors);
}

void SFModule_NetworkServer::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

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

    SSF_NETWORK_CLEANUP();
}

void SFModule_NetworkServer::CreateNetworkClient(SFObjectErrors &Errors)
{
    if (ServerNetworkSocket->IsSocketInvalid())
    {
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

    SSFNetworkSocketCreatorContext Context;
    Context.Socket = ClientSocket;
    SSF_PRT_CLIENT_SOCKET ClientNetworkSocket = NewObject<SSFObject_ClientSocket>(Context, Errors);
    if (Errors.IsValid() || ClientNetworkSocket == nullptr)
    {
        SSF_CLOSE_SOCKET(ClientSocket);
        return;
    }

    ClientNetworkSocketMap.insert(std::make_pair(ClientSocket, ClientNetworkSocket));

    SF_LOG_FRAMEWORK("New ClientSocket: " << ClientSocket);
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
}
