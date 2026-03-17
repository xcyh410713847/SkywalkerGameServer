/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Object\SFObject_ClientSocket.cpp
**作者: shyfan
**日期: 2023/09/06 20:16:33
**功能: 客户端套接字对象
*************************************************************************/

#include "SFObject_ClientSocket.h"

#include "Include/SFILog.h"

#include <chrono>
#include <cerrno>

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFObject_ClientSocket, ESFLogLevel::Debug);

SSFObject_ClientSocket::SSFObject_ClientSocket(SSFNetworkSocketCreatorContext &InContext, SFObjectErrors &InErrors)
    : SSFObject_NetworkSocket(InContext, InErrors)
{
}

SSFObject_ClientSocket::~SSFObject_ClientSocket()
{
}

void SSFObject_ClientSocket::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFObject_NetworkSocket::Tick(Errors, DelayMS);

    // 获取客户端套接字
    SSFSOCKET ClientSocket = GetSocket();

    // 检查客户端套接字是否有效
    if (IsSocketInvalid())
    {
        return;
    }

    // 与客户端进行数据通信
    char buffer[1024];
    int bytesReceived = SSF_SOCKET_READ(ClientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0)
    {
#if defined(SKYWALKER_PLATFORM_WINDOWS)
        if (bytesReceived == 0)
        {
            bClosed = true;
        }
        else
        {
            int Error = WSAGetLastError();
            if (Error != WSAEWOULDBLOCK)
            {
                bClosed = true;
            }
        }
#else
        if (bytesReceived == 0)
        {
            bClosed = true;
        }
        else if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            bClosed = true;
        }
#endif
        return;
    }

    LastRecvMS = static_cast<SFUInt64>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                            std::chrono::steady_clock::now().time_since_epoch())
                                            .count());
}
