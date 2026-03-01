/*************************************************************************
**文件: SkywalkerFramework\Core\Plugin\SSFPlugin.h
**作者: shyfan
**日期: 2023/08/07 23:53:05
**功能: 插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__

#include "Include/SFCore.h"

#include "Core/Module/SSFModule.h"
#include "Core/Object/SSFObjectManager.h"

SF_NAMESPACE_BEGIN

struct SFPluginContext : public SSFObjectContext
{
    SF_PTR(SFPluginManager)
    PluginManager{};
};

class SFPlugin : public SSFObjectManager<SSFModule>
{
public:
    SFPlugin(SFPluginContext &InContext, SFObjectErrors &InErrors);
    virtual ~SFPlugin();

    /**
     * 获取插件管理器
     */
    SF_PTR(SFPluginManager)
    GetPluginManager()
    {
        return PluginManager;
    };

    /**
     * 释放
     */
    virtual void Release(SFObjectErrors &Errors) override;

#pragma region Plugin Process

public:
    /**
     * 初始化
     */
    virtual void Init(SFObjectErrors &Errors);

    /**
     * 唤醒
     */
    virtual void Awake(SFObjectErrors &Errors);

    /**
     * 开始
     */
    virtual void Start(SFObjectErrors &Errors);

    /**
     * Tick
     */
    virtual void Tick(SFObjectErrors &Errors, int DelayMS);

    /**
     * 结束
     */
    virtual void Stop(SFObjectErrors &Errors);

    /**
     * 休眠
     */
    virtual void Sleep(SFObjectErrors &Errors);

    /**
     * 销毁
     */
    virtual void Destroy(SFObjectErrors &Errors);

#pragma endregion Plugin Process

#pragma region Module

public:
    /**
     * 设置配置加载的模块列表
     * @param ModuleNames 模块名列表，为空表示加载所有模块
     */
    void SetConfigModules(const SFMap<SFString, bool> &ModuleNames);

    /**
     * 注册模块
     * @param Module 模块
     */
    virtual void RegisterModule(SFObjectErrors &Errors, SF_PTR(SSFModule) Module);

    /**
     * 注销模块
     * @param Module 模块
     */
    virtual void UnregisterModule(SFObjectErrors &Errors, SF_PTR(SSFModule) Module);

    /**
     * 获取模块
     */
    inline SF_PTR(SSFModule) GetModule(const SFString &ModuleName)
    {
        auto Iter = ModuleMap.find(ModuleName);
        if (Iter == ModuleMap.end())
        {
            return nullptr;
        }

        auto IterObject = FindObject(Iter->second);
        if (SF_PTR_INVALID(IterObject))
        {
            return nullptr;
        }

        return SF_PTR_DYNAMIC_CAST(SSFModule)(IterObject);
    }

    /**
     * 获取模块
     * @return 模块
     */
    template <typename ModuleT>
    SF_PTR(ModuleT)
    GetModule()
    {
        SF_ASSERT_IS_BASE_OF(SSFModule, ModuleT);

        SFString ModuleName{};
        SF_CLASS_NAME(ModuleT, ModuleName);
        return SF_PTR_DYNAMIC_CAST(ModuleT)(GetModule(ModuleName));
    }

#pragma endregion Module

private:
    /**
     * 安装
     */
    virtual void Install(SFObjectErrors &Errors) {};

    /**
     * 卸载
     */
    virtual void Uninstall(SFObjectErrors &Errors) {};

protected:
    SF_PTR(SFPluginManager)
    PluginManager;

    SFMap<SFString, SFObjectGUID> ModuleMap;
    SFMap<SFString, bool> ConfigModules;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
