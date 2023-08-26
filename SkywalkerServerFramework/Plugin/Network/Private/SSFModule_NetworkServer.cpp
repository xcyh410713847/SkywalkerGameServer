/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Private\SSFModule_NetworkServer.cpp
**作者: shyfan
**日期: 2023/08/26 09:20:07
**功能: 网络服务器模块
*************************************************************************/

#include "SSFModule_NetworkServer.h"

SKYWALKER_SF_NAMESPACE_USE

#pragma region Object

void SSFOModule_NetworkServer::Init(SSFObjectErrors &Errors)
{
    SSFOModule::Init(Errors);

    ServerSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;
}

void SSFOModule_NetworkServer::Awake(SSFObjectErrors &Errors)
{
    SSFOModule::Awake(Errors);
}

void SSFOModule_NetworkServer::Start(SSFObjectErrors &Errors)
{
    SSFOModule::Start(Errors);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "Failed to initialize winsock" << std::endl;
        return;
    }
    // 创建服务器套接字
    ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerSocket == INVALID_SOCKET)
    {
        std::cout << "Failed to create socket" << std::endl;
        WSACleanup();
        return;
    }

    // 绑定IP地址和端口号
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9527); // 设置端口号
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(ServerSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cout << "Failed to bind address to the socket" << std::endl;
        closesocket(ServerSocket);
        WSACleanup();
        return;
    }

    // 监听和接受连接请求
    if (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cout << "Failed to listen" << std::endl;
        closesocket(ServerSocket);
        WSACleanup();
        return;
    }
}

void SSFOModule_NetworkServer::Tick(SSFObjectErrors &Errors, int DelayMS)
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
            std::cout << "Client disconnected" << std::endl;
            closesocket(ClientSocket);
            ClientSocket = INVALID_SOCKET;
            return;
        }

        // 处理接收到的数据
        std::cout << "Received: " << buffer << std::endl;

        // 回复客户端
        send(ClientSocket, buffer, bytesReceived, 0);
    }
    else
    {
        ClientSocket = accept(ServerSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET)
        {
            std::cout << "Waiting for client connection..." << std::endl;
            closesocket(ServerSocket);
            WSACleanup();
            return;
        }

        std::cout << "Client connected" << std::endl;
    }
}

void SSFOModule_NetworkServer::Stop(SSFObjectErrors &Errors)
{
    SSFOModule::Stop(Errors);

    closesocket(ServerSocket);
    WSACleanup();
}

void SSFOModule_NetworkServer::Sleep(SSFObjectErrors &Errors)
{
    SSFOModule::Sleep(Errors);
}

void SSFOModule_NetworkServer::Destroy(SSFObjectErrors &Errors)
{
    SSFOModule::Destroy(Errors);
}

void SSFOModule_NetworkServer::Release(SSFObjectErrors &Errors)
{
    SSFOModule::Release(Errors);
}

#pragma endregion Object
