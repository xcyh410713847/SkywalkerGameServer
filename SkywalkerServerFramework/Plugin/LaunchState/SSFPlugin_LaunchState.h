/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\LaunchState\SSFPlugin_LaunchState.h
**作者: shyfan
**日期: 2023/07/30 12:38:38
**功能: 启动状态插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_LAUNCH_STATE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_LAUNCH_STATE_H__

#include "Core\Plugin\SSFPlugin.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFCPlugin_LaunchState : public SSFCPlugin
{
public:
    SSFCPlugin_LaunchState(SSFPtr_IPluginManager InPluginManager);
    virtual ~SSFCPlugin_LaunchState();

#pragma region Object Base Interface

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

#pragma endregion Object Base Interface

#pragma region SSFIPlugin
public:
    /**
     * 获取插件名称
     * @return 插件名称
     */
    virtual const std::string GetPluginName() override;

#pragma endregion SSFIPlugin
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_LAUNCH_STATE_H__
