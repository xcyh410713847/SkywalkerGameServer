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

SSF_NAMESPACE_BEGIN

struct SSFNetworkSocketCreatorContext : public SSFObjectContext
{
    SOCKET Socket;

    SSFNetworkSocketCreatorContext()
        : SSFObjectContext(),
          Socket(INVALID_SOCKET)
    {
    }
};

class SSFObject_NetworkSocket : public SSFObject
{
    SSF_OBJECT_CLASS(SSFObject_NetworkSocket)

public:
    SSFObject_NetworkSocket(SSFNetworkSocketCreatorContext &InContext, SSFObjectErrors &InErrors);
    virtual ~SSFObject_NetworkSocket();

    /**
     * 获取套接字
     */
    SOCKET GetSocket() const
    {
        return Socket;
    }

    /**
     * 是否无效
     */
    bool IsSocketInvalid() const
    {
        return Socket == INVALID_SOCKET;
    }

    /**
     * 是否有效
     */
    bool IsSocketValid() const
    {
        return !IsSocketInvalid();
    }

    virtual void Init(SSFObjectErrors &Errors){};
    virtual void Awake(SSFObjectErrors &Errors){};
    virtual void Start(SSFObjectErrors &Errors){};
    virtual void Tick(SSFObjectErrors &Errors, int DelayMS){};
    virtual void Stop(SSFObjectErrors &Errors){};

protected:
    /**
     * 套接字
     */
    SOCKET Socket = INVALID_SOCKET;
};

SSF_NAMESPACE_END

#define SSF_PRT_NETWORK_SOCKET SSF_PTR(SSFObject_NetworkSocket)

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_NETWORK_SOCKET_H__
