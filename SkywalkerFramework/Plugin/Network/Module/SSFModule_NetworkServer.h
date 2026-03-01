/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Module\SSFModule_NetworkServer.h
**作者: shyfan
**日期: 2023/08/26 15:37:10
**功能: 网络服务器模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_SERVER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_SERVER_H__

#include "Include/SFCore.h"

#include "Core/Module/SSFModule.h"

#include "SSFObject_ServerSocket.h"
#include "SSFObject_ClientSocket.h"

SF_NAMESPACE_BEGIN

/**
 * 作为服务器，接受客户端连接
 */
class SSFModule_NetworkServer : public SSFModule
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
    SSFModule_NetworkServer(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors), ServerPort(0)
    {
    }
    virtual ~SSFModule_NetworkServer() {};

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

private:
    SSF_NETWORK_DATA;

    SF_UNIQUE_PTR(SSFObject_ServerSocket)
    ServerNetworkSocket = nullptr;
    SFMap<SSFSOCKET, SSF_PRT_CLIENT_SOCKET> ClientNetworkSocketMap;

    SFString ServerIP;
    int ServerPort;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_SERVER_H__
