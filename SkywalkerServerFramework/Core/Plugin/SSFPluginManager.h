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

#include "Core/DynamicLib/SSFDynamicLib.h"
#include "Core/Plugin/SSFPlugin.h"

SSF_NAMESPACE_BEGIN

/**
 * 插件管理器
 */
class SSFPluginManager : public SSFObjectManager<SSFPlugin>
{
    SSF_OBJECT_CLASS(SSFPluginManager)

public:
    SSFPluginManager(SSFObjectContext &InContext, SSFObjectErrors &InErrors);
    virtual ~SSFPluginManager();

    /**
     * 释放
     */
    virtual void Release(SSFObjectErrors &Errors) override;

#pragma region Process

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

#pragma endregion Process

#pragma region Plugin

public:
    /**
     * 注册插件
     * @param Plugin 插件
     */
    virtual void RegisterPlugin(SSFObjectErrors &Errors, SSF_PTR_PLUGIN Plugin);

    /**
     * 注销插件
     * @param Plugin 插件
     */
    virtual void UnregisterPlugin(SSFObjectErrors &Errors, SSF_PTR_PLUGIN Plugin);

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件
     */
    inline SSF_PTR_PLUGIN GetPlugin(const std::string &PluginName)
    {
        auto Iter = PluginMap.find(PluginName);
        if (Iter == PluginMap.end())
        {
            return nullptr;
        }

        auto IterObject = FindObject(Iter->second);
        if (SSF_PTR_INVALID(IterObject))
        {
            return nullptr;
        }

        return SSF_PTR_DYNAMIC_CAST(SSFPlugin)(IterObject);
    }

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件
     */
    template <typename PluginT>
    SSF_PTR(PluginT)
    GetPlugin()
    {
        SSF_ASSERT_IS_BASE_OF(SSFPlugin, PluginT);

        SSFString PluginName;
        SSF_CLASS_NAME(PluginT, PluginName);
        return SSF_PTR_DYNAMIC_CAST(PluginT)(GetPlugin(PluginName));
    }

private:
    /**
     * 加载插件配置
     */
    void LoadPluginConfig(SSFObjectErrors &Errors);

    /**
     * 加载插件
     */
    void LoadPlugin(SSFObjectErrors &Errors);

    /**
     * 启动插件
     */
    void StartPlugin(SSFObjectErrors &Errors);

#pragma endregion Plugin

private:
    typedef void (*DLL_START_PLUGIN_FUNC)(SSF_PTR(SSFPluginManager));
    typedef void (*DLL_STOP_PLUGIN_FUNC)(SSF_PTR(SSFPluginManager));

    typedef SSFMap<std::string, bool> TMap_PluginName;
    typedef SSFMap<std::string, SSF_PTR_DYNAMIC_LIB> TMap_DynamicLib;
    typedef SSFMap<std::string, SSFObjectGUID> TMap_Plugin;

    TMap_PluginName PluginNameMap;
    TMap_DynamicLib DynamicLibMap;
    TMap_Plugin PluginMap;

    SKYWALKER_PTR_SCRIPT_PARSE PluginScriptParse = nullptr;
};

/**
 * 注册库
 */
#define SSF_REGISTER_LIBRARY(LibraryName)                                                     \
    SSFObjectErrors LibraryName##Errors;                                                      \
    if (DynamicLibMap.find(LibraryName) != DynamicLibMap.end())                               \
    {                                                                                         \
        SSF_ERROR_TRACE(LibraryName##Errors, SkywalkerSFError_Plugin_Load_Repeated);          \
        return;                                                                               \
    }                                                                                         \
    SSFDynamicLib *DynamicLib = new SSFDynamicLib(LibraryName);                               \
    if (DynamicLib == nullptr)                                                                \
    {                                                                                         \
        SSF_ERROR_TRACE(LibraryName##Errors, SkywalkerSFError_Plugin_Load_DynamicLibNullptr); \
        return;                                                                               \
    }                                                                                         \
    DynamicLibMap.insert(std::make_pair(LibraryName, DynamicLib));                            \
    if (!DynamicLib->Load())                                                                  \
    {                                                                                         \
        SSF_ERROR_TRACE(LibraryName##Errors, SkywalkerSFError_Plugin_Load_Failed);            \
        return;                                                                               \
    }                                                                                         \
    SSF_LOG_INFO("Register Library [" << LibraryName << "] Success");

/**
 * 注册插件
 */
#define SSF_REGISTER_PLUGIN(PluginManager, PluginClass)                                                              \
    SSF_ASSERT(SKYWALKER_IS_DERIVED(PluginClass, SSFPlugin));                                                        \
    SSFObjectErrors PluginClass##Errors;                                                                             \
    SSFPluginContext PluginClass##Context;                                                                           \
    PluginClass##Context.SSFramework = PluginManager->GetFramework();                                                \
    PluginClass##Context.PluginManager = PluginManager;                                                              \
    SSF_PTR_PLUGIN Plugin = new PluginClass(PluginClass##Context, PluginClass##Errors);                              \
    PluginManager->RegisterPlugin(PluginClass##Errors, Plugin);                                                      \
    if (PluginClass##Errors.IsValid())                                                                               \
    {                                                                                                                \
        auto FirstError = PluginClass##Errors.GetFirstError();                                                       \
        SSF_LOG_INFO("Register Plugin [" << Plugin->GetObjectClassName() << "] Failed " << FirstError.GetContent()); \
    }                                                                                                                \
    else                                                                                                             \
    {                                                                                                                \
        SSF_LOG_INFO("Register Plugin [" << Plugin->GetObjectClassName() << "] Success");                            \
    }

/**
 * 注销插件
 */
#define SSF_UNREGISTER_PLUGIN(PluginManager, PluginClass)                                                              \
    SSF_ASSERT(SKYWALKER_IS_DERIVED(PluginClass, SSFPlugin));                                                          \
    SSFObjectErrors PluginClass##Errors;                                                                               \
    SSFString PluginClassName;                                                                                         \
    SSF_CLASS_NAME(PluginClass, PluginClassName);                                                                      \
    SSF_PTR_PLUGIN Plugin = PluginManager->GetPlugin(PluginClassName);                                                 \
    PluginManager->UnregisterPlugin(PluginClass##Errors, Plugin);                                                      \
    if (PluginClass##Errors.IsValid())                                                                                 \
    {                                                                                                                  \
        auto FirstError = PluginClass##Errors.GetFirstError();                                                         \
        SSF_LOG_INFO("Unregister Plugin [" << Plugin->GetObjectClassName() << "] Failed " << FirstError.GetContent()); \
    }                                                                                                                  \
    else                                                                                                               \
    {                                                                                                                  \
        SSF_LOG_INFO("Unregister Plugin [" << Plugin->GetObjectClassName() << "] Success");                            \
    }                                                                                                                  \
    Plugin->Release(PluginClass##Errors);

/**
 * 注册模块
 */
#define SSF_REGISTER_MODULE(ModuleClass)                                                             \
    SSF_ASSERT(SKYWALKER_IS_DERIVED(ModuleClass, SSFModule));                                        \
    SSFObjectErrors ModuleClass##Errors;                                                             \
    SSFModuleContext ModuleClass##Context;                                                           \
    ModuleClass##Context.SSFramework = GetFramework();                                               \
    ModuleClass##Context.Plugin = this;                                                              \
    SSF_PTR_MODULE ModuleClass##Module = new ModuleClass(ModuleClass##Context, ModuleClass##Errors); \
    RegisterModule(ModuleClass##Errors, ModuleClass##Module);                                        \
    if (ModuleClass##Errors.IsValid())                                                               \
    {                                                                                                \
        SSF_ERROR_DESC(ModuleClass##Errors,                                                          \
                       SkywalkerSFError_Module_Register_Failed,                                      \
                       "Plugin RegisterModule Failed");                                              \
    }                                                                                                \
    SSF_LOG_INFO("Register Module [" << #ModuleClass << "] Success");                                \
    ModuleClass##Module->Init(ModuleClass##Errors);

/**
 * 注销模块
 */
#define SSF_UNREGISTER_MODULE(ModuleClass)                                  \
    SSF_ASSERT(SKYWALKER_IS_DERIVED(ModuleClass, SSFModule));               \
    SSF_PTR_MODULE ModuleClass##Module = GetModule<ModuleClass>();          \
    SSFObjectErrors ModuleClass##Errors;                                    \
    UnregisterModule(ModuleClass##Errors, ModuleClass##Module);             \
    if (ModuleClass##Errors.IsValid())                                      \
    {                                                                       \
        SSF_ERROR_DESC(ModuleClass##Errors,                                 \
                       SkywalkerSFError_Module_Unregister_Failed,           \
                       "Plugin UnregisterModule Failed");                   \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        SSF_LOG_INFO("Unregister Module [" << #ModuleClass << "] Success"); \
    }                                                                       \
    ModuleClass##Module->Release(ModuleClass##Errors);

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
