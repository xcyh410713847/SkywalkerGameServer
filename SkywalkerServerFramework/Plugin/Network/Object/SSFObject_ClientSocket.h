/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Object\SSFObject_ClientSocket.h
**作者: shyfan
**日期: 2023/09/06 20:16:16
**功能: 客户端套接字对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_OBJECT_CLIENT_SOCKET_H__
#define __SKYWALKER_SERVER_FRAMEWORK_OBJECT_CLIENT_SOCKET_H__

#include "Include/SSFCore.h"

#include "SSFObject_NetworkSocket.h"

SSF_NAMESPACE_BEGIN

class SSFObject_ClientSocket : public SSFObject_NetworkSocket
{
#pragma region Object

public:
    virtual void Tick(SSFObjectErrors &Errors, int DelayMS) override;

#pragma endregion Object
};

SSF_NAMESPACE_END

#define SSF_PRT_CLIENT_SOCKET SSF_PTR(SSFObject_ClientSocket)

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_CLIENT_SOCKET_H__
