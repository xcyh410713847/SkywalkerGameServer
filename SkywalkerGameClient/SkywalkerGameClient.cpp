﻿/*************************************************************************
**文件: SkywalkerGameClient\Executable\SkywalkerGameClient.cpp
**作者: shyfan
**日期: 2023/08/07 11:34:09
**功能: SkywalkerGameClient
*************************************************************************/
#include "SkywalkerPlatform/SkywalkerPlatform.h"

#if defined(SKYWALKER_PLATFORM_WINDOWS)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[])
{
    std::cout << "Hello SkywalkerGameClient" << std::endl;

#if defined(SKYWALKER_PLATFORM_WINDOWS)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "Failed to initialize winsock" << std::endl;
        return 1;
    }
    // 创建客户端套接字
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cout << "Failed to create socket" << std::endl;
        WSACleanup();
        return 1;
    }

    // 设置服务器的IP地址和端口号
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9527); // 设置服务器端口号
    inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr));

    // 连接服务器
    if (connect(clientSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cout << "Failed to connect to server" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // 与服务器进行数据通信
    char buffer[1024];
    int bytesReceived;
    while (true)
    {
        // 发送数据给服务器
        std::cout << "Enter message: ";
        std::cin.getline(buffer, sizeof(buffer));
        send(clientSocket, buffer, strlen(buffer), 0);

        // 接收服务器的回复
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0)
        {
            std::cout << "Server disconnected" << std::endl;
            break;
        }

        // 处理接收到的数据
        buffer[bytesReceived] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }

    // 关闭套接字和清理Winsock库
    closesocket(clientSocket);
    WSACleanup();
#else
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9527);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }

    std::string message = "Hello, Server!";
    if (send(sock, message.c_str(), message.length(), 0) == -1)
    {
        std::cerr << "Send failed" << std::endl;
        return 1;
    }

    char buffer[1024] = {0};
    int valread = read(sock, buffer, 1024);
    if (valread == -1)
    {
        std::cerr << "Read failed" << std::endl;
        return 1;
    }

    std::cout << "Server response: " << buffer << std::endl;

    close(sock);
#endif

    return 0;
}
