/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFIPluginManager.h
**作者: shyfan
**日期: 2023/07/30 12:37:08
**功能: 插件管理器接口
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_MANAGER_H__

#include <string>

#include "SSFCore.h"

#include "SSFErrors.h"

#include "SSFIPlugin.h"

SKYWALKER_SF_NAMESPACE_BEGIN

struct SSFIPluginManager
{
    /**
     * 加载插件配置
     */
    virtual void LoadPluginConfig(SSFPluginErrors &Errors) = 0;

    /**
     * 加载库
     */
    virtual void LoadPlugin(SSFPluginErrors &Errors) = 0;

    /**
     * 注册插件
     * @param Plugin 插件
     */
    virtual void RegisterPlugin(SSFPluginErrors &Errors, SSFPtr_IPlugin Plugin) = 0;

    /**
     * 注销插件
     * @param Plugin 插件
     */
    virtual void UnregisterPlugin(SSFPluginErrors &Errors, SSFPtr_IPlugin Plugin) = 0;

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件
     */
    virtual SSFPtr_IPlugin GetPlugin(const std::string &PluginName) = 0;

    /**
     * Tick
     */
    virtual void Tick() = 0;

    /**
     * 结束
     */
    virtual void Stop() = 0;

    /**
     * 释放
     */
    virtual void Release() = 0;
};

SKYWALKER_SF_NAMESPACE_END

/**
 * SSFIPluginManager 智能指针
 */
typedef SKYWALKER_SF_NAMESPACE::SSFIPluginManager *SSFPtr_IPluginManager;

/**
 * 创建插件
 */
#define SKYWALKER_SF_CREATE_PLUGIN(PluginManager, ClassName) \
    SSFPluginErrors Errors;                                  \
    PluginManager->RegisterPlugin(Errors, new ClassName(PluginManager));

/**
 * 销毁插件
 */
#define SKYWALKER_SF_DESTROY_PLUGIN(PluginManager, ClassName) \
    SSFPluginErrors Errors;                                   \
    PluginManager->UnregisterPlugin(Errors, PluginManager->GetPlugin((#ClassName)));

#endif // __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_MANAGER_H__
