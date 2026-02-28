/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Module\SSFModule_NetworkClient.h
**作者: shyfan
**日期: 2023/08/26 15:37:38
**功能: 网络客户端模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_CLIENT_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_CLIENT_H__

#include "Include/SSFCore.h"

#include "Core/Module/SSFModule.h"

#include "SSFObject_NetworkSocket.h"

SSF_NAMESPACE_BEGIN

/**
 * 作为客户端，连接其他服务器
 */
class SSFModule_NetworkClient : public SSFModule
{
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

#pragma endregion Object

public:
    SSFModule_NetworkClient(SSFModuleContext &InContext, SSFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SSFModule_NetworkClient() {};

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
    void StartNetworkClient(SSFObjectErrors &Errors);

    /**
     * 停止网络客户端
     */
    void StopNetworkClient(SSFObjectErrors &Errors);

    /**
     * 处理接收数据
     */
    void HandleReceive(SSFObjectErrors &Errors);

private:
    SSF_NETWORK_DATA;

    SSF_PRT_NETWORK_SOCKET ClientNetworkSocket = nullptr;
    bool bIsConnected = false;
    std::string ServerIP;
    int ServerPort = 0;
};

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_CLIENT_H__
