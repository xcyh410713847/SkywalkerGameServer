/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFIPluginManager.h
**作者: shyfan
**日期: 2023/07/30 12:37:08
**功能: 插件管理器接口
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_MANAGER_H__

#include "SkywalkerDerived\SkywalkerDerived.h"

#include "SSFCore.h"
#include "SSFErrors.h"
#include "SSFIPlugin.h"
#include "SSFIModule.h"

SKYWALKER_SF_NAMESPACE_BEGIN

struct SSFIPluginManager
{
    /**
     * 注册插件
     * @param Plugin 插件
     */
    virtual void RegisterPlugin(SSFPluginErrors &Errors, SSF_PTR_PLUGIN Plugin) = 0;

    /**
     * 注销插件
     * @param Plugin 插件
     */
    virtual void UnregisterPlugin(SSFPluginErrors &Errors, SSF_PTR_PLUGIN Plugin) = 0;

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件
     */
    virtual SSF_PTR_PLUGIN GetPlugin(const std::string &PluginName) = 0;

    /**
     * 注册模块
     * @param Module 模块
     */
    virtual void RegisterModule(SSFModuleErrors &Errors, SSF_PTR_PLUGIN Plugin, SSF_PTR_MODULE Module) = 0;

    /**
     * 注销模块
     * @param Module 模块
     */
    virtual void UnregisterModule(SSFModuleErrors &Errors, SSF_PTR_PLUGIN Plugin, SSF_PTR_MODULE Module) = 0;

    /**
     * 获取模块
     * @param ModuleName 模块名称
     * @return 模块
     */
    virtual SSF_PTR_MODULE GetModule(const std::string &ModuleName) = 0;
};

/**
 * SSFIPluginManager 指针
 */
#define SKYWALKER_SF_PTR_PLUGIN_MANAGER SKYWALKER_SF_PTR(SSFIPluginManager)

/**
 * 注册插件
 */
#define SKYWALKER_SF_REGISTER_PLUGIN(PluginManager, ClassName)                                \
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(ClassName, SSFIPlugin));                         \
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
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(ClassName, SSFIPlugin));                           \
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
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(ClassName, SSFIModule));                                                                \
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(Plugin, SSFIPlugin));                                                                   \
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
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(ClassName, SSFIModule));                                                                    \
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(Plugin, SSFIPlugin));                                                                       \
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

#endif // __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_MANAGER_H__
