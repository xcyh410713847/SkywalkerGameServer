/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPluginManager.h
**作者: shyfan
**日期: 2023/08/07 23:53:25
**功能: 插件管理器
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__

#include "SkywalkerScript/Include/SkywalkerScriptParse.h"

#include "Include/SSFCore.h"

#include "Core/DynamicLib/SSFDynamicLib.h"
#include "Core/Plugin/SSFPlugin.h"

SSF_NAMESPACE_BEGIN

/**
 * 插件管理器
 */
class SSFPluginManager : public SSFObjectManager<SSFPlugin>
{
    SSF_OBJECT_CLASS(SSFPluginManager)

public:
    SSFPluginManager(SSFObjectContext &InContext, SSFObjectErrors &InErrors);
    virtual ~SSFPluginManager();

    /**
     * 释放
     */
    virtual void Release(SSFObjectErrors &Errors) override;

#pragma region Process

public:
    /**
     * 初始化
     */
    virtual void Init(SSFObjectErrors &Errors);

    /**
     * 唤醒
     */
    virtual void Awake(SSFObjectErrors &Errors);

    /**
     * 开始
     */
    virtual void Start(SSFObjectErrors &Errors);

    /**
     * Tick
     */
    virtual void Tick(SSFObjectErrors &Errors, int DelayMS);

    /**
     * 结束
     */
    virtual void Stop(SSFObjectErrors &Errors);

    /**
     * 休眠
     */
    virtual void Sleep(SSFObjectErrors &Errors);

    /**
     * 销毁
     */
    virtual void Destroy(SSFObjectErrors &Errors);

#pragma endregion Process

#pragma region Plugin

public:
    /**
     * 注册插件
     * @param Plugin 插件
     */
    virtual void RegisterPlugin(SSFObjectErrors &Errors, SSF_PTR(SSFPlugin) Plugin);

    /**
     * 注销插件
     * @param Plugin 插件
     */
    virtual void UnregisterPlugin(SSFObjectErrors &Errors, SSF_PTR(SSFPlugin) Plugin);

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件
     */
    inline SSF_PTR(SSFPlugin) GetPlugin(const std::string &PluginName)
    {
        auto Iter = PluginMap.find(PluginName);
        if (Iter == PluginMap.end())
        {
            return nullptr;
        }

        auto IterObject = FindObject(Iter->second);
        if (SSF_PTR_INVALID(IterObject))
        {
            return nullptr;
        }

        return SSF_PTR_DYNAMIC_CAST(SSFPlugin)(IterObject);
    }

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件
     */
    template <typename PluginT>
    SSF_PTR(PluginT)
    GetPlugin()
    {
        SSF_ASSERT_IS_BASE_OF(SSFPlugin, PluginT);

        SSFString PluginName;
        SSF_CLASS_NAME(PluginT, PluginName);
        return SSF_PTR_DYNAMIC_CAST(PluginT)(GetPlugin(PluginName));
    }

private:
    /**
     * 加载插件配置
     */
    void LoadPluginConfig(SSFObjectErrors &Errors);

    /**
     * 加载插件
     */
    void LoadPlugin(SSFObjectErrors &Errors);

    /**
     * 启动插件
     */
    void StartPlugin(SSFObjectErrors &Errors);

#pragma endregion Plugin

private:
    typedef void (*DLL_START_PLUGIN_FUNC)(SSF_PTR(SSFPluginManager));
    typedef void (*DLL_STOP_PLUGIN_FUNC)(SSF_PTR(SSFPluginManager));

    typedef SSFMap<std::string, bool> TMap_PluginName;
    typedef SSFMap<std::string, SSF_PTR_DYNAMIC_LIB> TMap_DynamicLib;
    typedef SSFMap<std::string, SSFObjectGUID> TMap_Plugin;

    TMap_PluginName PluginNameMap;
    TMap_DynamicLib DynamicLibMap;
    TMap_Plugin PluginMap;

    SKYWALKER_PTR_SCRIPT_PARSE PluginScriptParse = nullptr;
};

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
