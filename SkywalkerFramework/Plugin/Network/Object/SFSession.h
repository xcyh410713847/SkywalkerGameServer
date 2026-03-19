/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Object\SFSession.h
**作者: shyfan
**日期: 2026/03/19
**功能: 网络会话对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SESSION_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SESSION_H__

#include "Include/SFCore.h"

#include "SFNetworkBuffer.h"
#include "SFObject_ClientSocket.h"

#include <chrono>

SF_NAMESPACE_BEGIN

/**
 * 会话状态
 */
enum class ESFSessionState : SFUInt8
{
    Connected = 0, /* 已连接，未登录 */
    Authenticated, /* 已登录 */
    Closing,       /* 正在关闭 */
};

/**
 * 网络会话
 *
 * 代表一个客户端连接的完整会话生命周期。
 * 持有 ClientSocket 引用、收发缓冲区、登录状态。
 */
class SFSession
{
public:
    SFSession(SFUInt32 InSessionId, SSF_PRT_CLIENT_SOCKET InSocket)
        : SessionId(InSessionId),
          ClientSocket(InSocket),
          State(ESFSessionState::Connected),
          PlayerId(0),
          LastActiveMS(GetSteadyNowMS())
    {
    }

    ~SFSession() = default;

    /** 获取会话ID */
    SFUInt32 GetSessionId() const { return SessionId; }

    /** 获取客户端Socket */
    SSF_PRT_CLIENT_SOCKET GetClientSocket() const { return ClientSocket; }

    /** 获取底层Socket句柄 */
    SSFSOCKET GetSocket() const
    {
        return (ClientSocket != nullptr) ? ClientSocket->GetSocket() : SSF_INVALID_SOCKET;
    }

    /** 获取会话状态 */
    ESFSessionState GetState() const { return State; }

    /** 设置为已认证状态 */
    void SetAuthenticated(SFUInt32 InPlayerId)
    {
        State = ESFSessionState::Authenticated;
        PlayerId = InPlayerId;
    }

    /** 是否已认证 */
    bool IsAuthenticated() const { return State == ESFSessionState::Authenticated; }

    /** 获取绑定的玩家ID */
    SFUInt32 GetPlayerId() const { return PlayerId; }

    /** 获取接收缓冲区 */
    SFNetworkBuffer &GetRecvBuffer() { return RecvBuffer; }

    /** 获取发送缓冲区 */
    SFNetworkBuffer &GetSendBuffer() { return SendBuffer; }

    /** 更新最后活跃时间 */
    void TouchActive() { LastActiveMS = GetSteadyNowMS(); }

    /** 获取最后活跃时间 */
    SFUInt64 GetLastActiveMS() const { return LastActiveMS; }

    /** 检查是否超时 */
    bool IsTimeout(SFUInt64 TimeoutMS) const
    {
        return (GetSteadyNowMS() - LastActiveMS) > TimeoutMS;
    }

    /** 标记为关闭状态 */
    void MarkClosing() { State = ESFSessionState::Closing; }

    /** 是否正在关闭 */
    bool IsClosing() const { return State == ESFSessionState::Closing; }

    /** 连接是否已断开 */
    bool IsClosed() const
    {
        return (ClientSocket == nullptr) || ClientSocket->IsClosed();
    }

private:
    static SFUInt64 GetSteadyNowMS()
    {
        return static_cast<SFUInt64>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now().time_since_epoch())
                .count());
    }

    SFUInt32 SessionId;
    SSF_PRT_CLIENT_SOCKET ClientSocket;
    ESFSessionState State;
    SFUInt32 PlayerId;
    SFUInt64 LastActiveMS;

    SFNetworkBuffer RecvBuffer;
    SFNetworkBuffer SendBuffer;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_SESSION_H__ */
