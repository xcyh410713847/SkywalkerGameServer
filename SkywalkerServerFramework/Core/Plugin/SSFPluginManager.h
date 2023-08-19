/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPluginManager.h
**作者: shyfan
**日期: 2023/08/07 23:53:25
**功能: 插件管理器
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__

#include "SkywalkerSingleton/SkywalkerSingleton.h"
#include "SkywalkerDerived/SkywalkerDerived.h"

#include "Include/SSFCore.h"

#include "Core/Object/SSFObject.h"
#include "Core/Map/SSFMap.h"
#include "Core/DynamicLib/SSFDynamicLib.h"

SKYWALKER_SF_NAMESPACE_BEGIN

/**
 * 插件管理器
 */
class SSFCPluginManager
    : public SSFCObject
{
    SKYWALKER_SINGLETON_DECLARE(SSFCPluginManager);

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
    /**
     * 注册插件
     * @param Plugin 插件
     */
    virtual void RegisterPlugin(SSFPluginErrors &Errors, SKYWALKER_SF_PTR_PLUGIN Plugin);

    /**
     * 注销插件
     * @param Plugin 插件
     */
    virtual void UnregisterPlugin(SSFPluginErrors &Errors, SKYWALKER_SF_PTR_PLUGIN Plugin);

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件
     */
    virtual SKYWALKER_SF_PTR_PLUGIN GetPlugin(const std::string &PluginName);

    /**
     * 注册模块
     * @param Module 模块
     */
    virtual void RegisterModule(SSFModuleErrors &Errors, SKYWALKER_SF_PTR_PLUGIN Plugin, SKYWALKER_SF_PTR_MODULE Module);

    /**
     * 注销模块
     * @param Module 模块
     */
    virtual void UnregisterModule(SSFModuleErrors &Errors, SKYWALKER_SF_PTR_PLUGIN Plugin, SKYWALKER_SF_PTR_MODULE Module);

    /**
     * 获取模块
     * @param ModuleName 模块名称
     * @return 模块
     */
    virtual SKYWALKER_SF_PTR_MODULE GetModule(const std::string &ModuleName);

private:
    /**
     * 加载插件配置
     */
    void LoadPluginConfig(SSFPluginErrors &Errors);

    /**
     * 加载库
     */
    void LoadPlugin(SSFPluginErrors &Errors);

    /**
     * 加载插件库
     */
    void LoadPluginLib(SSFPluginErrors &Errors, const std::string &PluginName);

private:
    /**
     * 插件
     */
    typedef void (*DLL_START_PLUGIN_FUNC)(SKYWALKER_SF_PTR(SSFCPluginManager));
    typedef void (*DLL_STOP_PLUGIN_FUNC)(SKYWALKER_SF_PTR(SSFCPluginManager));

    typedef SSFMap<std::string, bool> TMap_PluginName;
    typedef SSFMap<std::string, SKYWALKER_SF_PTR_DYNAMIC_LIB> TMap_DynamicLib;
    typedef std::map<std::string, SKYWALKER_SF_PTR_PLUGIN> TMap_Plugin;

    TMap_PluginName PluginNameMap;
    TMap_DynamicLib DynamicLibMap;
    TMap_Plugin PluginMap;

    // 模块映射
    typedef SSFMap<std::string, std::string> TMap_ModuleToPlugin;
    TMap_ModuleToPlugin ModuleToPluginMap;
};

/**
 * SSFIPluginManager 指针
 */
#define SKYWALKER_SF_PTR_PLUGIN_MANAGER SKYWALKER_SF_PTR(SSFCPluginManager)

/**
 * 注册插件
 */
#define SKYWALKER_SF_REGISTER_PLUGIN(PluginManager, ClassName)                                \
    SSFPluginErrors ClassName##Errors;                                                        \
    PluginManager->RegisterPlugin(ClassName##Errors, new ClassName(PluginManager));           \
    if (ClassName##Errors.IsValid())                                                          \
    {                                                                                         \
        SKYWALKER_SF_ERROR_TRACE(ClassName##Errors, SkywalkerSFError_Plugin_Register_Failed); \
    }

/**
 * 注销插件
 */
#define SKYWALKER_SF_UNREGISTER_PLUGIN(PluginManager, ClassName)                                \
    SSFPluginErrors ClassName##Errors;                                                          \
    PluginManager->UnregisterPlugin(ClassName##Errors, PluginManager->GetPlugin((#ClassName))); \
    if (ClassName##Errors.IsValid())                                                            \
    {                                                                                           \
        SKYWALKER_SF_ERROR_TRACE(ClassName##Errors, SkywalkerSFError_Plugin_Unregister_Failed); \
    }

/**
 * 注册模块
 */
#define SKYWALKER_SF_CREATE_MODULE(PluginManager, Plugin, ClassName)                                                                 \
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(ClassName, SSFCModule));                                                                \
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(Plugin, SSFCPlugin));                                                                   \
    SSFModuleErrors ClassName##Errors;                                                                                               \
    PluginManager->RegisterModule(ClassName##Errors, Plugin, new ClassName(PluginManager));                                          \
    if (ClassName##Errors.IsValid())                                                                                                 \
    {                                                                                                                                \
        SKYWALKER_SF_ERROR_TRACE(ClassName##Errors, SkywalkerSFError_Module_Register_Failed, "PluginManager RegisterModule Failed"); \
    }                                                                                                                                \
    Plugin->AddModule((#ClassName), PluginManager->GetModule((#ClassName))) if (ClassName##Errors.IsValid())                         \
    {                                                                                                                                \
        SKYWALKER_SF_ERROR_TRACE(ClassName##Errors, SkywalkerSFError_Module_Register_Failed, "Plugin AddModule Failed");             \
    }

/**
 * 注销模块
 */
#define SKYWALKER_SF_DESTROY_MODULE(PluginManager, Plugin, ClassName)                                                                    \
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(ClassName, SSFCModule));                                                                    \
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(Plugin, SSFCPlugin));                                                                       \
    SSFModuleErrors ClassName##Errors;                                                                                                   \
    PluginManager->UnregisterModule(ClassName##Errors, Plugin, PluginManager->GetModule((#ClassName)));                                  \
    if (ClassName##Errors.IsValid())                                                                                                     \
    {                                                                                                                                    \
        SKYWALKER_SF_ERROR_TRACE(ClassName##Errors, SkywalkerSFError_Module_Unregister_Failed, "PluginManager UnregisterModule Failed"); \
    }                                                                                                                                    \
    Plugin->RemoveModule((#ClassName)) if (ClassName##Errors.IsValid())                                                                  \
    {                                                                                                                                    \
        SKYWALKER_SF_ERROR_TRACE(ClassName##Errors, SkywalkerSFError_Module_Unregister_Failed, "Plugin RemoveModule Failed");            \
    }

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
