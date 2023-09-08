/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Object\SSFObject_ClientSocket.cpp
**作者: shyfan
**日期: 2023/09/06 20:16:33
**功能: 客户端套接字对象
*************************************************************************/

#include "SSFObject_ClientSocket.h"

#include "Include/SSFILog.h"

SKYWALKER_SF_NAMESPACE_USING

SKYWALKER_SF_LOG_DEFINE(SSFObject_ClientSocket, LogLevel_Debug);

#pragma region Object

void SSFObject_ClientSocket::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SSFObject_NetworkSocket::Tick(Errors, DelayMS);

    SOCKET ClientSocket = GetSocket();

    // 与客户端进行数据通信
    char buffer[1024];
    int bytesReceived;

    // 接收客户端发送的数据
    bytesReceived = recv(ClientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0)
    {
        return;
    }

    // 处理接收到的数据
    SKYWALKER_SF_LOG_DEBUG("ClientSocket " << ClientSocket << " Received: " << buffer)
    // 回复客户端
    send(ClientSocket, buffer, bytesReceived, 0);
}

#pragma endregion Object
