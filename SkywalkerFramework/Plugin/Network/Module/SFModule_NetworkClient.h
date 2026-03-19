/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Module\SFModule_NetworkClient.h
**作者: shyfan
**日期: 2023/08/26 15:37:38
**功能: 网络客户端模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_CLIENT_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_CLIENT_H__

#include "Include/SFCore.h"
#include "Include/SFNetworkInterface.h"

#include "Core/Module/SFModule.h"

#include "SFObject_NetworkSocket.h"
#include "SFNetworkBuffer.h"
#include "SFMessageCodec.h"
#include "SFMessageDispatcher.h"

#include <chrono>

SF_NAMESPACE_BEGIN

/**
 * 网络客户端模块
 *
 * 职责：
 * 1. 连接服务器
 * 2. 消息帧编码发送
 * 3. 消息帧接收解码分发
 * 4. 心跳发送
 */
class SFModule_NetworkClient : public SSFModule
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
    SFModule_NetworkClient(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors), ServerPort(0),
          HeartbeatIntervalMS(5000), LastHeartbeatMS(0),
          LoginPlayerId(0), bLoggedIn(false), bLoginSent(false)
    {
    }
    virtual ~SFModule_NetworkClient() {};

    /** 连接到服务器 */
    bool Connect(const char *IP, int Port);

    /** 断开连接 */
    void Disconnect();

    /** 是否已连接 */
    bool IsConnected() const;

    /**
     * 发送协议消息
     * @return true=成功写入发送缓冲区
     */
    bool SendMsg(SFMsgID MsgID, const char *Payload, SFUInt32 PayloadLen);

    /** 获取消息分发器（供注册接收处理器） */
    SFMessageDispatcher &GetDispatcher() { return Dispatcher; }

    /** 发送原始数据（兼容旧接口） */
    int Send(const char *Data, int Length);

private:
    void StartNetworkClient(SFObjectErrors &InErrors);
    void StopNetworkClient(SFObjectErrors &InErrors);

    /** 接收数据 + 解码 + 分发 */
    void ProcessRecv();

    /** 刷新发送缓冲区 */
    void FlushSend();

    /** 发送心跳 */
    void SendHeartbeat();

    /** 连接后发送登录请求 */
    void SendLoginReq();

private:
    SSF_NETWORK_DATA;

    SSF_PRT_NETWORK_SOCKET ClientNetworkSocket = nullptr;
    bool bIsConnected = false;
    SFString ServerIP;
    int ServerPort;

    SFNetworkBuffer RecvBuffer;
    SFNetworkBuffer SendBuffer;
    SFMessageDispatcher Dispatcher;

    SFUInt64 HeartbeatIntervalMS;
    SFUInt64 LastHeartbeatMS;

    /** 登录相关 */
    SFUInt32 LoginPlayerId;
    SFString LoginToken;
    bool bLoggedIn;
    bool bLoginSent;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_CLIENT_H__ */
