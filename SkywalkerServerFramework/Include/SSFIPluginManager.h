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
     * 注册插件
     * @param Plugin 插件
     */
    virtual void RegisterPlugin(SSFPluginErrors &Errors, SSFIPlugin *Plugin) = 0;

    /**
     * 注销插件
     * @param Plugin 插件
     */
    virtual void UnregisterPlugin(SSFPluginErrors &Errors, SSFIPlugin *Plugin) = 0;

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件
     */
    virtual SSFIPlugin *GetPlugin(const std::string &PluginName) = 0;
};

SKYWALKER_SF_NAMESPACE_END

// SSFIPluginManager 智能指针
typedef SKYWALKER_SF_NAMESPACE::SSFIPluginManager *SSFSharedPtr_IPluginManager;

/**
 * 获取插件管理器
 * @return 插件管理器
 */
SKYWALKER_SF_API SSFSharedPtr_IPluginManager SkywalkerServerFramework_GetPluginManager();

#endif // __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_MANAGER_H__
