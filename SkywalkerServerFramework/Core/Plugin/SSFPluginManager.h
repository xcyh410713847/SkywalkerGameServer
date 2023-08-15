/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPluginManager.h
**作者: shyfan
**日期: 2023/08/07 23:53:25
**功能: 插件管理器
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__

#include "SkywalkerSingleton\SkywalkerSingleton.h"

#include "Include\SSFIPluginManager.h"

#include "Core\Object\SSFObject.h"
#include "Core\Map\SSFMap.h"
#include "Core\DynamicLib\SSFDynamicLib.h"

SKYWALKER_SF_NAMESPACE_BEGIN

/**
 * 插件管理器
 */
class SSFCPluginManager
    : public SSFIPluginManager,
      public SSFCObject
{
    SKYWALKER_SINGLETON_DECLARE(SSFCPluginManager);

#pragma region Object Base Interface

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

#pragma endregion Object Base Interface

#pragma region SSFIPluginManager

public:
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

#pragma endregion SSFIPluginManager

private:
    /**
     * 加载插件配置
     */
    void LoadPluginConfig(SSFPluginErrors &Errors);

    /**
     * 加载库
     */
    void LoadPlugin(SSFPluginErrors &Errors);

    /**
     * 加载插件库
     */
    void LoadPluginLib(SSFPluginErrors &Errors, const std::string &PluginName);

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
