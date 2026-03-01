/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Module\SSFModule_NetworkClient.cpp
**作者: shyfan
**日期: 2023/08/26 15:37:47
**功能: 网络客户端模块
*************************************************************************/

#include "SSFModule_NetworkClient.h"

#include "Include/SFCore.h"
#include "Include/SFILog.h"

#if defined(SKYWALKER_PLATFORM_WINDOWS)
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#endif

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_NetworkClient, Framework);

#pragma region Object

void SSFModule_NetworkClient::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
}

void SSFModule_NetworkClient::Awake(SFObjectErrors &Errors)
{
    SSFModule::Awake(Errors);
}

void SSFModule_NetworkClient::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

    StartNetworkClient(Errors);
}

void SSFModule_NetworkClient::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);

    if (bIsConnected && ClientNetworkSocket != nullptr)
    {
        HandleReceive(Errors);
    }
}

void SSFModule_NetworkClient::Stop(SFObjectErrors &Errors)
{
    StopNetworkClient(Errors);

    SSFModule::Stop(Errors);
}

void SSFModule_NetworkClient::Sleep(SFObjectErrors &Errors)
{
    SSFModule::Sleep(Errors);
}

void SSFModule_NetworkClient::Destroy(SFObjectErrors &Errors)
{
    SSFModule::Destroy(Errors);
}

#pragma endregion Object

bool SSFModule_NetworkClient::Connect(const char *IP, int Port)
{
    SFObjectErrors InErrors;

    if (bIsConnected && ClientNetworkSocket != nullptr)
    {
        SF_LOG_ERROR("Already connected to server");
        return false;
    }

    ServerIP = IP;
    ServerPort = Port;

    SSFNetworkSocketCreatorContext Context;
    Context.Socket = SSF_SOCKET_CREATE(AF_INET, SOCK_STREAM);

    if (Context.Socket == SSF_INVALID_SOCKET)
    {
        SF_LOG_ERROR("Failed to create socket");
        return false;
    }

    struct sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(static_cast<u_short>(Port));
#if defined(SKYWALKER_PLATFORM_WINDOWS)
    inet_pton(AF_INET, IP, &ServerAddr.sin_addr);
#else
    ServerAddr.sin_addr.s_addr = inet_addr(IP);
#endif

    if (connect(Context.Socket, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) < 0)
    {
        SSF_CLOSE_SOCKET(Context.Socket);
        SF_LOG_ERROR("Failed to connect to server " << IP << ":" << Port);
        return false;
    }

    // 设置为非阻塞模式
#if defined(SKYWALKER_PLATFORM_WINDOWS)
    u_long mode = 1;
    if (ioctlsocket(Context.Socket, FIONBIO, &mode) == SSF_SOCKET_ERROR)
    {
        SSF_CLOSE_SOCKET(Context.Socket);
        SF_LOG_ERROR("Failed to set socket to non-blocking mode");
        return false;
    }
#else
    int flags = fcntl(Context.Socket, F_GETFL, 0);
    fcntl(Context.Socket, F_SETFL, flags | O_NONBLOCK);
#endif

    auto pClientSocket = NewObject<SSFObject_NetworkSocket>(Context, InErrors);
    ClientNetworkSocket = static_cast<SSF_PRT_NETWORK_SOCKET>(pClientSocket);

    bIsConnected = true;
    SF_LOG_FRAMEWORK("Connected to server " << IP << ":" << Port << " Socket " << ClientNetworkSocket->GetSocket());

    return true;
}

void SSFModule_NetworkClient::Disconnect()
{
    if (ClientNetworkSocket != nullptr && !ClientNetworkSocket->IsSocketInvalid())
    {
        SSF_CLOSE_SOCKET(ClientNetworkSocket->GetSocket());
        ClientNetworkSocket = nullptr;
    }

    bIsConnected = false;
    ServerIP.clear();
    ServerPort = 0;

    SF_LOG_FRAMEWORK("Disconnected from server");
}

bool SSFModule_NetworkClient::IsConnected() const
{
    return bIsConnected && ClientNetworkSocket != nullptr && ClientNetworkSocket->IsSocketValid();
}

int SSFModule_NetworkClient::Send(const char *Data, int Length)
{
    if (!IsConnected())
    {
        SF_LOG_ERROR("Not connected to server");
        return -1;
    }

    int SentBytes = SSF_SOCKET_WRITE(ClientNetworkSocket->GetSocket(), Data, Length, 0);
    if (SentBytes < 0)
    {
        SF_LOG_ERROR("Failed to send data");
        return -1;
    }

    return SentBytes;
}

void SSFModule_NetworkClient::StartNetworkClient(SFObjectErrors &Errors)
{
    if (Errors.IsValid())
    {
        return;
    }

#if defined(SKYWALKER_PLATFORM_WINDOWS)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        SF_LOG_ERROR("Failed to initialize winsock");
        return;
    }
#endif

    SF_LOG_FRAMEWORK("Network client started");
}

void SSFModule_NetworkClient::StopNetworkClient(SFObjectErrors &Errors)
{
    Disconnect();

    SSF_NETWORK_CLEANUP();

    SF_LOG_FRAMEWORK("Network client stopped");
}

void SSFModule_NetworkClient::HandleReceive(SFObjectErrors &Errors)
{
    if (ClientNetworkSocket == nullptr || ClientNetworkSocket->IsSocketInvalid())
    {
        return;
    }

    char Buffer[4096];
    int ReceivedBytes = SSF_SOCKET_READ(ClientNetworkSocket->GetSocket(), Buffer, sizeof(Buffer), 0);

    if (ReceivedBytes > 0)
    {
        SF_LOG_DEBUG("Received " << ReceivedBytes << " bytes from server");
    }
    else if (ReceivedBytes == 0)
    {
        SF_LOG_DEBUG("Server closed connection");
        Disconnect();
    }
    else
    {
#if defined(SKYWALKER_PLATFORM_WINDOWS)
        int Error = WSAGetLastError();
        if (Error != WSAEWOULDBLOCK)
        {
            SF_LOG_ERROR("Socket error: " << Error);
            Disconnect();
        }
#else
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            SF_LOG_ERROR("Socket error: " << errno);
            Disconnect();
        }
#endif
    }
}
