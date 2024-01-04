/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Module\SSFModule_NetworkServer.h
**作者: shyfan
**日期: 2023/08/26 15:37:10
**功能: 网络服务器模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_SERVER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_SERVER_H__

#include "Include/SSFCore.h"

#include "Core/Module/SSFModule.h"

#include "SSFObject_ServerSocket.h"
#include "SSFObject_ClientSocket.h"

SSF_NAMESPACE_BEGIN

class SSFModule_NetworkServer : public SSFOModule
{
    SSF_OBJECT_CLASS(SSFModule_NetworkServer)

#pragma region Object

public:
    /**
     * 初始化
     */
    virtual void Init(SSFObjectErrors &Errors) override;

    /**
     * 唤醒
     */
    virtual void Awake(SSFObjectErrors &Errors) override;

    /**
     * 开始
     */
    virtual void Start(SSFObjectErrors &Errors) override;

    /**
     * Tick
     */
    virtual void Tick(SSFObjectErrors &Errors, int DelayMS) override;

    /**
     * 结束
     */
    virtual void Stop(SSFObjectErrors &Errors) override;

    /**
     * 休眠
     */
    virtual void Sleep(SSFObjectErrors &Errors) override;

    /**
     * 销毁
     */
    virtual void Destroy(SSFObjectErrors &Errors) override;

    /**
     * 释放
     */
    virtual void Release(SSFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SSFModule_NetworkServer(SSF_PTR_PLUGIN_MANAGER InPluginManager)
        : SSFOModule(InPluginManager)
    {
    }
    virtual ~SSFModule_NetworkServer(){};

private:
    /**
     * 开启网络服务器
     */
    void StartNetworkServer(SSFNetworkErrors &Errors);

    /**
     * 创建网络客户端
     */
    void CreateNetworkClient(SSFNetworkErrors &Errors);

private:
    WSADATA wsaData;
    SSF_PRT_SERVER_SOCKET ServerNetworkSocket = nullptr;
    SSFMap<SOCKET, SSF_PRT_CLIENT_SOCKET> ClientNetworkSocketMap;
};

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_SERVER_H__
