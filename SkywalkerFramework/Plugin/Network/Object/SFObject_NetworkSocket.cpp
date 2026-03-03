/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Object\SFObject_NetworkSocket.cpp
**作者: shyfan
**日期: 2023/08/26 15:47:17
**功能: 网络套接字对象
*************************************************************************/

#include "SFObject_NetworkSocket.h"

SF_NAMESPACE_USING

SSFObject_NetworkSocket::SSFObject_NetworkSocket(SSFNetworkSocketCreatorContext &InContext, SFObjectErrors &InErrors)
    : SSFObject(InContext, InErrors)
{
    Socket = InContext.Socket;

    Init(InErrors);
    Awake(InErrors);
    Start(InErrors);
}

SSFObject_NetworkSocket::~SSFObject_NetworkSocket()
{
}
