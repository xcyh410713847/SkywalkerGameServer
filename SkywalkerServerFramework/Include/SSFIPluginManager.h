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
#include "SSFPluginDefinition.h"
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
    virtual void RegisterModule(SSFModuleErrors &Errors, SSF_PTR_MODULE Module) = 0;

    /**
     * 注销模块
     * @param Module 模块
     */
    virtual void UnregisterModule(SSFModuleErrors &Errors, SSF_PTR_MODULE Module) = 0;

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
 * 创建插件
 */
#define SKYWALKER_SF_CREATE_PLUGIN(PluginManager, ClassName) \
    assert(SKYWALKER_IS_DERIVED(ClassName, SSFIPlugin));     \
    SSFPluginErrors Errors;                                  \
    PluginManager->RegisterPlugin(Errors, new ClassName(PluginManager));

/**
 * 销毁插件
 */
#define SKYWALKER_SF_DESTROY_PLUGIN(PluginManager, ClassName)         \
    SKYWALKER_SF_ASSERT(SKYWALKER_IS_DERIVED(ClassName, SSFIPlugin)); \
    SSFPluginErrors Errors;                                           \
    PluginManager->UnregisterPlugin(Errors, PluginManager->GetPlugin((#ClassName)));

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_MANAGER_H__
