/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPluginManager.h
**作者: shyfan
**日期: 2023/08/07 23:53:25
**功能: 插件管理器
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__

#include "SkywalkerDerived/SkywalkerDerived.h"

#include "SkywalkerScript/Include/SkywalkerScriptParse.h"

#include "Include/SSFCore.h"
#include "Include/SSFILog.h"

#include "Core/Object/SSFObject.h"
#include "Core/Map/SSFMap.h"
#include "Core/DynamicLib/SSFDynamicLib.h"
#include "Core/Module/SSFModuleManager.h"

SSF_NAMESPACE_BEGIN

/**
 * 插件管理器
 */
class SSFOPluginManager
    : public SSFOModuleManager
{
    SSF_OBJECT_CLASS(SSFOPluginManager)

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
    SSFOPluginManager() : SSFOModuleManager() {}
    virtual ~SSFOPluginManager() {}

    /**
     * 注册插件
     * @param Plugin 插件
     */
    virtual void RegisterPlugin(SSFPluginErrors &Errors, SSF_PTR_PLUGIN Plugin);

    /**
     * 注销插件
     * @param Plugin 插件
     */
    virtual void UnregisterPlugin(SSFPluginErrors &Errors, SSF_PTR_PLUGIN Plugin);

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件
     */
    virtual SSF_PTR_PLUGIN GetPlugin(const std::string &PluginName);

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
    typedef void (*DLL_START_PLUGIN_FUNC)(SSF_PTR(SSFOPluginManager));
    typedef void (*DLL_STOP_PLUGIN_FUNC)(SSF_PTR(SSFOPluginManager));

    typedef SSFMap<std::string, bool> TMap_PluginName;
    typedef SSFMap<std::string, SKYWALKER_SF_PTR_DYNAMIC_LIB> TMap_DynamicLib;
    typedef SSFMap<std::string, SSF_PTR_PLUGIN> TMap_Plugin;

    TMap_PluginName PluginNameMap;
    TMap_DynamicLib DynamicLibMap;
    TMap_Plugin PluginMap;

    SKYWALKER_PTR_SCRIPT_PARSE PluginScriptParse = nullptr;
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
#define SKYWALKER_SF_REGISTER_PLUGIN(PluginManager, PluginClass)                                                   \
    SSF_ASSERT(SKYWALKER_IS_DERIVED(PluginClass, SSFOPlugin));                                            \
    SSFPluginErrors PluginClass##Errors;                                                                           \
    SSF_PTR_PLUGIN Plugin = SSF_NEW_OBJECT(PluginClass, PluginManager);                                   \
    PluginManager->RegisterPlugin(PluginClass##Errors, Plugin);                                                    \
    if (PluginClass##Errors.IsValid())                                                                             \
    {                                                                                                              \
        auto FirstError = PluginClass##Errors.GetFirstError();                                                     \
        SKYWALKER_SF_LOG_INFO("Register Plugin [" << Plugin->GetName() << "] Failed " << FirstError.GetContent()); \
    }                                                                                                              \
    else                                                                                                           \
    {                                                                                                              \
        SKYWALKER_SF_LOG_INFO("Register Plugin [" << Plugin->GetName() << "] Success");                            \
    }

/**
 * 注销插件
 */
#define SKYWALKER_SF_UNREGISTER_PLUGIN(PluginManager, PluginClass)                                                   \
    SSF_ASSERT(SKYWALKER_IS_DERIVED(PluginClass, SSFOPlugin));                                              \
    SSFPluginErrors PluginClass##Errors;                                                                             \
    SSF_PTR_PLUGIN Plugin = PluginManager->GetPlugin(SSF_CLASS_NAME(PluginClass));                 \
    PluginManager->UnregisterPlugin(PluginClass##Errors, Plugin);                                                    \
    if (PluginClass##Errors.IsValid())                                                                               \
    {                                                                                                                \
        auto FirstError = PluginClass##Errors.GetFirstError();                                                       \
        SKYWALKER_SF_LOG_INFO("Unregister Plugin [" << Plugin->GetName() << "] Failed " << FirstError.GetContent()); \
    }                                                                                                                \
    else                                                                                                             \
    {                                                                                                                \
        SKYWALKER_SF_LOG_INFO("Unregister Plugin [" << Plugin->GetName() << "] Success");                            \
    }

/**
 * 注册模块
 */
#define SKYWALKER_SF_REGISTER_MODULE(PluginManager, PluginClass, ModuleClass)                                                 \
    SSF_ASSERT(SKYWALKER_IS_DERIVED(ModuleClass, SSFOModule));                                                       \
    SSF_ASSERT(SKYWALKER_IS_DERIVED(PluginClass, SSFOPlugin));                                                       \
                                                                                                                              \
    SSFModuleErrors ModuleClass##Errors;                                                                                      \
    SSF_PTR_MODULE ModuleClass##Module = SSF_NEW_OBJECT(ModuleClass, PluginManager);                                 \
    PluginManager->RegisterModule(ModuleClass##Errors, ModuleClass##Module);                                                  \
    if (ModuleClass##Errors.IsValid())                                                                                        \
    {                                                                                                                         \
        SKYWALKER_SF_ERROR_DESC(ModuleClass##Errors,                                                                          \
                                SkywalkerSFError_Module_Register_Failed,                                                      \
                                "PluginManager RegisterModule Failed");                                                       \
    }                                                                                                                         \
                                                                                                                              \
    PluginManager->GetPlugin(SSF_CLASS_NAME(PluginClass))->RegisterModule(ModuleClass##Errors, ModuleClass##Module); \
    if (ModuleClass##Errors.IsValid())                                                                                        \
    {                                                                                                                         \
        SKYWALKER_SF_ERROR_DESC(ModuleClass##Errors,                                                                          \
                                SkywalkerSFError_Module_Register_Failed,                                                      \
                                "Plugin RegisterModule Failed");                                                              \
    }                                                                                                                         \
    SKYWALKER_SF_LOG_INFO("Register Module [" << #ModuleClass << "] Success");                                                \
    ModuleClass##Module->Init(ModuleClass##Errors);

/**
 * 注销模块
 */
#define SKYWALKER_SF_UNREGISTER_MODULE(PluginManager, PluginClass, ModuleClass)                                                             \
    SSF_ASSERT(SKYWALKER_IS_DERIVED(ModuleClass, SSFOModule));                                                                     \
    SSF_ASSERT(SKYWALKER_IS_DERIVED(PluginClass, SSFOPlugin));                                                                     \
                                                                                                                                            \
    SSF_PTR_MODULE ModuleClass##Module = PluginManager->GetPlugin(SSF_CLASS_NAME(PluginClass))->GetModule<ModuleClass>(); \
    SSFModuleErrors ModuleClass##Errors;                                                                                                    \
    PluginManager->UnregisterModule(ModuleClass##Errors, ModuleClass##Module);                                                              \
    if (ModuleClass##Errors.IsValid())                                                                                                      \
    {                                                                                                                                       \
        SKYWALKER_SF_ERROR_DESC(ModuleClass##Errors,                                                                                        \
                                SkywalkerSFError_Module_Unregister_Failed,                                                                  \
                                "PluginManager UnregisterModule Failed");                                                                   \
    }                                                                                                                                       \
    PluginManager->GetPlugin(SSF_CLASS_NAME(PluginClass))->UnregisterModule(ModuleClass##Errors, ModuleClass##Module);             \
    if (ModuleClass##Errors.IsValid())                                                                                                      \
    {                                                                                                                                       \
        SKYWALKER_SF_ERROR_DESC(ModuleClass##Errors,                                                                                        \
                                SkywalkerSFError_Module_Unregister_Failed,                                                                  \
                                "Plugin UnregisterModule Failed");                                                                          \
    }                                                                                                                                       \
    SKYWALKER_SF_LOG_INFO("Unregister Module [" << #ModuleClass << "] Success");                                                            \
    ModuleClass##Module->Release(ModuleClass##Errors);

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
