/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Object\SSFObject_ServerSocket.h
**作者: shyfan
**日期: 2023/09/06 20:15:28
**功能: 服务器套接字对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_OBJECT_SERVER_SOCKET_H__
#define __SKYWALKER_SERVER_FRAMEWORK_OBJECT_SERVER_SOCKET_H__

#include "Include/SSFCore.h"

#include "SSFObject_NetworkSocket.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFObject_ServerSocket : public SSFObject_NetworkSocket
{
};

SKYWALKER_SF_NAMESPACE_END

#define SSF_PRT_SERVER_SOCKET SKYWALKER_SF_PTR(SSFObject_ServerSocket)

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_SERVER_SOCKET_H__
