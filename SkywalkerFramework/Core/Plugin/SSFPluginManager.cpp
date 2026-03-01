/*************************************************************************
**File: SkywalkerFramework\Core\Plugin\SSFPluginManager.cpp
**Author: shyfan
**Date: 2023/08/07 23:53:15
**Desc: Plugin Manager
*************************************************************************/

#include "SSFPluginManager.h"

#include "Include/SFILog.h"

#include "SkywalkerPlatform/SkywalkerPlatform.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFPluginManager, ESFLogLevel::Debug);

SFPluginManager::SFPluginManager(SSFObjectContext &InContext, SFObjectErrors &InErrors)
    : SSFObjectManager(InContext, InErrors)
{
}

SFPluginManager::~SFPluginManager()
{
}

void SFPluginManager::Release(SFObjectErrors &Errors)
{
    SF_LOG_DEBUG_MODULE("Release");

    PluginMap.clear();
    DynamicLibMap.clear();
    PluginNameMap.clear();

    PluginScriptParse = nullptr;

    SSFObjectManager::Release(Errors);
}

#pragma region Process

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

void SFPluginManager::Tick(SFObjectErrors &Errors, int DelayMS)
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
}

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

void SFPluginManager::LoadPluginConfig(SFObjectErrors &Errors)
{
    const char *ConfigPath = getenv("SKYWALKER_PLUGIN_CONFIG");
    SFString PluginConfigPath = ConfigPath ? ConfigPath : "ServerPlugin.skywalkerC";

    PluginScriptParse = new SKYWALKER_SCRIPT_NAMESPACE::CSkywalkerScriptParse();
    if (!PluginScriptParse->LoadScript(PluginConfigPath.c_str()))
    {
        SF_ERROR_TRACE(Errors, ESFError::Plugin_Load_ConfigNullptr);
        return;
    }

    SKYWALKER_PTR_SCRIPT_NODE RootNode = PluginScriptParse->GetRootNode();
    if (RootNode == nullptr)
    {
        SF_ERROR_TRACE(Errors, ESFError::Plugin_Load_ConfigNullptr);
        return;
    }

    for (int i = 0; i < RootNode->GetChildNodeNum(); i++)
    {
        SKYWALKER_PTR_SCRIPT_NODE PluginNode = RootNode->GetChildNodeFromIndex(i);
        if (PluginNode == nullptr)
        {
            SF_ERROR_TRACE(Errors, ESFError::Plugin_Load_ConfigNullptr);
            continue;
        }

        SKYWALKER_PTR_SCRIPT_NODE NameNode = PluginNode->GetChildNodeFromName("Name");
        if (NameNode == nullptr)
        {
            SF_ERROR_TRACE(Errors, ESFError::Plugin_Load_ConfigNullptr);
            continue;
        }

        SFString PluginName = NameNode->GetNodeValueString();
        PluginNameMap.insert(std::make_pair(PluginName, true));

        SFMap<SFString, bool> ModuleMap;
        for (int j = 0; j < PluginNode->GetChildNodeNum(); j++)
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

void SFPluginManager::LoadPlugin(SFObjectErrors &Errors)
{
    SF_COMMON_ITERATOR(IterName, PluginNameMap)
    {
        const SFString LibraryName = IterName->first;
        SF_REGISTER_LIBRARY(LibraryName);
    }
}

void SFPluginManager::StartPlugin(SFObjectErrors &Errors)
{
    SF_COMMON_ITERATOR(IterLib, DynamicLibMap)
    {
        const SFString &LibraryName = IterLib->first;
        DLL_START_PLUGIN_FUNC DllStartPluginFunc = (DLL_START_PLUGIN_FUNC)IterLib->second->GetSymbol("DllStartPlugin");
        if (DllStartPluginFunc == nullptr)
        {
            SF_ERROR_TRACE(Errors, ESFError::Plugin_Load_EntryNullptr);
            continue;
        }

        DllStartPluginFunc((SF_PTR(SFPluginManager))(this));

        auto IterModules = PluginModulesMap.find(LibraryName);
        if (IterModules != PluginModulesMap.end())
        {
            SF_PTR(SFPlugin)
            Plugin = GetPlugin(LibraryName);
            if (Plugin != nullptr)
            {
                Plugin->SetConfigModules(IterModules->second);
            }
        }
    }
}

#pragma endregion Plugin
