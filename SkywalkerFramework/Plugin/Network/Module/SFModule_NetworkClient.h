/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Module\SFModule_NetworkClient.h
**作者: shyfan
**日期: 2023/08/26 15:37:38
**功能: 网络客户端模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_CLIENT_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_CLIENT_H__

#include "Include/SFCore.h"

#include "Core/Module/SFModule.h"

#include "SFObject_NetworkSocket.h"

#include <chrono>

SF_NAMESPACE_BEGIN

/**
 * 作为客户端，连接其他服务器
 */
class SFModule_NetworkClient : public SSFModule
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
    SFModule_NetworkClient(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SFModule_NetworkClient() {};

    /**
     * 连接到服务器
     * @param IP 服务器IP
     * @param Port 服务器端口
     * @return 是否连接成功
     */
    bool Connect(const char *IP, int Port);

    /**
     * 断开连接
     */
    void Disconnect();

    /**
     * 是否已连接
     */
    bool IsConnected() const;

    /**
     * 发送数据
     * @param Data 数据
     * @param Length 数据长度
     * @return 发送的字节数
     */
    int Send(const char *Data, int Length);

private:
    /**
     * 开启网络客户端
     */
    void StartNetworkClient(SFObjectErrors &InErrors);

    /**
     * 停止网络客户端
     */
    void StopNetworkClient(SFObjectErrors &InErrors);

private:
    SSF_NETWORK_DATA;

    SSF_PRT_NETWORK_SOCKET ClientNetworkSocket = nullptr;
    bool bIsConnected = false;
    std::string ServerIP;
    int ServerPort = 0;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_CLIENT_H__
