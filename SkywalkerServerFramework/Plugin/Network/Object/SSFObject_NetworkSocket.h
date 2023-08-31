﻿/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Object\SSFObject_NetworkSocket.h
**作者: shyfan
**日期: 2023/08/26 15:46:45
**功能: 网络套接字对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_OBJECT_NETWORK_SOCKET_H__
#define __SKYWALKER_SERVER_FRAMEWORK_OBJECT_NETWORK_SOCKET_H__

#include "Include/SSFCore.h"

#include "Core/Object/SSFObject.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFObject_NetworkSocket : public SSFObject
{
    SSF_OBJECT_CLASS(SSFObject_NetworkSocket)

public:
    SSFObject_NetworkSocket();
    virtual ~SSFObject_NetworkSocket();
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_NETWORK_SOCKET_H__