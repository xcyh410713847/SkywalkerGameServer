/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Module\SFModule_NetworkServer.h
**作者: shyfan
**日期: 2023/08/26 15:37:10
**功能: 网络服务器模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_SERVER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_SERVER_H__

#include "Include/SFCore.h"
#include "Include/SFNetworkInterface.h"

#include "Core/Module/SFModule.h"

#include "SFObject_ServerSocket.h"
#include "SFObject_ClientSocket.h"
#include "SFSession.h"
#include "SFMessageCodec.h"
#include "SFMessageDispatcher.h"

#include <functional>
#include <filesystem>

SF_NAMESPACE_BEGIN

/**
 * 网络服务器模块
 *
 * 职责：
 * 1. TCP 监听与接受连接
 * 2. 会话(Session)生命周期管理
 * 3. 消息帧的接收、解码、分发
 * 4. 消息帧的编码、发送
 * 5. 心跳超时检测
 *
 * 实现 ISFNetworkServer 接口供其他插件跨插件调用。
 */
class SFModule_NetworkServer : public SSFModule, public ISFNetworkServer
{
#pragma region Object

public:
    virtual void Init(SFObjectErrors &Errors) override;
    virtual void Awake(SFObjectErrors &Errors) override;
    virtual void Start(SFObjectErrors &Errors) override;
    virtual void Tick(SFObjectErrors &Errors, SFUInt64 DelayMS) override;
    virtual void Stop(SFObjectErrors &Errors) override;
    virtual void Sleep(SFObjectErrors &Errors) override;
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SFModule_NetworkServer(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors), ServerPort(0), NextSessionId(1),
          SessionTimeoutMS(15000)
    {
    }
    virtual ~SFModule_NetworkServer() {};

#pragma region ISFNetworkServer Interface

    /** ISFNetworkServer::RegisterHandler */
    virtual bool RegisterHandler(SFMsgID MsgID, SFMessageHandler Handler) override;

    /** ISFNetworkServer::UnregisterHandler */
    virtual void UnregisterHandler(SFMsgID MsgID) override;

    /** ISFNetworkServer::SendTo */
    virtual bool SendTo(SFUInt32 SessionId, SFMsgID MsgID,
                        const char *Payload, SFUInt32 PayloadLen) override;

    /** ISFNetworkServer::Broadcast */
    virtual void Broadcast(SFMsgID MsgID,
                           const char *Payload, SFUInt32 PayloadLen) override;

    /** ISFNetworkServer::BroadcastTo */
    virtual void BroadcastTo(const std::vector<SFUInt32> &SessionIds,
                             SFMsgID MsgID,
                             const char *Payload, SFUInt32 PayloadLen) override;

    /** ISFNetworkServer::SetSessionAuthenticated */
    virtual void SetSessionAuthenticated(SFUInt32 SessionId, SFUInt32 PlayerId) override;

    /** ISFNetworkServer::IsSessionAuthenticated */
    virtual bool IsSessionAuthenticated(SFUInt32 SessionId) override;

    /** ISFNetworkServer::GetSessionPlayerId */
    virtual SFUInt32 GetSessionPlayerId(SFUInt32 SessionId) override;

    /** ISFNetworkServer::FindSessionByPlayerId */
    virtual SFUInt32 FindSessionByPlayerId(SFUInt32 PlayerId) override;

    /** ISFNetworkServer::CloseSession */
    virtual void CloseSession(SFUInt32 SessionId) override;

    /** ISFNetworkServer::DispatchLocal */
    virtual bool DispatchLocal(SFUInt32 SessionId, SFMsgID MsgID,
                               const char *Payload, SFUInt32 PayloadLen) override;

#pragma endregion ISFNetworkServer Interface

#pragma region Public API

    /**
     * 获取消息分发器（本插件内直接使用）
     */
    SFMessageDispatcher &GetDispatcher() { return Dispatcher; }

    /**
     * 根据 SessionId 获取 Session（本插件内直接使用）
     */
    SFSession *GetSession(SFUInt32 SessionId);

#pragma endregion Public API

private:
    /** 开启网络服务器 */
    void StartNetworkServer(SFObjectErrors &Errors);

    /** 停止网络服务器 */
    void StopNetworkServer(SFObjectErrors &Errors);

    /** 接受新连接，创建 Session */
    void AcceptNewConnections(SFObjectErrors &Errors);

    /** 处理所有 Session 的接收 */
    void ProcessSessionRecv(SFObjectErrors &Errors);

    /** 处理所有 Session 的发送 */
    void ProcessSessionSend(SFObjectErrors &Errors);

    /** 清理断开的 Session */
    void CleanupClosedSessions(SFObjectErrors &Errors);

    /** 心跳超时检测 */
    void CheckSessionTimeout();

    /** 内部清理一个 Session */
    void DestroySession(SFUInt32 SessionId, SFObjectErrors &Errors);

private:
    SSF_NETWORK_DATA;

    SF_UNIQUE_PTR(SSFObject_ServerSocket)
    ServerNetworkSocket = nullptr;

    /** SessionId → Session */
    SFMap<SFUInt32, SFSession *> SessionMap;
    /** Socket → SessionId (快速查找) */
    SFMap<SSFSOCKET, SFUInt32> SocketToSessionMap;

    /** 消息分发器 */
    SFMessageDispatcher Dispatcher;

    /** SessionId 自增计数器 */
    SFUInt32 NextSessionId;

    /** 配置 */
    SFString ServerIP;
    int ServerPort;
    SFUInt64 SessionTimeoutMS;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_SERVER_H__ */
