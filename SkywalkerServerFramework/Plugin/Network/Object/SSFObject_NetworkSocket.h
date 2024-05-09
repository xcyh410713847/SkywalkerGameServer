/*************************************************************************
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

#if defined(SKYWALKER_PLATFORM_WINDOWS)
typedef SOCKET SSFSOCKET;
#define SSF_INVALID_SOCKET INVALID_SOCKET
#define SSF_SOCKET_ERROR SOCKET_ERROR
#else
typedef int SSFSOCKET;
#define SSF_INVALID_SOCKET -1
#define SSF_SOCKET_ERROR -1
#endif

struct SSFNetworkSocketCreatorContext : public SSFObjectContext
{
    SSFSOCKET Socket;

    SSFNetworkSocketCreatorContext()
        : SSFObjectContext(),
          Socket(SSF_INVALID_SOCKET)
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
    SSFSOCKET GetSocket() const
    {
        return Socket;
    }

    /**
     * 是否无效
     */
    bool IsSocketInvalid() const
    {
        return Socket == SSF_INVALID_SOCKET;
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
    SSFSOCKET Socket = SSF_INVALID_SOCKET;
};

SSF_NAMESPACE_END

#define SSF_PRT_NETWORK_SOCKET SSF_PTR(SSFObject_NetworkSocket)

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_NETWORK_SOCKET_H__
