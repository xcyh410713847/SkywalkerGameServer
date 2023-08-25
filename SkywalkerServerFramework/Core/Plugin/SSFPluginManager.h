/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPluginManager.h
**作者: shyfan
**日期: 2023/08/07 23:53:25
**功能: 插件管理器
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__

#include "SkywalkerDerived/SkywalkerDerived.h"

#include "Include/SSFCore.h"
#include "Include/SSFCreator.h"
#include "Include/SSFILog.h"

#include "Core/Object/SSFObject.h"
#include "Core/Map/SSFMap.h"
#include "Core/DynamicLib/SSFDynamicLib.h"

SKYWALKER_SF_NAMESPACE_BEGIN

/**
 * 插件管理器
 */
class SSFOPluginManager
    : public SSFObject
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
     * 加载插件
     */
    void LoadPlugin(SSFPluginErrors &Errors);

    /**
     * 启动插件
     */
    void StartPlugin(SSFPluginErrors &Errors);

private:
    /**
     * 插件
     */
    typedef void (*DLL_START_PLUGIN_FUNC)(SKYWALKER_SF_PTR(SSFOPluginManager));
    typedef void (*DLL_STOP_PLUGIN_FUNC)(SKYWALKER_SF_PTR(SSFOPluginManager));

    typedef SSFMap<std::string, bool> TMap_PluginName;
    typedef SSFMap<std::string, SKYWALKER_SF_PTR_DYNAMIC_LIB> TMap_DynamicLib;
    typedef SSFMap<std::string, SKYWALKER_SF_PTR_PLUGIN> TMap_Plugin;

    TMap_PluginName PluginNameMap;
    TMap_DynamicLib DynamicLibMap;
    TMap_Plugin PluginMap;

    // 模块映射
    typedef SSFMap<std::string, std::string> TMap_ModuleToPlugin;
    TMap_ModuleToPlugin ModuleToPluginMap;
};

/**
 * 注册库
 */
#define SKYWALKER_SF_REGISTER_LIBRARY(LibraryName)                                                     \
    SSFPluginErrors LibraryName##Errors;                                                               \
    if (DynamicLibMap.find(LibraryName) != DynamicLibMap.end())                                        \
    {                                                                                                  \
        SKYWALKER_SF_ERROR_TRACE(LibraryName##Errors, SkywalkerSFError_Plugin_Load_Repeated);          \
        return;                                                                                        \
    }                                                                                                  \
    SSFDynamicLib *DynamicLib = new SSFDynamicLib(LibraryName);                                        \
    if (DynamicLib == nullptr)                                                                         \
    {                                                                                                  \
        SKYWALKER_SF_ERROR_TRACE(LibraryName##Errors, SkywalkerSFError_Plugin_Load_DynamicLibNullptr); \
        return;                                                                                        \
    }                                                                                                  \
    DynamicLibMap.insert(std::make_pair(LibraryName, DynamicLib));                                     \
    if (!DynamicLib->Load())                                                                           \
    {                                                                                                  \
        SKYWALKER_SF_ERROR_TRACE(LibraryName##Errors, SkywalkerSFError_Plugin_Load_Failed);            \
        return;                                                                                        \
    }                                                                                                  \
    SKYWALKER_SF_LOG_INFO("Register Library [" << LibraryName << "] Success");

/**
 * 注册插件
 */
#define SKYWALKER_SF_REGISTER_PLUGIN(PluginManager, PluginName)                                                    \
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(PluginName, SSFOPlugin));                                             \
    SSFPluginErrors PluginName##Errors;                                                                            \
    SKYWALKER_SF_PTR_PLUGIN Plugin = NewObject<PluginName>(PluginManager);                                         \
    PluginManager->RegisterPlugin(PluginName##Errors, Plugin);                                                     \
    if (PluginName##Errors.IsValid())                                                                              \
    {                                                                                                              \
        auto FirstError = PluginName##Errors.GetFirstError();                                                      \
        SKYWALKER_SF_LOG_INFO("Register Plugin [" << Plugin->GetName() << "] Failed " << FirstError.GetContent()); \
    }                                                                                                              \
    else                                                                                                           \
    {                                                                                                              \
        SKYWALKER_SF_LOG_INFO("Register Plugin [" << Plugin->GetName() << "] Success");                            \
    }

/**
 * 注销插件
 */
#define SKYWALKER_SF_UNREGISTER_PLUGIN(PluginManager, PluginName)                                                    \
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(PluginName, SSFOPlugin));                                               \
    SSFPluginErrors PluginName##Errors;                                                                              \
    SKYWALKER_SF_PTR_PLUGIN Plugin = PluginManager->GetPlugin((#PluginName));                                        \
    PluginManager->UnregisterPlugin(PluginName##Errors, Plugin);                                                     \
    if (PluginName##Errors.IsValid())                                                                                \
    {                                                                                                                \
        auto FirstError = PluginName##Errors.GetFirstError();                                                        \
        SKYWALKER_SF_LOG_INFO("Unregister Plugin [" << Plugin->GetName() << "] Failed " << FirstError.GetContent()); \
    }                                                                                                                \
    else                                                                                                             \
    {                                                                                                                \
        SKYWALKER_SF_LOG_INFO("Unregister Plugin [" << Plugin->GetName() << "] Success");                            \
    }

/**
 * 注册模块
 */
#define SKYWALKER_SF_REGISTER_MODULE(PluginManager, PluginName, ModuleName)   \
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(ModuleName, SSFOModule));        \
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(PluginName, SSFOPlugin));        \
                                                                              \
    SKYWALKER_SF_PTR_PLUGIN Plugin = PluginManager->GetPlugin((#PluginName)); \
    SSFModuleErrors ModuleName##Errors;                                       \
    SKYWALKER_SF_PTR_MODULE Module = NewObject<ModuleName>(PluginManager);    \
    PluginManager->RegisterModule(ModuleName##Errors, Plugin, Module);        \
    if (ModuleName##Errors.IsValid())                                         \
    {                                                                         \
        SKYWALKER_SF_ERROR_DESC(ModuleName##Errors,                           \
                                SkywalkerSFError_Module_Register_Failed,      \
                                "PluginManager RegisterModule Failed");       \
    }                                                                         \
    Plugin->RegisterModule(ModuleName##Errors, Module);                       \
    if (ModuleName##Errors.IsValid())                                         \
    {                                                                         \
        SKYWALKER_SF_ERROR_DESC(ModuleName##Errors,                           \
                                SkywalkerSFError_Module_Register_Failed,      \
                                "Plugin RegisterModule Failed");              \
    }                                                                         \
    SKYWALKER_SF_LOG_INFO("Register Module [" << #ModuleName << "] Success"); \
    Module->Init(ModuleName##Errors);

/**
 * 注销模块
 */
#define SKYWALKER_SF_UNREGISTER_MODULE(PluginManager, PluginName, ModuleName)   \
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(ModuleName, SSFOModule));          \
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(PluginName, SSFOPlugin));          \
                                                                                \
    SKYWALKER_SF_PTR_PLUGIN Plugin = PluginManager->GetPlugin((#PluginName));   \
    SKYWALKER_SF_PTR_MODULE Module = Plugin->GetModule(#ModuleName);            \
    SSFModuleErrors ModuleName##Errors;                                         \
    PluginManager->UnregisterModule(ModuleName##Errors, Plugin, Module);        \
    if (ModuleName##Errors.IsValid())                                           \
    {                                                                           \
        SKYWALKER_SF_ERROR_DESC(ModuleName##Errors,                             \
                                SkywalkerSFError_Module_Unregister_Failed,      \
                                "PluginManager UnregisterModule Failed");       \
    }                                                                           \
    Plugin->UnregisterModule(ModuleName##Errors, Module);                       \
    if (ModuleName##Errors.IsValid())                                           \
    {                                                                           \
        SKYWALKER_SF_ERROR_DESC(ModuleName##Errors,                             \
                                SkywalkerSFError_Module_Unregister_Failed,      \
                                "Plugin UnregisterModule Failed");              \
    }                                                                           \
    SKYWALKER_SF_LOG_INFO("Unregister Module [" << #ModuleName << "] Success"); \
    Module->Release(ModuleName##Errors);

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
