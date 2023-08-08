/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPluginManager.h
**作者: shyfan
**日期: 2023/08/07 23:53:25
**功能: 插件管理器
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
    virtual void RegisterPlugin(SSFPluginErrors &Errors, SSFSharedPtr_IPlugin Plugin) override;

    /**
     * 注销插件
     * @param Plugin 插件
     */
    virtual void UnregisterPlugin(SSFPluginErrors &Errors, SSFSharedPtr_IPlugin Plugin) override;

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件
     */
    virtual SSFSharedPtr_IPlugin GetPlugin(const std::string &PluginName) override;

    /**
     * Tick
     */
    virtual void Tick() override;

    /**
     * 结束
     */
    virtual void Stop() override;

    /**
     * 释放
     */
    virtual void Release() override;

#pragma endregion SSFIPluginManager

private:
    typedef std::map<std::string, SSFSharedPtr_IPlugin> TMap_Plugin;
    TMap_Plugin PluginMap;
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
