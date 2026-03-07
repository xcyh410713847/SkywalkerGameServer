/*************************************************************************
**文件: SkywalkerFramework\Core\Plugin\SFPlugin.h
**作者: shyfan
**日期: 2023/08/07 23:53:05
**功能: 插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__

#include "Include/SFCore.h"

#include "Core/Module/SFModule.h"
#include "Core/Object/SFObjectManager.h"

SF_NAMESPACE_BEGIN

/**
 * 插件创建上下文
 * 说明：用于注入插件管理器指针。
 */
struct SSFPluginContext : public SSFObjectContext
{
    /** 所属插件管理器 */
    SF_PTR(SFPluginManager)
    PluginManager{};
};

using SFPluginContext = SSFPluginContext;

/**
 * 插件基类
 *
 * 职责：
 * 1. 管理插件生命周期（Init/Awake/Start/Tick/Stop/Sleep/Destroy）
 * 2. 管理模块注册、卸载与查找
 * 3. 支持根据配置筛选模块加载
 */
class SFPlugin : public SSFObjectManager<SSFModule>
{
public:
    /** 构造函数 */
    SFPlugin(SSFPluginContext &InContext, SFObjectErrors &InErrors);
    /** 析构函数 */
    virtual ~SFPlugin();

    /**
     * 获取插件管理器
     * @return 插件管理器指针
     */
    SF_PTR(SFPluginManager)
    GetPluginManager()
    {
        return PluginManager;
    };

    /**
     * 释放插件
     * 说明：会先执行 Uninstall，再释放基类资源。
     */
    virtual void Release(SFObjectErrors &Errors) override;

#pragma region Plugin Process

public:
    /**
     * 初始化
     * 说明：默认会调用 Install 加载模块并发送插件初始化事件。
     */
    virtual void Init(SFObjectErrors &Errors);

    /**
     * 唤醒
     * 说明：分发给所有已注册模块。
     */
    virtual void Awake(SFObjectErrors &Errors);

    /**
     * 开始
     * 说明：分发给所有已注册模块。
     */
    virtual void Start(SFObjectErrors &Errors);

    /**
     * 每帧更新
     * @param DelayMS 帧间隔（毫秒）
     */
    virtual void Tick(SFObjectErrors &Errors, SFUInt64 DelayMS);

    /**
     * 结束
     * 说明：分发给所有已注册模块。
     */
    virtual void Stop(SFObjectErrors &Errors);

    /**
     * 休眠
     * 说明：分发给所有已注册模块。
     */
    virtual void Sleep(SFObjectErrors &Errors);

    /**
     * 销毁
     * 说明：分发给所有已注册模块。
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
     * @param Errors 错误输出对象
     * @param Module 待注册模块
     */
    virtual void RegisterModule(SFObjectErrors &Errors, SF_PTR(SSFModule) Module);

    /**
     * 注销模块
     * @param Errors 错误输出对象
     * @param Module 待注销模块
     */
    virtual void UnregisterModule(SFObjectErrors &Errors, SF_PTR(SSFModule) Module);

    /**
     * 按名称获取模块
     * @param ModuleName 模块名称
     * @return 模块指针；未找到返回 nullptr
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
     * 按类型获取模块
     * @tparam ModuleT 模块类型（必须继承自 SSFModule）
     * @return 模块指针；未找到或类型不匹配返回 nullptr
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
     * 安装阶段（可由子类重写）
     * 说明：通常在此注册模块。
     */
    virtual void Install(SFObjectErrors &Errors) {};

    /**
     * 卸载阶段（可由子类重写）
     * 说明：通常在此注销模块并清理资源。
     */
    virtual void Uninstall(SFObjectErrors &Errors) {};

protected:
    /** 所属插件管理器 */
    SF_PTR(SFPluginManager)
    PluginManager;

    /** 模块索引表（Key: 模块类名，Value: 模块对象 GUID） */
    SFMap<SFString, SFObjectGUID> ModuleMap;
    /** 配置允许加载的模块集合（为空表示不过滤） */
    SFMap<SFString, bool> ConfigModules;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
