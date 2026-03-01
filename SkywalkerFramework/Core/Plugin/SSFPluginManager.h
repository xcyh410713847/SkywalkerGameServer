/*************************************************************************
**文件: SkywalkerFramework\Core\Plugin\SSFPluginManager.h
**作者: shyfan
**日期: 2023/08/07 23:53:25
**功能: 插件管理器
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__

#include "SkywalkerScript/Include/SkywalkerScriptParse.h"

#include "Include/SFCore.h"

#include "Core/DynamicLib/SSFDynamicLib.h"
#include "Core/Plugin/SSFPlugin.h"

SF_NAMESPACE_BEGIN

/**
 * 插件管理器
 */
class SFPluginManager : public SSFObjectManager<SFPlugin>
{
public:
    SFPluginManager(SSFObjectContext &InContext, SFObjectErrors &InErrors);
    virtual ~SFPluginManager();

    /**
     * 释放
     */
    virtual void Release(SFObjectErrors &Errors) override;

#pragma region Process

public:
    /**
     * 初始化
     */
    virtual void Init(SFObjectErrors &Errors);

    /**
     * 唤醒
     */
    virtual void Awake(SFObjectErrors &Errors);

    /**
     * 开始
     */
    virtual void Start(SFObjectErrors &Errors);

    /**
     * Tick
     */
    virtual void Tick(SFObjectErrors &Errors, int DelayMS);

    /**
     * 结束
     */
    virtual void Stop(SFObjectErrors &Errors);

    /**
     * 休眠
     */
    virtual void Sleep(SFObjectErrors &Errors);

    /**
     * 销毁
     */
    virtual void Destroy(SFObjectErrors &Errors);

#pragma endregion Process

#pragma region Plugin

public:
    /**
     * 注册插件
     * @param Plugin 插件
     */
    virtual void RegisterPlugin(SFObjectErrors &Errors, SF_PTR(SFPlugin) Plugin);

    /**
     * 注销插件
     * @param Plugin 插件
     */
    virtual void UnregisterPlugin(SFObjectErrors &Errors, SF_PTR(SFPlugin) Plugin);

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件
     */
    inline SF_PTR(SFPlugin) GetPlugin(const SFString &PluginName)
    {
        auto Iter = PluginMap.find(PluginName);
        if (Iter == PluginMap.end())
        {
            return nullptr;
        }

        auto IterObject = FindObject(Iter->second);
        if (SF_PTR_INVALID(IterObject))
        {
            return nullptr;
        }

        return SF_PTR_DYNAMIC_CAST(SFPlugin)(IterObject);
    }

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件
     */
    template <typename PluginT>
    SF_PTR(PluginT)
    GetPlugin()
    {
        SF_ASSERT_IS_BASE_OF(SFPlugin, PluginT);

        SFString PluginName;
        SF_CLASS_NAME(PluginT, PluginName);
        return SF_PTR_DYNAMIC_CAST(PluginT)(GetPlugin(PluginName));
    }

private:
    /**
     * 加载插件配置
     */
    void LoadPluginConfig(SFObjectErrors &Errors);

    /**
     * 加载插件
     */
    void LoadPlugin(SFObjectErrors &Errors);

    /**
     * 启动插件
     */
    void StartPlugin(SFObjectErrors &Errors);

#pragma endregion Plugin

private:
    typedef void (*DLL_START_PLUGIN_FUNC)(SF_PTR(SFPluginManager));
    typedef void (*DLL_STOP_PLUGIN_FUNC)(SF_PTR(SFPluginManager));

    typedef SFMap<SFString, bool> TMap_PluginName;
    typedef SFMap<SFString, SFMap<SFString, bool>> TMap_PluginModules;
    typedef SFMap<SFString, SSF_PTR_DYNAMIC_LIB> TMap_DynamicLib;
    typedef SFMap<SFString, SFObjectGUID> TMap_Plugin;

    TMap_PluginName PluginNameMap;
    TMap_PluginModules PluginModulesMap;
    TMap_DynamicLib DynamicLibMap;
    TMap_Plugin PluginMap;

    SKYWALKER_PTR_SCRIPT_PARSE PluginScriptParse = nullptr;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
