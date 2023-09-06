/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Module\SSFModule_NetworkServer.cpp
**作者: shyfan
**日期: 2023/08/26 15:37:24
**功能: 网络服务器模块
*************************************************************************/

#include "SSFModule_NetworkServer.h"

#include "Include/SSFCore.h"
#include "Include/SSFILog.h"

SKYWALKER_SF_NAMESPACE_USE

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

    CreateNetworkServer(Errors);
}

void SSFModule_NetworkServer::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SSFOModule::Tick(Errors, DelayMS);

    CreateNetworkClient(Errors);

    SKYWALKER_SF_COMMON_ITERATOR(Iter, ClientNetworkSocketMap)
    {
        SSF_PRT_NETWORK_SOCKET ClientNetworkSocket = Iter->second;

        SOCKET ClientSocket = ClientNetworkSocket->GetSocket();

        // 与客户端进行数据通信
        char buffer[1024];
        int bytesReceived;

        // 接收客户端发送的数据
        bytesReceived = recv(ClientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0)
        {
            SKYWALKER_SF_LOG_DEBUG("ClientSocket " << ClientSocket << " disconnected")
            closesocket(ClientSocket);
            ClientSocket = INVALID_SOCKET;
            return;
        }

        // 处理接收到的数据
        SKYWALKER_SF_LOG_DEBUG("Received: " << buffer)
        // 回复客户端
        send(ClientSocket, buffer, bytesReceived, 0);
    }
}

void SSFModule_NetworkServer::Stop(SSFObjectErrors &Errors)
{
    SSFOModule::Stop(Errors);

    SOCKET ServerSocket = ServerNetworkSocket->GetSocket();

    closesocket(ServerSocket);
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

void SSFModule_NetworkServer::CreateNetworkServer(SSFNetworkErrors &Errors)
{
    if (Errors.IsValid())
    {
        return;
    }

    ServerNetworkSocket = SSF_NEW_OBJECT(SSFObject_NetworkSocket);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Network_Init_Failed, "Failed to initialize winsock")
        return;
    }

    // 创建服务器套接字
    SSFNetworkSocketCreatorContext Context;
    Context.Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (Context.Socket == INVALID_SOCKET)
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Network_Socket_CreateFailed, "Failed to create socket")
        WSACleanup();
        return;
    }

    ServerNetworkSocket->Create(Errors, Context);
    ServerNetworkSocket->Init(Errors);
    ServerNetworkSocket->Awake(Errors);
    ServerNetworkSocket->Start(Errors);

    SOCKET ServerSocket = ServerNetworkSocket->GetSocket();

    // 绑定IP地址和端口号
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9527); // 设置端口号
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(ServerSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Network_Socket_BindFailed, "Failed to bind address to the socket")
        closesocket(ServerSocket);
        WSACleanup();
        return;
    }

    // 监听和接受连接请求
    if (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Network_Socket_ListenFailed, "Failed to listen")
        closesocket(ServerSocket);
        WSACleanup();
        return;
    }
}

void SSFModule_NetworkServer::CreateNetworkClient(SSFNetworkErrors &Errors)
{
    SOCKET ServerSocket = ServerNetworkSocket->GetSocket();

    SOCKET ClientSocket = accept(ServerSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET)
    {
        return;
    }

    SSF_PRT_NETWORK_SOCKET ClientNetworkSocket = SSF_NEW_OBJECT(SSFObject_NetworkSocket);

    // 创建客户端套接字
    SSFNetworkSocketCreatorContext Context;
    Context.Socket = ClientSocket;
    ClientNetworkSocket->Create(Errors, Context);
    ClientNetworkSocket->Init(Errors);
    ClientNetworkSocket->Awake(Errors);
    ClientNetworkSocket->Start(Errors);

    ClientNetworkSocketMap.insert(std::make_pair(ClientSocket, ClientNetworkSocket));

    SKYWALKER_SF_LOG_DEBUG("New ClientSocket: " << ClientSocket);
}
