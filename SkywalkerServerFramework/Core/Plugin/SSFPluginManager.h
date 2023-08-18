/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPluginManager.h
**作者: shyfan
**日期: 2023/08/07 23:53:25
**功能: 插件管理器
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__

#include "SkywalkerSingleton/SkywalkerSingleton.h"

#include "Include/SSFIPluginManager.h"

#include "Core/Object/SSFObject.h"
#include "Core/Map/SSFMap.h"
#include "Core/DynamicLib/SSFDynamicLib.h"

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
    virtual void RegisterPlugin(SSFPluginErrors &Errors, SSF_PTR_PLUGIN Plugin) override;

    /**
     * 注销插件
     * @param Plugin 插件
     */
    virtual void UnregisterPlugin(SSFPluginErrors &Errors, SSF_PTR_PLUGIN Plugin) override;

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件
     */
    virtual SSF_PTR_PLUGIN GetPlugin(const std::string &PluginName) override;

    /**
     * 注册模块
     * @param Module 模块
     */
    virtual void RegisterModule(SSFModuleErrors &Errors, SSF_PTR_PLUGIN Plugin, SSF_PTR_MODULE Module) override;

    /**
     * 注销模块
     * @param Module 模块
     */
    virtual void UnregisterModule(SSFModuleErrors &Errors, SSF_PTR_PLUGIN Plugin, SSF_PTR_MODULE Module) override;

    /**
     * 获取模块
     * @param ModuleName 模块名称
     * @return 模块
     */
    virtual SSF_PTR_MODULE GetModule(const std::string &ModuleName) override;

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
    /**
     * 插件
     */
    typedef void (*DLL_START_PLUGIN_FUNC)(SKYWALKER_SF_PTR_PLUGIN_MANAGER);
    typedef void (*DLL_STOP_PLUGIN_FUNC)(SKYWALKER_SF_PTR_PLUGIN_MANAGER);

    typedef SSFMap<std::string, bool> TMap_PluginName;
    typedef SSFMap<std::string, SKYWALKER_SF_PTR_DYNAMIC_LIB> TMap_DynamicLib;
    typedef std::map<std::string, SSF_PTR_PLUGIN> TMap_Plugin;

    TMap_PluginName PluginNameMap;
    TMap_DynamicLib DynamicLibMap;
    TMap_Plugin PluginMap;

    // 模块映射
    typedef SSFMap<std::string, std::string> TMap_ModuleToPlugin;
    TMap_ModuleToPlugin ModuleToPluginMap;
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
