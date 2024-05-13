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

#pragma region 跨平台套接字

#if defined(SKYWALKER_PLATFORM_WINDOWS)

/**
 * 套接字类型
 */
typedef SOCKET SSFSOCKET;

/**
 * 无效套接字
 */
#define SSF_INVALID_SOCKET INVALID_SOCKET

/**
 * 套接字错误
 */
#define SSF_SOCKET_ERROR SOCKET_ERROR

/**
 * 关闭套接字
 */
#define SSF_CLOSE_SOCKET closesocket

/**
 * 清理网络
 */
#define SSF_NETWORK_CLEANUP() WSACleanup()

/**
 * 启动网络
 */
#define SSF_NETWORK_STARTUP()                                                                                    \
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)                                                           \
    {                                                                                                        \
        SSF_ERROR_DESC_TRACE(InErrors, SkywalkerSFError_Network_Init_Failed, "Failed to initialize winsock") \
        return;                                                                                              \
    }

/**
 * 网络数据
 */
#define SSF_NETWORK_DATA \
    WSADATA wsaData

/**
 * 创建套接字
 */
#define SSF_SOCKET_CREATE(AF, Type) socket(AF, Type, 0)

/**
 * 读取套接字
 */
#define SSF_SOCKET_READ(Socket, Buffer, BufferLength, Flags) recv(Socket, Buffer, BufferLength, Flags)

/**
 * 写入套接字
 */
#define SSF_SOCKET_WRITE(Socket, Buffer, BufferLength, Flags) send(Socket, Buffer, BufferLength, Flags)

/**
 * 设置套接字为非阻塞模式
 */
#define SSF_SOCKET_SET_NONBLOCKING(Socket)                                                                                         \
    {                                                                                                                              \
        u_long mode = 1;                                                                                                           \
        if (ioctlsocket(Socket, FIONBIO, &mode) == SSF_SOCKET_ERROR)                                                               \
        {                                                                                                                          \
            SSF_ERROR_DESC_TRACE(InErrors, SkywalkerSFError_Network_Socket_SetFailed, "Failed to set socket to non-blocking mode") \
            SSF_CLOSE_SOCKET(Socket);                                                                                              \
            return;                                                                                                                \
        }                                                                                                                          \
    }

#else

/**
 * 套接字类型
 */
typedef int SSFSOCKET;

/**
 * 无效套接字
 */
#define SSF_INVALID_SOCKET -1

/**
 * 套接字错误
 */
#define SSF_SOCKET_ERROR -1

/**
 * 关闭套接字
 */
#define SSF_CLOSE_SOCKET close

/**
 * 初始化网络
 */
#define SSF_NETWORK_CLEANUP()

/**
 * 启动网络
 */
#define SSF_NETWORK_STARTUP()

/**
 * 网络数据
 */
#define SSF_NETWORK_DATA

/**
 * 创建套接字
 */
#define SSF_SOCKET_CREATE(AF, Type) socket(AF, Type, IPPROTO_TCP)

/**
 * 读取套接字
 */
#define SSF_SOCKET_READ(Socket, Buffer, BufferLength, Flags) read(Socket, Buffer, BufferLength)

/**
 * 写入套接字
 */
#define SSF_SOCKET_WRITE(Socket, Buffer, BufferLength, Flags) write(Socket, Buffer, BufferLength)

/**
 * 设置套接字为非阻塞模式
 */
#define SSF_SOCKET_SET_NONBLOCKING(Socket)          \
    {                                               \
        int flags = fcntl(Socket, F_GETFL, 0);      \
        fcntl(Socket, F_SETFL, flags | O_NONBLOCK); \
    }

#endif

#pragma endregion 跨平台套接字

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

    virtual void Init(SSFObjectErrors &Errors) {};
    virtual void Awake(SSFObjectErrors &Errors) {};
    virtual void Start(SSFObjectErrors &Errors) {};
    virtual void Tick(SSFObjectErrors &Errors, int DelayMS) {};
    virtual void Stop(SSFObjectErrors &Errors) {};

protected:
    /**
     * 套接字
     */
    SSFSOCKET Socket = SSF_INVALID_SOCKET;
};

SSF_NAMESPACE_END

#define SSF_PRT_NETWORK_SOCKET SSF_PTR(SSFObject_NetworkSocket)

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_NETWORK_SOCKET_H__
