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

SSF_NAMESPACE_BEGIN

class SSFObject_ServerSocket : public SSFObject_NetworkSocket
{
#pragma region Object

public:
    virtual void Stop(SSFObjectErrors &Errors) override;

#pragma endregion Object

public:
    /**
     * 创建
     */
    virtual void Create(SSFObjectErrors &Errors, SSFNetworkSocketCreatorContext &Context) override;
};

SSF_NAMESPACE_END

#define SSF_PRT_SERVER_SOCKET SSF_PTR(SSFObject_ServerSocket)

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_SERVER_SOCKET_H__
