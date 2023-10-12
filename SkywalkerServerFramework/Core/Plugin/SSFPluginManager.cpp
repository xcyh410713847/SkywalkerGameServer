﻿/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPluginManager.cpp
**作者: shyfan
**日期: 2023/08/07 23:53:15
**功能: 插件管理器
*************************************************************************/

#include "SSFPluginManager.h"

#include "Core/Object/SSFObject.h"
#include "Core/Plugin/SSFPlugin.h"
#include "Core/Module/SSFModule.h"
#include "SkywalkerScript/Include/SkywalkerScriptParse.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFPluginManager, LogLevel_Debug);

#pragma region Object

void SSFOPluginManager::Init(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG("Init");

    SSFObject::Init(Errors);

    SSFPluginErrors PluginErrors;
    LoadPluginConfig(PluginErrors);
    if (PluginErrors.IsValid())
    {
        SSF_ERROR_DESC(Errors, SkywalkerSFError_Object_Init_Failed, "LoadPluginConfig Failed");
        return;
    }

    LoadPlugin(PluginErrors);
    if (PluginErrors.IsValid())
    {
        SSF_ERROR_DESC(Errors, SkywalkerSFError_Object_Init_Failed, "LoadPlugin Failed");
        return;
    }

    StartPlugin(PluginErrors);
    if (PluginErrors.IsValid())
    {
        SSF_ERROR_DESC(Errors, SkywalkerSFError_Object_Init_Failed, "StartPlugin Failed");
        return;
    }

    SSF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Init(Errors);
    }
}

void SSFOPluginManager::Awake(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG("Awake");

    SSFObject::Awake(Errors);

    SSF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Awake(Errors);
    }
}

void SSFOPluginManager::Start(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG("Start");

    SSFObject::Start(Errors);

    SSF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Start(Errors);
    }
}

void SSFOPluginManager::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SSFObject::Tick(Errors, DelayMS);

    SSF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Tick(Errors, DelayMS);
    }
}

void SSFOPluginManager::Stop(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Stop(Errors);
    }

    SSFObject::Stop(Errors);

    SSF_LOG_DEBUG("Stop");
}

void SSFOPluginManager::Sleep(SSFObjectErrors &Errors)
{

    SSF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Sleep(Errors);
    }

    SSFObject::Sleep(Errors);

    SSF_LOG_DEBUG("Sleep");
}

void SSFOPluginManager::Destroy(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Destroy(Errors);
    }

    SSFObject::Destroy(Errors);

    SSF_LOG_DEBUG("Destroy");
}

void SSFOPluginManager::Release(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Release(Errors);
    }

    SSF_LOG_DEBUG("Release");

    SSFObject::Release(Errors);
}

#pragma endregion Object

#pragma region SSFIPluginManager

void SSFOPluginManager::RegisterPlugin(SSFPluginErrors &Errors, SSF_PTR_PLUGIN Plugin)
{
    if (!SSF_PTR_VALID(Plugin))
    {
        SSF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Register_nullptr);
        return;
    }

    std::string PluginName = Plugin->GetName();
    if (PluginName.empty())
    {
        SSF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Register_NameEmpty);
        return;
    }

    auto Iter = PluginMap.find(PluginName);
    if (Iter != PluginMap.end())
    {
        SSF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Register_Repeat);
        return;
    }

    PluginMap.insert(std::make_pair(PluginName, Plugin));
}

void SSFOPluginManager::UnregisterPlugin(SSFPluginErrors &Errors, SSF_PTR_PLUGIN Plugin)
{
    if (!SSF_PTR_VALID(Plugin))
    {
        SSF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Unregister_nullptr);
        return;
    }

    std::string PluginName = Plugin->GetName();
    if (PluginName.empty())
    {
        SSF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Unregister_NameEmpty);
        return;
    }

    auto Iter = PluginMap.find(PluginName);
    if (Iter == PluginMap.end())
    {
        SSF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Unregister_NotFound)
        return;
    }

    PluginMap.erase(Iter);
}

SSF_PTR_PLUGIN SSFOPluginManager::GetPlugin(const std::string &PluginName)
{
    auto it = PluginMap.find(PluginName);
    if (it != PluginMap.end())
    {
        return it->second;
    }

    return nullptr;
}

#pragma endregion SSFIPluginManager

void SSFOPluginManager::LoadPluginConfig(SSFPluginErrors &Errors)
{
    PluginScriptParse = new SKYWALKER_SCRIPT_NAMESPACE::CSkywalkerScriptParse();
    if (!PluginScriptParse->LoadScript("ServerPlugin.skywalkerC"))
    {
        SSF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Load_ConfigNullptr);
        return;
    }

    SKYWALKER_PTR_SCRIPT_NODE RootNode = PluginScriptParse->GetRootNode();
    if (RootNode == nullptr)
    {
        SSF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Load_ConfigNullptr);
        return;
    }

    for (int i = 0; i < RootNode->GetChildNodeNum(); i++)
    {
        SKYWALKER_PTR_SCRIPT_NODE PluginNode = RootNode->GetChildNodeFromIndex(i);
        if (PluginNode == nullptr)
        {
            SSF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Load_ConfigNullptr);
            continue;
        }

        SKYWALKER_PTR_SCRIPT_NODE NameNode = PluginNode->GetChildNodeFromName("Name");
        if (NameNode == nullptr)
        {
            SSF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Load_ConfigNullptr);
            continue;
        }

        PluginNameMap.insert(std::make_pair(NameNode->GetNodeValueString(), true));
    }
}

void SSFOPluginManager::LoadPlugin(SSFPluginErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterName, PluginNameMap)
    {
        const std::string LibraryName = IterName->first;
        SSF_REGISTER_LIBRARY(LibraryName);
    }
}

void SSFOPluginManager::StartPlugin(SSFPluginErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterLib, DynamicLibMap)
    {
        DLL_START_PLUGIN_FUNC DllStartPluginFunc = (DLL_START_PLUGIN_FUNC)IterLib->second->GetSymbol("DllStartPlugin");
        if (DllStartPluginFunc == nullptr)
        {
            SSF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Load_EntryNullptr);
            continue;
        }

        DllStartPluginFunc((SSF_PTR_PLUGIN_MANAGER)(this));
    }
}
