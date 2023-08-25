/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Private\SSFModule_TCP.h
**作者: shyfan
**日期: 2023/08/25 15:31:50
**功能: TCP链接模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_TCP_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_TCP_H__

#include <windows.h>

#include "Include/SSFCore.h"

#include "Core/Module/SSFModule.h"

#pragma comment(lib, "ws2_32.lib")

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFOModule_TCP : public SSFOModule
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

    /**
     * 释放
     */
    virtual void Release(SSFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SSFOModule_TCP(SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager)
        : SSFOModule(InPluginManager)
    {
    }
    virtual ~SSFOModule_TCP(){};

    /**
     * 获取模块名称
     * @return 模块名称
     */
    virtual const std::string GetName() override
    {
        return SKYWALKER_SF_CLASS_NAME(SSFOModule_TCP);
    };

private:
    WSADATA wsaData;
    SOCKET ServerSocket;
    SOCKET ClientSocket;
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_TCP_H__
