/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Object\SFObject_ServerSocket.h
**作者: shyfan
**日期: 2023/09/06 20:15:28
**功能: 服务器套接字对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_OBJECT_SERVER_SOCKET_H__
#define __SKYWALKER_SERVER_FRAMEWORK_OBJECT_SERVER_SOCKET_H__

#include "Include/SFCore.h"

#include "SFObject_NetworkSocket.h"

SF_NAMESPACE_BEGIN

class SSFObject_ServerSocket : public SSFObject_NetworkSocket
{
public:
    SSFObject_ServerSocket(SSFNetworkSocketCreatorContext &InContext, SFObjectErrors &InErrors);
    virtual ~SSFObject_ServerSocket();

    virtual void Stop(SFObjectErrors &Errors) override;
};

SF_NAMESPACE_END

#define SSF_PRT_SERVER_SOCKET SF_PTR(SSFObject_ServerSocket)

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_SERVER_SOCKET_H__
