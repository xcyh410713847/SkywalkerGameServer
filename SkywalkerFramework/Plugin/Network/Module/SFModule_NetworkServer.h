/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Module\SFModule_NetworkServer.h
**作者: shyfan
**日期: 2023/08/26 15:37:10
**功能: 网络服务器模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_SERVER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_SERVER_H__

#include "Include/SFCore.h"

#include "Core/Module/SFModule.h"

#include "SFObject_ServerSocket.h"
#include "SFObject_ClientSocket.h"
#include "../Protocol/SFNetworkPacket.h"

#include <functional>
#include <filesystem>

SF_NAMESPACE_BEGIN

/**
 * 作为服务器，接受客户端连接
 */
class SFModule_NetworkServer : public SSFModule
{
#pragma region Object

public:
    /**
     * 初始化
     */
    virtual void Init(SFObjectErrors &Errors) override;

    /**
     * 唤醒
     */
    virtual void Awake(SFObjectErrors &Errors) override;

    /**
     * 开始
     */
    virtual void Start(SFObjectErrors &Errors) override;

    /**
     * Tick
     */
    virtual void Tick(SFObjectErrors &Errors, SFUInt64 DelayMS) override;

    /**
     * 结束
     */
    virtual void Stop(SFObjectErrors &Errors) override;

    /**
     * 休眠
     */
    virtual void Sleep(SFObjectErrors &Errors) override;

    /**
     * 销毁
     */
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SFModule_NetworkServer(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors), ServerPort(0)
    {
    }
    virtual ~SFModule_NetworkServer() {};

private:
    /**
     * 开启网络服务器
     */
    void StartNetworkServer(SFObjectErrors &Errors);

    /**
     * 停止网络服务器
     */
    void StopNetworkServer(SFObjectErrors &Errors);

    /**
     * 创建网络客户端
     */
    void CreateNetworkClient(SFObjectErrors &Errors);

    /**
     * 关闭并清理连接
     */
    void CleanupClientSocket(SSFSOCKET Socket, SFObjectErrors &Errors);

    /**
     * 注册服务端消息路由
     */
    void RegisterRouteHandlers();

    /**
     * 派发客户端消息
     */
    void DispatchClientPackets(SFObjectErrors &Errors);

    void HandleHeartbeat(SSFSOCKET Socket, const SSFNetworkPacket &Packet);
    void HandleLogin(SSFSOCKET Socket, const SSFNetworkPacket &Packet);
    void HandlePlayerInput(SSFSOCKET Socket, const SSFNetworkPacket &Packet);

    bool SendToClient(SSFSOCKET Socket, const SSFNetworkPacket &Packet);

private:
    SSF_NETWORK_DATA;

    SF_UNIQUE_PTR(SSFObject_ServerSocket)
    ServerNetworkSocket = nullptr;
    SFMap<SSFSOCKET, SSF_PRT_CLIENT_SOCKET> ClientNetworkSocketMap;
    SFMap<SFUInt16, std::function<void(SSFSOCKET, const SSFNetworkPacket &)>> RouteHandlerMap;

    SFString ServerIP;
    int ServerPort;
    SFUInt32 MaxMsgPerSecond = 120;
    SFUInt64 StatsLogIntervalMS = 5000;
    SFUInt64 LastStatsLogMS = 0;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_SERVER_H__
