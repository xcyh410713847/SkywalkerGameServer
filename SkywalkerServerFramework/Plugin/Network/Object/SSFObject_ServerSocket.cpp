/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Object\SSFObject_ServerSocket.cpp
**作者: shyfan
**日期: 2023/09/06 20:16:10
**功能: 服务器套接字对象
*************************************************************************/

#include "SSFObject_ServerSocket.h"

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFObject_ServerSocket, LogLevel_Debug);

SSFObject_ServerSocket::SSFObject_ServerSocket(SSFNetworkSocketCreatorContext &InContext, SSFObjectErrors &InErrors)
    : SSFObject_NetworkSocket(InContext, InErrors)
{
    // 创建套接字
#if defined(SKYWALKER_PLATFORM_WINDOWS)
    Socket = socket(AF_INET, SOCK_STREAM, 0);
#else
    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif
    if (Socket == SSF_INVALID_SOCKET)
    {
        SSF_ERROR_DESC_TRACE(InErrors,
                             SkywalkerSFError_Network_Socket_CreateFailed,
                             "Failed to create socket")
        return;
    }

    SSFSOCKET ServerSocket = GetSocket();

    // 设置套接字为非阻塞模式
    u_long mode = 1; // 将非阻塞模式设置为1

    // 设置套接字为非阻塞模式
#if defined(SKYWALKER_PLATFORM_WINDOWS)
    int result = ioctlsocket(ServerSocket, FIONBIO, &mode);
    if (result == SSF_SOCKET_ERROR)
    {
        SSF_ERROR_DESC_TRACE(InErrors,
                             SkywalkerSFError_Network_Socket_SetFailed,
                             "Failed to set socket to non-blocking mode");
        closesocket(ServerSocket);
        return;
    }
#else
    int flags = fcntl(ServerSocket, F_GETFL, 0);
    fcntl(ServerSocket, F_SETFL, flags | O_NONBLOCK);
#endif

    // 绑定IP地址和端口号
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9527); // 设置端口号
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(ServerSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SSF_SOCKET_ERROR)
    {
        SSF_ERROR_DESC_TRACE(InErrors,
                             SkywalkerSFError_Network_Socket_BindFailed,
                             "Failed to bind address to the socket");
        // 关闭套接字
        #if defined(SKYWALKER_PLATFORM_WINDOWS)
        closesocket(ServerSocket);
        #else
        close(ServerSocket);
        #endif
        return;
    }

    // 监听和接受连接请求
    if (listen(ServerSocket, SOMAXCONN) == SSF_SOCKET_ERROR)
    {
        SSF_ERROR_DESC_TRACE(InErrors,
                             SkywalkerSFError_Network_Socket_ListenFailed,
                             "Failed to listen")
        // 关闭套接字
#if defined(SKYWALKER_PLATFORM_WINDOWS)
        closesocket(ServerSocket);
#else
        close(ServerSocket);
#endif
        return;
    }
}

SSFObject_ServerSocket::~SSFObject_ServerSocket()
{
}

void SSFObject_ServerSocket::Stop(SSFObjectErrors &Errors)
{
    SSFObject_NetworkSocket::Stop(Errors);

    // 关闭套接字
#if defined(SKYWALKER_PLATFORM_WINDOWS)
    closesocket(GetSocket());
#else
    close(GetSocket());
#endif
}
