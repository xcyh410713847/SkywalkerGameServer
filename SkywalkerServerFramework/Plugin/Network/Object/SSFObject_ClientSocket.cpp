﻿/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Object\SSFObject_ClientSocket.cpp
**作者: shyfan
**日期: 2023/09/06 20:16:33
**功能: 客户端套接字对象
*************************************************************************/

#include "SSFObject_ClientSocket.h"

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFObject_ClientSocket, LogLevel_Debug);

SSFObject_ClientSocket::SSFObject_ClientSocket(SSFNetworkSocketCreatorContext &InContext, SSFObjectErrors &InErrors)
    : SSFObject_NetworkSocket(InContext, InErrors)
{
}

SSFObject_ClientSocket::~SSFObject_ClientSocket()
{
}

void SSFObject_ClientSocket::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SSFObject_NetworkSocket::Tick(Errors, DelayMS);

    // 获取客户端套接字
    SSFSOCKET ClientSocket = GetSocket();

    // 检查客户端套接字是否有效
    if (IsSocketInvalid())
    {
        return;
    }

    // TODO Shyfan 检查客户端套接字连接状态

    // 与客户端进行数据通信
    char buffer[1024];
    int bytesReceived = SSF_SOCKET_READ(ClientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0)
    {
        return;
    }

    // 处理接收到的数据
    SSF_LOG_DEBUG("ClientSocket " << ClientSocket << " Received: " << buffer)

    // 回复客户端
    SSF_SOCKET_WRITE(ClientSocket, buffer, bytesReceived, 0);
}
