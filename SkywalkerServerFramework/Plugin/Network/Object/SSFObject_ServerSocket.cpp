/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Object\SSFObject_ServerSocket.cpp
**作者: shyfan
**日期: 2023/09/06 20:16:10
**功能: 服务器套接字对象
*************************************************************************/

#include "SSFObject_ServerSocket.h"

SKYWALKER_SF_NAMESPACE_USING

void SSFObject_ServerSocket::Create(SSFObjectErrors &Errors, SSFNetworkSocketCreatorContext &Context)
{
    SSFObject_NetworkSocket::Create(Errors, Context);
}
