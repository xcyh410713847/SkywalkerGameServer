/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPlugin.h
**作者: shyfan
**日期: 2023/08/07 23:53:05
**功能: 插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__

#include "Include/SSFCore.h"

#include "Core/Module/SSFModule.h"
#include "Core/Object/SSFObjectManager.h"

SSF_NAMESPACE_BEGIN

struct SSFPluginContext : public SSFObjectContext
{
    SSF_PTR(SSFPluginManager)
    PluginManager{};
};

class SSFPlugin : public SSFObjectManager<SSFModule>
{
    SSF_OBJECT_CLASS(SSFPlugin)

public:
    SSFPlugin(SSFPluginContext &InContext, SSFObjectErrors &InErrors);
    virtual ~SSFPlugin();

    /**
     * 获取插件管理器
     */
    SSF_PTR(SSFPluginManager)
    GetPluginManager()
    {
        return PluginManager;
    };

    /**
     * 释放
     */
    virtual void Release(SSFObjectErrors &Errors) override;

#pragma region Plugin Process

public:
    /**
     * 初始化
     */
    virtual void Init(SSFObjectErrors &Errors);

    /**
     * 唤醒
     */
    virtual void Awake(SSFObjectErrors &Errors);

    /**
     * 开始
     */
    virtual void Start(SSFObjectErrors &Errors);

    /**
     * Tick
     */
    virtual void Tick(SSFObjectErrors &Errors, int DelayMS);

    /**
     * 结束
     */
    virtual void Stop(SSFObjectErrors &Errors);

    /**
     * 休眠
     */
    virtual void Sleep(SSFObjectErrors &Errors);

    /**
     * 销毁
     */
    virtual void Destroy(SSFObjectErrors &Errors);

#pragma endregion Plugin Process

#pragma region Module

public:
    /**
     * 注册模块
     * @param Module 模块
     */
    virtual void RegisterModule(SSFObjectErrors &Errors, SSF_PTR(SSFModule) Module);

    /**
     * 注销模块
     * @param Module 模块
     */
    virtual void UnregisterModule(SSFObjectErrors &Errors, SSF_PTR(SSFModule) Module);

    /**
     * 获取模块
     */
    inline SSF_PTR(SSFModule) GetModule(const SSFString &ModuleName)
    {
        auto Iter = ModuleMap.find(ModuleName);
        if (Iter == ModuleMap.end())
        {
            return nullptr;
        }

        auto IterObject = FindObject(Iter->second);
        if (SSF_PTR_INVALID(IterObject))
        {
            return nullptr;
        }

        return SSF_PTR_DYNAMIC_CAST(SSFModule)(IterObject);
    }

    /**
     * 获取模块
     * @return 模块
     */
    template <typename ModuleT>
    SSF_PTR(ModuleT)
    GetModule()
    {
        SSF_ASSERT_IS_BASE_OF(SSFModule, ModuleT);

        SSFString ModuleName{};
        SSF_CLASS_NAME(ModuleT, ModuleName);
        return SSF_PTR_DYNAMIC_CAST(ModuleT)(GetModule(ModuleName));
    }

#pragma endregion Module

private:
    /**
     * 安装
     */
    virtual void Install(SSFObjectErrors &Errors){};

    /**
     * 卸载
     */
    virtual void Uninstall(SSFObjectErrors &Errors){};

protected:
    SSF_PTR(SSFPluginManager)
    PluginManager;

    SSFMap<SSFString, SSFObjectGUID> ModuleMap;
};

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
