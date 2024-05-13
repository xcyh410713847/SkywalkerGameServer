/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Module\SSFModule_NetworkServer.cpp
**作者: shyfan
**日期: 2023/08/26 15:37:24
**功能: 网络服务器模块
*************************************************************************/

#include "SSFModule_NetworkServer.h"

#include "Include/SSFCore.h"
#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFModule_NetworkServer, LogLevel_Framework);

#pragma region Object

void SSFModule_NetworkServer::Init(SSFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
}

void SSFModule_NetworkServer::Awake(SSFObjectErrors &Errors)
{
    SSFModule::Awake(Errors);
}

void SSFModule_NetworkServer::Start(SSFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

    StartNetworkServer(Errors);
}

void SSFModule_NetworkServer::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);

    CreateNetworkClient(Errors);

    SSF_COMMON_ITERATOR(Iter, ClientNetworkSocketMap)
    {
        Iter->second->Tick(Errors, DelayMS);
    }
}

void SSFModule_NetworkServer::Stop(SSFObjectErrors &Errors)
{
    StopNetworkServer(Errors);

    SSFModule::Stop(Errors);
}

void SSFModule_NetworkServer::Sleep(SSFObjectErrors &Errors)
{
    SSFModule::Sleep(Errors);
}

void SSFModule_NetworkServer::Destroy(SSFObjectErrors &Errors)
{
    SSFModule::Destroy(Errors);
}

#pragma endregion Object

void SSFModule_NetworkServer::StartNetworkServer(SSFObjectErrors &InErrors)
{
    if (InErrors.IsValid())
    {
        return;
    }

    SSF_NETWORK_STARTUP();

    // 创建服务器套接字
    SSFNetworkSocketCreatorContext Context;
    auto pServerSocket = NewObject<SSFObject_ServerSocket>(Context, InErrors);
    ServerNetworkSocket = SSF_UNIQUE_PTR_CAST(SSFObject_ServerSocket, pServerSocket);

    if (InErrors.IsValid())
    {
        SSF_ERROR_DESC_TRACE(InErrors,
                             SkywalkerSFError_Network_Start_Failed,
                             "Failed to start network server");
        SSF_NETWORK_CLEANUP();

        return;
    }

    SSF_LOG_FRAMEWORK("Create ServerSocket ObjectGUID " << ServerNetworkSocket->GetObjectGUID() << " Socket " << ServerNetworkSocket->GetSocket());
}

void SSFModule_NetworkServer::StopNetworkServer(SSFObjectErrors &Errors)
{
    ServerNetworkSocket->Stop(Errors);

    SSF_NETWORK_CLEANUP();
}

void SSFModule_NetworkServer::CreateNetworkClient(SSFObjectErrors &Errors)
{
    if (ServerNetworkSocket->IsSocketInvalid())
    {
        SSF_ERROR_DESC_TRACE(
            Errors,
            SkywalkerSFError_Network_Socket_CreateFailed,
            "Create ClientSocket Failed, Because ServerSocket is Invalid")

        return;
    }

    SSFSOCKET ClientSocket = accept(ServerNetworkSocket->GetSocket(), NULL, NULL);
    if (ClientSocket == SSF_INVALID_SOCKET)
    {
        return;
    }

    // 创建客户端套接字
    SSFNetworkSocketCreatorContext Context;
    Context.Socket = ClientSocket;
    SSF_PRT_CLIENT_SOCKET ClientNetworkSocket = NewObject<SSFObject_ClientSocket>(Context, Errors);
    ClientNetworkSocketMap.insert(std::make_pair(ClientSocket, ClientNetworkSocket));

    SSF_LOG_DEBUG("New ClientSocket: " << ClientSocket);
}
