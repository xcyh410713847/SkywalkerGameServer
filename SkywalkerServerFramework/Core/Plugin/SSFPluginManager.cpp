/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPluginManager.cpp
**作者: shyfan
**日期: 2023/08/07 23:53:15
**功能: 插件管理器
*************************************************************************/

#include "SSFPluginManager.h"

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFPluginManager, LogLevel_Debug);

SSFPluginManager::SSFPluginManager(SSFObjectContext &InContext, SSFObjectErrors &InErrors)
    : SSFObjectManager(InContext, InErrors)
{
}

SSFPluginManager::~SSFPluginManager()
{
}

void SSFPluginManager::Release(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        auto IterObject = FindObject(IterPlugin->second);
        if (SSF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SSFPlugin *)IterObject)->Release(Errors);
    }

    PluginMap.clear();
    DynamicLibMap.clear();
    PluginNameMap.clear();

    PluginScriptParse = nullptr;

    // 智能指针不需要释放
    // SSFObjectManager::Release(Errors);
}

#pragma region Process

void SSFPluginManager::Init(SSFObjectErrors &Errors)
{
    SSFObjectErrors PluginErrors;
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
        auto IterObject = FindObject(IterPlugin->second);
        if (SSF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SSFPlugin *)IterObject)->Init(Errors);
    }
}

void SSFPluginManager::Awake(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        auto IterObject = FindObject(IterPlugin->second);
        if (SSF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SSFPlugin *)IterObject)->Awake(Errors);
    }
}

void SSFPluginManager::Start(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        auto IterObject = FindObject(IterPlugin->second);
        if (SSF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SSFPlugin *)IterObject)->Start(Errors);
    }
}

void SSFPluginManager::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SSF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        auto IterObject = FindObject(IterPlugin->second);
        if (SSF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SSFPlugin *)IterObject)->Tick(Errors, DelayMS);
    }
}

void SSFPluginManager::Stop(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        auto IterObject = FindObject(IterPlugin->second);
        if (SSF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SSFPlugin *)IterObject)->Stop(Errors);
    }
}

void SSFPluginManager::Sleep(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        auto IterObject = FindObject(IterPlugin->second);
        if (SSF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SSFPlugin *)IterObject)->Sleep(Errors);
    }
}

void SSFPluginManager::Destroy(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        auto IterObject = FindObject(IterPlugin->second);
        if (SSF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SSFPlugin *)IterObject)->Destroy(Errors);
    }
}

#pragma endregion Process

#pragma region Plugin

void SSFPluginManager::RegisterPlugin(SSFObjectErrors &Errors, SSF_PTR_PLUGIN Plugin)
{
    if (!SSF_PTR_VALID(Plugin))
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Plugin_Register_nullptr, "PluginManager Register Plugin nullptr");
        return;
    }

    std::string PluginName = Plugin->GetObjectClassName();
    if (PluginName.empty())
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Plugin_Register_NameEmpty, "PluginManager Register Plugin NameEmpty");
        return;
    }

    if (PluginMap.find(PluginName) != PluginMap.end())
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Plugin_Register_Repeat, "PluginManager Register Plugin Repeat");
        return;
    }

    AddObject(Errors, Plugin);
    if (Errors.IsValid())
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Plugin_Register_AddObjectFailed, "PluginManager Register Plugin AddObjectFailed");
        return;
    }
    PluginMap.insert(std::make_pair(PluginName, Plugin->GetObjectGUID()));
}

void SSFPluginManager::UnregisterPlugin(SSFObjectErrors &Errors, SSF_PTR_PLUGIN Plugin)
{
    if (!SSF_PTR_VALID(Plugin))
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Plugin_Unregister_nullptr, "PluginManager Unregister Plugin nullptr");
        return;
    }

    std::string PluginName = Plugin->GetObjectClassName();
    if (PluginName.empty())
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Plugin_Unregister_NameEmpty, "PluginManager Unregister Plugin NameEmpty");
        return;
    }

    auto Iter = PluginMap.find(PluginName);
    if (Iter == PluginMap.end())
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Plugin_Unregister_NotFound, "PluginManager Unregister Plugin NotFound");
        return;
    }

    RemoveObject(Errors, Plugin);
    if (Errors.IsValid())
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Plugin_Unregister_RemoveObjectFailed, "PluginManager Unregister Plugin RemoveObjectFailed");
        return;
    }
    PluginMap.erase(Iter);
}

void SSFPluginManager::LoadPluginConfig(SSFObjectErrors &Errors)
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

void SSFPluginManager::LoadPlugin(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterName, PluginNameMap)
    {
        const std::string LibraryName = IterName->first;
        SSF_REGISTER_LIBRARY(LibraryName);
    }
}

void SSFPluginManager::StartPlugin(SSFObjectErrors &Errors)
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

#pragma endregion Plugin
