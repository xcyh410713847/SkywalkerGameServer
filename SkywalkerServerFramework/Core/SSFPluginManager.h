/*************************************************************************
**文件: SkywalkerServerFramework\Core\SSFPluginManager.h
**作者: shyfan
**日期: 2023/07/30 12:57:31
**功能:
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__

#include <map>
#include <string>

#include "Include\SSFIPluginManager.h"
#include "Include\SSFPluginDefinition.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFCPluginManager : public SSFIPluginManager
{
public:
    SSFCPluginManager();
    virtual ~SSFCPluginManager();

#pragma region SSFIPluginManager
    /**
     * 注册插件
     * @param Plugin 插件
     */
    virtual void RegisterPlugin(SSFPluginErrors &Errors, SSFIPlugin *Plugin) override;

    /**
     * 注销插件
     * @param Plugin 插件
     */
    virtual void UnregisterPlugin(SSFPluginErrors &Errors, SSFIPlugin *Plugin) override;

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件
     */
    virtual SSFIPlugin *GetPlugin(const std::string &PluginName) override;

#pragma endregion SSFIPluginManager

private:
    typedef std::map<std::string, SSFIPlugin *> TMap_Plugin;
    TMap_Plugin PluginMap;
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
