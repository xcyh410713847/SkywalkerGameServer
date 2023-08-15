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

#include "SkywalkerSingleton\SkywalkerSingleton.h"

#include "Include\SSFIPluginManager.h"
#include "Include\SSFPluginDefinition.h"

#include "Core\Map\SSFMap.h"
#include "Core\DynamicLib\SSFDynamicLib.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFCPluginManager : public SSFIPluginManager
{
    SKYWALKER_SINGLETON_DECLARE(SSFCPluginManager);

#pragma region SSFIPluginManager
public:
    /**
     * 加载插件配置
     */
    virtual void LoadPluginConfig(SSFPluginErrors &Errors) override;

    /**
     * 加载库
     */
    virtual void LoadPlugin(SSFPluginErrors &Errors) override;

    /**
     * 注册插件
     * @param Plugin 插件
     */
    virtual void RegisterPlugin(SSFPluginErrors &Errors, SSFPtr_IPlugin Plugin) override;

    /**
     * 注销插件
     * @param Plugin 插件
     */
    virtual void UnregisterPlugin(SSFPluginErrors &Errors, SSFPtr_IPlugin Plugin) override;

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件
     */
    virtual SSFPtr_IPlugin GetPlugin(const std::string &PluginName) override;

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
    void LoadPluginLib(const std::string &PluginName);

private:
    typedef void (*DLL_START_PLUGIN_FUNC)(SSFPtr_IPluginManager);
    typedef void (*DLL_STOP_PLUGIN_FUNC)(SSFPtr_IPluginManager);

    typedef SSFMap<std::string, bool> TMap_PluginName;
    TMap_PluginName PluginNameMap;

    typedef SSFMap<std::string, SSFSharedPtr_DynamicLib> TMap_DynamicLib;
    TMap_DynamicLib DynamicLibMap;

    typedef std::map<std::string, SSFPtr_IPlugin> TMap_Plugin;
    TMap_Plugin PluginMap;
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
