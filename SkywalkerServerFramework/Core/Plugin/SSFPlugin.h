/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPlugin.h
**作者: shyfan
**日期: 2023/08/07 23:53:05
**功能: 插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__

#include "Include/SSFCore.h"

#include "Core/Object/SSFObject.h"
#include "Core/Map/SSFMap.h"
#include "Core/Module/SSFModuleManager.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFOPlugin
    : public SSFOModuleManager
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
    SSFOPlugin(SSFObjectCreatorContext &Context, SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager);
    virtual ~SSFOPlugin();

    /**
     * 获取插件名称
     * @return 插件名称
     */
    virtual const std::string GetName()
    {
        return SKYWALKER_SF_CLASS_NAME(SSFOPlugin);
    };

private:
    /**
     * 安装
     */
    virtual void Install(SSFModuleErrors &Errors){};

    /**
     * 卸载
     */
    virtual void Uninstall(SSFModuleErrors &Errors){};

protected:
    SKYWALKER_SF_PTR_PLUGIN_MANAGER PluginManager;
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
