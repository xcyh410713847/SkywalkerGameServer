/*************************************************************************
**File: SkywalkerFramework\Core\Plugin\SFPluginManager.cpp
**Author: shyfan
**Date: 2023/08/07 23:53:15
**Desc: Plugin Manager
*************************************************************************/

#include "SFPluginManager.h"

#include "Include/SFILog.h"

#include "SkywalkerPlatform/SkywalkerPlatform.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFPluginManager, ESFLogLevel::Debug);

/** 构造函数 */
SFPluginManager::SFPluginManager(SSFObjectContext &InContext, SFObjectErrors &InErrors)
    : SSFObjectManager(InContext, InErrors)
{
}

/** 析构函数 */
SFPluginManager::~SFPluginManager()
{
}

/**
 * 释放插件管理器
 * 说明：先调用动态库停止入口，随后清空内存结构并释放基类对象。
 */
void SFPluginManager::Release(SFObjectErrors &Errors)
{
    SF_LOG_DEBUG_MODULE("Release");

    StopPlugin(Errors);

    PluginMap.clear();
    PluginNameMap.clear();
    PluginModulesMap.clear();

    DynamicLibMap.clear();

    PluginScriptParse = nullptr;

    SSFObjectManager::Release(Errors);
}

#pragma region Process

/**
 * 初始化流程
 * 1. 解析插件配置
 * 2. 加载动态库
 * 3. 调用动态库入口启动插件
 * 4. 逐个调用插件 Init
 */
void SFPluginManager::Init(SFObjectErrors &Errors)
{
    SF_LOG_DEBUG_MODULE("Init");

    SFObjectErrors PluginErrors;
    LoadPluginConfig(PluginErrors);
    if (PluginErrors.IsValid())
    {
        SF_ERROR_DESC(Errors, ESFError::Object_Init_Failed, "LoadPluginConfig Failed");
        return;
    }

    LoadPlugin(PluginErrors);
    if (PluginErrors.IsValid())
    {
        SF_ERROR_DESC(Errors, ESFError::Object_Init_Failed, "LoadPlugin Failed");
        return;
    }

    StartPlugin(PluginErrors);
    if (PluginErrors.IsValid())
    {
        SF_ERROR_DESC(Errors, ESFError::Object_Init_Failed, "StartPlugin Failed");
        return;
    }

    SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        auto IterObject = FindObject(IterPlugin->second);
        if (SF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SFPlugin *)IterObject)->Init(Errors);
    }
}

/** 唤醒阶段：逐个调用插件 Awake */
void SFPluginManager::Awake(SFObjectErrors &Errors)
{
    SF_LOG_DEBUG_MODULE("Awake");

    SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        auto IterObject = FindObject(IterPlugin->second);
        if (SF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SFPlugin *)IterObject)->Awake(Errors);
    }
}

/** 启动阶段：逐个调用插件 Start */
void SFPluginManager::Start(SFObjectErrors &Errors)
{
    SF_LOG_DEBUG_MODULE("Start");

    SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        auto IterObject = FindObject(IterPlugin->second);
        if (SF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SFPlugin *)IterObject)->Start(Errors);
    }
}

/** Tick 阶段：向所有插件广播帧间隔 */
void SFPluginManager::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        auto IterObject = FindObject(IterPlugin->second);
        if (SF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SFPlugin *)IterObject)->Tick(Errors, DelayMS);
    }
}

/** 停止阶段：逐个调用插件 Stop */
void SFPluginManager::Stop(SFObjectErrors &Errors)
{
    SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        auto IterObject = FindObject(IterPlugin->second);
        if (SF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SFPlugin *)IterObject)->Stop(Errors);
    }

    SF_LOG_DEBUG_MODULE("Stop");
}

/** 休眠阶段：逐个调用插件 Sleep */
void SFPluginManager::Sleep(SFObjectErrors &Errors)
{
    SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        auto IterObject = FindObject(IterPlugin->second);
        if (SF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SFPlugin *)IterObject)->Sleep(Errors);
    }

    SF_LOG_DEBUG_MODULE("Sleep");
}

/** 销毁阶段：逐个调用插件 Destroy */
void SFPluginManager::Destroy(SFObjectErrors &Errors)
{
    SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        auto IterObject = FindObject(IterPlugin->second);
        if (SF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SFPlugin *)IterObject)->Destroy(Errors);
    }

    SF_LOG_DEBUG_MODULE("Destroy");
}

#pragma endregion Process

#pragma region Plugin

/**
 * 注册插件
 * 说明：校验后写入对象管理器与插件索引，并尝试应用配置模块列表。
 */
void SFPluginManager::RegisterPlugin(SFObjectErrors &Errors, SF_PTR(SFPlugin) Plugin)
{
    if (!SF_PTR_VALID(Plugin))
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Plugin_Register_nullptr, "PluginManager Register Plugin nullptr");
        return;
    }

    SFString PluginName = Plugin->GetObjectClassName();
    if (PluginName.empty())
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Plugin_Register_NameEmpty, "PluginManager Register Plugin NameEmpty");
        return;
    }

    if (PluginMap.find(PluginName) != PluginMap.end())
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Plugin_Register_Repeat, "PluginManager Register Plugin Repeat");
        return;
    }

    AddObject(Errors, Plugin);
    if (Errors.IsValid())
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Plugin_Register_AddObjectFailed, "PluginManager Register Plugin AddObjectFailed");
        return;
    }
    PluginMap.insert(std::make_pair(PluginName, Plugin->GetObjectGUID()));

    SFMap<SFString, bool> MatchedModules;
    SFString MatchedPluginName;

    auto IterExact = PluginModulesMap.find(PluginName);
    if (IterExact != PluginModulesMap.end())
    {
        MatchedPluginName = IterExact->first;
        MatchedModules = IterExact->second;
    }
    else
    {
        SF_COMMON_ITERATOR(IterConfig, PluginModulesMap)
        {
            const SFString &ConfigPluginName = IterConfig->first;
            const SFString ScopedPluginName = "::" + ConfigPluginName;
            if (PluginName.size() > ScopedPluginName.size() &&
                PluginName.compare(PluginName.size() - ScopedPluginName.size(),
                                   ScopedPluginName.size(),
                                   ScopedPluginName) == 0)
            {
                MatchedPluginName = ConfigPluginName;
                MatchedModules = IterConfig->second;
                break;
            }
        }
    }

    if (!MatchedModules.empty())
    {
        Plugin->SetConfigModules(MatchedModules);
        SF_LOG_DEBUG("Apply plugin config modules for [" << PluginName << "] from [" << MatchedPluginName << "]");
    }
}

/** 注销插件：从对象管理器与插件索引中移除 */
void SFPluginManager::UnregisterPlugin(SFObjectErrors &Errors, SF_PTR(SFPlugin) Plugin)
{
    if (!SF_PTR_VALID(Plugin))
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Plugin_Unregister_nullptr, "PluginManager Unregister Plugin nullptr");
        return;
    }

    SFString PluginName = Plugin->GetObjectClassName();
    if (PluginName.empty())
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Plugin_Unregister_NameEmpty, "PluginManager Unregister Plugin NameEmpty");
        return;
    }

    auto Iter = PluginMap.find(PluginName);
    if (Iter == PluginMap.end())
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Plugin_Unregister_NotFound, "PluginManager Unregister Plugin NotFound");
        return;
    }

    RemoveObject(Errors, Plugin);
    if (Errors.IsValid())
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Plugin_Unregister_RemoveObjectFailed, "PluginManager Unregister Plugin RemoveObjectFailed");
        return;
    }
    PluginMap.erase(Iter);
}

/**
 * 加载插件配置
 * 说明：优先读取环境变量指定路径，未设置则回退到默认配置文件。
 */
void SFPluginManager::LoadPluginConfig(SFObjectErrors &Errors)
{
    const char *ConfigPath = nullptr;
#if defined(_WIN32) || defined(_WIN64)
    char *ConfigPathBuffer = nullptr;
    size_t ConfigPathLen = 0;
    _dupenv_s(&ConfigPathBuffer, &ConfigPathLen, SF_ENV_PLUGIN_CONFIG_DIR);
    ConfigPath = ConfigPathBuffer;
#else
    ConfigPath = getenv(SF_ENV_PLUGIN_CONFIG_DIR);
#endif
    SFString PluginConfigPath = ConfigPath ? ConfigPath : "ServerPlugin.skywalkerC";
#if defined(_WIN32) || defined(_WIN64)
    if (ConfigPathBuffer != nullptr)
    {
        free(ConfigPathBuffer);
        ConfigPathBuffer = nullptr;
    }
#endif

    PluginScriptParse = new SKYWALKER_SCRIPT_NAMESPACE::CSkywalkerScriptParse();
    if (!PluginScriptParse->LoadScript(PluginConfigPath.c_str()))
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Plugin_Load_ConfigNullptr, "LoadScript failed");
        return;
    }

    SKYWALKER_PTR_SCRIPT_NODE RootNode = PluginScriptParse->GetRootNode();
    if (RootNode == nullptr)
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Plugin_Load_ConfigNullptr, "RootNode is nullptr");
        return;
    }

    for (size_t i = 0; i < RootNode->GetChildNodeNum(); i++)
    {
        SKYWALKER_PTR_SCRIPT_NODE PluginNode = RootNode->GetChildNodeFromIndex(i);
        if (PluginNode == nullptr)
        {
            SF_ERROR_DESC_TRACE(Errors, ESFError::Plugin_Load_ConfigNullptr, "PluginNode is nullptr");
            continue;
        }

        SKYWALKER_PTR_SCRIPT_NODE NameNode = PluginNode->GetChildNodeFromName("Name");
        if (NameNode == nullptr)
        {
            SF_ERROR_DESC_TRACE(Errors, ESFError::Plugin_Load_ConfigNullptr, "NameNode is nullptr");
            continue;
        }

        SFString PluginName = NameNode->GetNodeValueString();
        PluginNameMap.insert(std::make_pair(PluginName, true));

        SFMap<SFString, bool> ModuleMap;
        for (size_t j = 0; j < PluginNode->GetChildNodeNum(); j++)
        {
            SKYWALKER_PTR_SCRIPT_NODE ModuleNode = PluginNode->GetChildNodeFromIndex(j);
            if (ModuleNode == nullptr)
            {
                continue;
            }

            SKYWALKER_PTR_SCRIPT_NODE ModuleNameNode = ModuleNode->GetChildNodeFromName("Name");
            if (ModuleNameNode == nullptr)
            {
                continue;
            }

            ModuleMap.insert(std::make_pair(ModuleNameNode->GetNodeValueString(), true));
        }
        PluginModulesMap.insert(std::make_pair(PluginName, ModuleMap));
    }
}

/** 加载插件动态库 */
void SFPluginManager::LoadPlugin(SFObjectErrors &Errors)
{
    SF_COMMON_ITERATOR(IterName, PluginNameMap)
    {
        const SFString LibraryName = IterName->first;
        SF_REGISTER_LIBRARY(LibraryName);
    }
}

/**
 * 启动动态库插件
 * 说明：调用每个动态库的 DllStartPlugin，并根据配置下发模块启用列表。
 */
void SFPluginManager::StartPlugin(SFObjectErrors &Errors)
{
    SF_COMMON_ITERATOR(IterLib, DynamicLibMap)
    {
        const SFString &LibraryName = IterLib->first;
        DLL_START_PLUGIN_FUNC DllStartPluginFunc = (DLL_START_PLUGIN_FUNC)IterLib->second->GetSymbol("DllStartPlugin");
        if (DllStartPluginFunc == nullptr)
        {
            SF_LOG_WARNING("StartPlugin [" << LibraryName << "] Skip: DllStartPlugin entry not found");
            continue;
        }

        DllStartPluginFunc((SF_PTR(SFPluginManager))(this));

        auto IterModules = PluginModulesMap.find(LibraryName);
        if (IterModules != PluginModulesMap.end())
        {
            SF_PTR(SFPlugin)
            Plugin = GetPlugin(LibraryName);
            if (Plugin == nullptr)
            {
                const SFString ScopedPluginName = "::" + LibraryName;
                SF_COMMON_ITERATOR(IterPlugin, PluginMap)
                {
                    const SFString &RegisteredPluginName = IterPlugin->first;
                    if (RegisteredPluginName == LibraryName)
                    {
                        auto IterObject = FindObject(IterPlugin->second);
                        if (SF_PTR_VALID(IterObject))
                        {
                            Plugin = SF_PTR_DYNAMIC_CAST(SFPlugin)(IterObject);
                            break;
                        }
                    }

                    if (RegisteredPluginName.size() > ScopedPluginName.size() &&
                        RegisteredPluginName.compare(RegisteredPluginName.size() - ScopedPluginName.size(),
                                                     ScopedPluginName.size(),
                                                     ScopedPluginName) == 0)
                    {
                        auto IterObject = FindObject(IterPlugin->second);
                        if (SF_PTR_VALID(IterObject))
                        {
                            Plugin = SF_PTR_DYNAMIC_CAST(SFPlugin)(IterObject);
                            break;
                        }
                    }
                }
            }

            if (Plugin != nullptr)
            {
                Plugin->SetConfigModules(IterModules->second);
            }
        }
    }
}

/** 停止动态库插件：调用每个动态库的 DllStopPlugin */
void SFPluginManager::StopPlugin(SFObjectErrors &Errors)
{
    SF_COMMON_ITERATOR(IterLib, DynamicLibMap)
    {
        DLL_STOP_PLUGIN_FUNC DllStopPluginFunc = (DLL_STOP_PLUGIN_FUNC)IterLib->second->GetSymbol("DllStopPlugin");
        if (DllStopPluginFunc == nullptr)
        {
            continue;
        }

        DllStopPluginFunc((SF_PTR(SFPluginManager))(this));
    }
}

#pragma endregion Plugin
