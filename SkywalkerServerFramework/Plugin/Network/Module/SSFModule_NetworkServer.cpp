/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Module\SSFModule_NetworkServer.cpp
**作者: shyfan
**日期: 2023/08/26 15:37:24
**功能: 网络服务器模块
*************************************************************************/

#include "SSFModule_NetworkServer.h"

#include "Include/SSFILog.h"

SKYWALKER_SF_NAMESPACE_USE

SKYWALKER_SF_LOG_DEFINE(SSFModule_NetworkServer, LogLevel_Debug);

#pragma region Object

void SSFModule_NetworkServer::Init(SSFObjectErrors &Errors)
{
    SSFOModule::Init(Errors);

    ClientSocket = INVALID_SOCKET;

    ServerNetworkSocket = SSF_NEW_OBJECT(SSFObject_NetworkSocket);
}

void SSFModule_NetworkServer::Awake(SSFObjectErrors &Errors)
{
    SSFOModule::Awake(Errors);
}

void SSFModule_NetworkServer::Start(SSFObjectErrors &Errors)
{
    SSFOModule::Start(Errors);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        SKYWALKER_SF_LOG_ERROR("Failed to initialize winsock")
        return;
    }

    SSFNetworkSocketCreatorContext Context;

    // 创建服务器套接字
    Context.Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (Context.Socket == INVALID_SOCKET)
    {
        SKYWALKER_SF_LOG_ERROR("Failed to create socket")
        WSACleanup();
        return;
    }

    ServerNetworkSocket->Create(Errors, Context);

    SOCKET ServerSocket = ServerNetworkSocket->GetSocket();

    SKYWALKER_SF_LOG_DEBUG("ServerSocket: " << ServerSocket)

    // 绑定IP地址和端口号
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9527); // 设置端口号
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(ServerSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        SKYWALKER_SF_LOG_ERROR("Failed to bind address to the socket")
        closesocket(ServerSocket);
        WSACleanup();
        return;
    }

    // 监听和接受连接请求
    if (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        SKYWALKER_SF_LOG_ERROR("Failed to listen")
        closesocket(ServerSocket);
        WSACleanup();
        return;
    }
}

void SSFModule_NetworkServer::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SSFOModule::Tick(Errors, DelayMS);

    // 接受客户端连接
    if (ClientSocket != INVALID_SOCKET)
    {
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
    else
    {
        SOCKET ServerSocket = ServerNetworkSocket->GetSocket();

        ClientSocket = accept(ServerSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET)
        {
            closesocket(ServerSocket);
            WSACleanup();
            return;
        }

        SKYWALKER_SF_LOG_DEBUG("ClientSocket: " << ClientSocket)
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
