/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPlugin.h
**作者: shyfan
**日期: 2023/08/07 23:53:05
**功能: 插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__

#include <memory>

#include "Include\SSFIPluginManager.h"

#include "Core\Object\SSFObject.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFCPlugin
    : public SSFIPlugin,
      public SSFCObject
{
public:
    SSFCPlugin(SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager);
    virtual ~SSFCPlugin();

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

    /**
     * 释放
     */
    virtual void Release(SSFObjectErrors &Errors) override;

#pragma endregion Object Base Interface

private:
    /**
     * 安装
     */
    void Install(SSFModuleErrors &Errors);

    /**
     * 卸载
     */
    void Uninstall(SSFModuleErrors &Errors);

    /**
     * 加入模块
     */
    void AddModule(const std::string &ModuleName, SSF_PTR_MODULE Module);

    /**
     * 移除模块
     */
    void RemoveModule(const std::string &ModuleName);

protected:
    SKYWALKER_SF_PTR_PLUGIN_MANAGER PluginManager;
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
