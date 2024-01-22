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

SSF_LOG_DEFINE(SSFModule_NetworkServer, LogLevel_Debug);

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
    SSFModule::Stop(Errors);

    ServerNetworkSocket->Stop(Errors);

    WSACleanup();
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

void SSFModule_NetworkServer::StartNetworkServer(SSFObjectErrors &Errors)
{
    if (Errors.IsValid())
    {
        return;
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        SSF_ERROR_DESC_TRACE(Errors,
                             SkywalkerSFError_Network_Init_Failed,
                             "Failed to initialize winsock")
        return;
    }

    // 创建服务器套接字
    ServerNetworkSocket = new SSFObject_ServerSocket();
    SSFNetworkSocketCreatorContext Context;
    ServerNetworkSocket->Create(Errors, Context);

    if (Errors.IsValid())
    {
        SSF_ERROR_DESC_TRACE(Errors,
                             SkywalkerSFError_Network_Start_Failed,
                             "Failed to start network server")
        WSACleanup();
        return;
    }
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

    SOCKET ClientSocket = accept(ServerNetworkSocket->GetSocket(), NULL, NULL);
    if (ClientSocket == INVALID_SOCKET)
    {
        return;
    }

    SSF_PRT_CLIENT_SOCKET ClientNetworkSocket = new SSFObject_ClientSocket();

    // 创建客户端套接字
    SSFNetworkSocketCreatorContext Context;
    Context.Socket = ClientSocket;
    ClientNetworkSocket->Create(Errors, Context);

    ClientNetworkSocketMap.insert(std::make_pair(ClientSocket, ClientNetworkSocket));

    SSF_LOG_DEBUG("New ClientSocket: " << ClientSocket);
}
