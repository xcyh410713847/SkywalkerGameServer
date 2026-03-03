/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Object\SFObject_ServerSocket.cpp
**作者: shyfan
**日期: 2023/09/06 20:16:10
**功能: 服务器套接字对象
*************************************************************************/

#include "SFObject_ServerSocket.h"

#include "Include/SFILog.h"

#include "SkywalkerPlatform/SkywalkerPlatform.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFObject_ServerSocket, ESFLogLevel::Debug);

SSFObject_ServerSocket::SSFObject_ServerSocket(SSFNetworkSocketCreatorContext &InContext, SFObjectErrors &InErrors)
    : SSFObject_NetworkSocket(InContext, InErrors)
{
    // 创建套接字
    Socket = SSF_SOCKET_CREATE(AF_INET, SOCK_STREAM);
    if (Socket == SSF_INVALID_SOCKET)
    {
        SF_ERROR_DESC_TRACE(InErrors,
                            ESFError::Network_Socket_CreateFailed,
                            "Failed to create socket")
        return;
    }

    SSFSOCKET ServerSocket = GetSocket();

    // 设置套接字为非阻塞模式
    u_long mode = 1; // 将非阻塞模式设置为1

    // 设置套接字为非阻塞模式
    SSF_SOCKET_SET_NONBLOCKING(ServerSocket);

    // 绑定IP地址和端口号
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(InContext.Port);

    if (!InContext.IP.empty())
    {
        SkywalkerInetPton(AF_INET, InContext.IP.c_str(), &serverAddr.sin_addr);
    }
    else
    {
        serverAddr.sin_addr.s_addr = INADDR_ANY;
    }

    if (bind(ServerSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SSF_SOCKET_ERROR)
    {
        SF_ERROR_DESC_TRACE(InErrors,
                            ESFError::Network_Socket_BindFailed,
                            "Failed to bind address to the socket");
        // 关闭套接字
        SSF_CLOSE_SOCKET(ServerSocket);

        return;
    }

    // 监听和接受连接请求
    if (listen(ServerSocket, SOMAXCONN) == SSF_SOCKET_ERROR)
    {
        SF_ERROR_DESC_TRACE(InErrors,
                            ESFError::Network_Socket_ListenFailed,
                            "Failed to listen")
        // 关闭套接字
        SSF_CLOSE_SOCKET(ServerSocket);

        return;
    }
}

SSFObject_ServerSocket::~SSFObject_ServerSocket()
{
}

void SSFObject_ServerSocket::Stop(SFObjectErrors &Errors)
{
    SSFObject_NetworkSocket::Stop(Errors);

    // 关闭套接字
    SSF_CLOSE_SOCKET(GetSocket());
}
