/*************************************************************************
**文件: SkywalkerFramework\Core\Plugin\SFPluginManager.h
**作者: shyfan
**日期: 2023/08/07 23:53:25
**功能: 插件管理器
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__

#include "SkywalkerScript/Include/SkywalkerScriptParse.h"

#include "Include/SFCore.h"

#include "Core/DynamicLib/SFDynamicLib.h"
#include "Core/Plugin/SFPlugin.h"

SF_NAMESPACE_BEGIN

/**
 * 插件管理器
 *
 * 职责：
 * 1. 管理插件生命周期（Init/Awake/Start/Tick/Stop/Sleep/Destroy）
 * 2. 管理插件注册表与对象映射
 * 3. 解析插件配置并按配置加载动态库
 */
class SFPluginManager : public SSFObjectManager<SFPlugin>
{
public:
    /** 构造函数 */
    SFPluginManager(SSFObjectContext &InContext, SFObjectErrors &InErrors);
    /** 析构函数 */
    virtual ~SFPluginManager();

    /**
     * 释放插件管理器
     * 说明：会先停止动态库插件，再清理映射与脚本解析器。
     */
    virtual void Release(SFObjectErrors &Errors) override;

#pragma region Process

public:
    /**
     * 初始化插件管理器
     * 流程：加载配置 -> 加载动态库 -> 启动动态库入口 -> 调用插件 Init。
     */
    virtual void Init(SFObjectErrors &Errors);

    /**
     * 唤醒所有插件
     */
    virtual void Awake(SFObjectErrors &Errors);

    /**
     * 启动所有插件
     */
    virtual void Start(SFObjectErrors &Errors);

    /**
     * 驱动所有插件 Tick
     * @param DelayMS 帧间隔（毫秒）
     */
    virtual void Tick(SFObjectErrors &Errors, SFUInt64 DelayMS);

    /**
     * 停止所有插件
     */
    virtual void Stop(SFObjectErrors &Errors);

    /**
     * 使所有插件进入休眠阶段
     */
    virtual void Sleep(SFObjectErrors &Errors);

    /**
     * 销毁所有插件
     */
    virtual void Destroy(SFObjectErrors &Errors);

#pragma endregion Process

#pragma region Plugin

public:
    /**
     * 注册插件
     * @param Errors 错误输出对象
     * @param Plugin 待注册插件
     * 说明：注册成功后会写入对象管理器与插件名索引。
     */
    virtual void RegisterPlugin(SFObjectErrors &Errors, SF_PTR(SFPlugin) Plugin);

    /**
     * 注销插件
     * @param Errors 错误输出对象
     * @param Plugin 待注销插件
     */
    virtual void UnregisterPlugin(SFObjectErrors &Errors, SF_PTR(SFPlugin) Plugin);

    /**
     * 获取插件
     * @param PluginName 插件名称
     * @return 插件指针；未找到返回 nullptr
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
     * 按类型获取插件
     * @tparam PluginT 插件类型（必须继承自 SFPlugin）
     * @return 插件指针；未找到或类型不匹配返回 nullptr
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
     * 说明：读取插件清单及模块启用信息。
     */
    void LoadPluginConfig(SFObjectErrors &Errors);

    /**
     * 加载插件动态库
     */
    void LoadPlugin(SFObjectErrors &Errors);

    /**
     * 调用动态库入口启动插件
     */
    void StartPlugin(SFObjectErrors &Errors);

    /**
     * 调用动态库入口停止插件
     */
    void StopPlugin(SFObjectErrors &Errors);

#pragma endregion Plugin

private:
    /** 动态库导出：启动插件入口函数 */
    typedef void (*DLL_START_PLUGIN_FUNC)(SF_PTR(SFPluginManager));
    /** 动态库导出：停止插件入口函数 */
    typedef void (*DLL_STOP_PLUGIN_FUNC)(SF_PTR(SFPluginManager));

    /** 插件名映射表（插件名 -> 是否启用） */
    typedef SFMap<SFString, bool> TMap_PluginName;
    /** 插件模块映射表（插件名 -> 模块名映射） */
    typedef SFMap<SFString, SFMap<SFString, bool>> TMap_PluginModules;
    /** 动态库映射表（库名 -> 动态库对象） */
    typedef SFMap<SFString, SSF_PTR_DYNAMIC_LIB> TMap_DynamicLib;
    /** 插件索引表（插件名 -> ObjectGUID） */
    typedef SFMap<SFString, SFObjectGUID> TMap_Plugin;

    /** 插件配置中的插件列表 */
    TMap_PluginName PluginNameMap;
    /** 插件配置中的模块启用信息 */
    TMap_PluginModules PluginModulesMap;
    /** 已加载动态库列表 */
    TMap_DynamicLib DynamicLibMap;
    /** 已注册插件索引 */
    TMap_Plugin PluginMap;

    /** 脚本解析器实例（用于解析插件配置） */
    SKYWALKER_PTR_SCRIPT_PARSE PluginScriptParse = nullptr;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
