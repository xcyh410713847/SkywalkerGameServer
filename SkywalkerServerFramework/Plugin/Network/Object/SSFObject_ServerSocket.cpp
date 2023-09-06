/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Object\SSFObject_ServerSocket.cpp
**作者: shyfan
**日期: 2023/09/06 20:16:10
**功能: 服务器套接字对象
*************************************************************************/

#include "SSFObject_ServerSocket.h"

SKYWALKER_SF_NAMESPACE_USING

#pragma region Object

void SSFObject_ServerSocket::Stop(SSFObjectErrors &Errors)
{
    SSFObject_NetworkSocket::Stop(Errors);

    closesocket(GetSocket());
    WSACleanup();
}

#pragma endregion Object

void SSFObject_ServerSocket::Create(SSFObjectErrors &Errors, SSFNetworkSocketCreatorContext &Context)
{
    SSFObject_NetworkSocket::Create(Errors, Context);

    SOCKET ServerSocket = GetSocket();

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
