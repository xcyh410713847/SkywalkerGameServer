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

SKYWALKER_SF_LOG_DEFINE(SSFModule_NetworkServer, LogLevel_Debug);

#pragma region Object

void SSFModule_NetworkServer::Init(SSFObjectErrors &Errors)
{
    SSFOModule::Init(Errors);
}

void SSFModule_NetworkServer::Awake(SSFObjectErrors &Errors)
{
    SSFOModule::Awake(Errors);
}

void SSFModule_NetworkServer::Start(SSFObjectErrors &Errors)
{
    SSFOModule::Start(Errors);

    StartNetworkServer(Errors);
}

void SSFModule_NetworkServer::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SSFOModule::Tick(Errors, DelayMS);

    CreateNetworkClient(Errors);

    SSF_COMMON_ITERATOR(Iter, ClientNetworkSocketMap)
    {
        Iter->second->Tick(Errors, DelayMS);
    }
}

void SSFModule_NetworkServer::Stop(SSFObjectErrors &Errors)
{
    SSFOModule::Stop(Errors);

    ServerNetworkSocket->Stop(Errors);

    WSACleanup();
}

void SSFModule_NetworkServer::Sleep(SSFObjectErrors &Errors)
{
    SSFOModule::Sleep(Errors);
}

void SSFModule_NetworkServer::Destroy(SSFObjectErrors &Errors)
{
    SSFOModule::Destroy(Errors);
}

void SSFModule_NetworkServer::Release(SSFObjectErrors &Errors)
{
    SSFOModule::Release(Errors);
}

#pragma endregion Object

void SSFModule_NetworkServer::StartNetworkServer(SSFNetworkErrors &Errors)
{
    if (Errors.IsValid())
    {
        return;
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors,
                                      SkywalkerSFError_Network_Init_Failed,
                                      "Failed to initialize winsock")
        return;
    }

    // 创建服务器套接字
    ServerNetworkSocket = SSF_NEW_OBJECT(SSFObject_ServerSocket);
    SSFNetworkSocketCreatorContext Context;
    ServerNetworkSocket->Create(Errors, Context);

    if (Errors.IsValid())
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors,
                                      SkywalkerSFError_Network_Start_Failed,
                                      "Failed to start network server")
        WSACleanup();
        return;
    }
}

void SSFModule_NetworkServer::CreateNetworkClient(SSFNetworkErrors &Errors)
{
    if (ServerNetworkSocket->IsSocketInvalid())
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(
            Errors,
            SkywalkerSFError_Network_Socket_CreateFailed,
            "Create ClientSocket Failed, Because ServerSocket is Invalid")

        return;
    }

    SOCKET ClientSocket = accept(ServerNetworkSocket->GetSocket(), NULL, NULL);
    if (ClientSocket == INVALID_SOCKET)
    {
        return;
    }

    SSF_PRT_CLIENT_SOCKET ClientNetworkSocket = SSF_NEW_OBJECT(SSFObject_ClientSocket);

    // 创建客户端套接字
    SSFNetworkSocketCreatorContext Context;
    Context.Socket = ClientSocket;
    ClientNetworkSocket->Create(Errors, Context);

    ClientNetworkSocketMap.insert(std::make_pair(ClientSocket, ClientNetworkSocket));

    SKYWALKER_SF_LOG_DEBUG("New ClientSocket: " << ClientSocket);
}
