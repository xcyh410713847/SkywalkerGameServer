/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPluginManager.cpp
**作者: shyfan
**日期: 2023/08/07 23:53:15
**功能: 插件管理器
*************************************************************************/

#include "SSFPluginManager.h"

#include "Include\SSFILog.h"

#include "SSFPlugin.h"

SKYWALKER_SF_NAMESPACE_USE

SKYWALKER_SF_LOG_DEFINE(SSFPluginManager, LogLevel_Debug);

SKYWALKER_SINGLETON_IMPLEMENT(SSFCPluginManager);

#pragma region Object Base Interface

void SSFCPluginManager::Init(SSFObjectErrors &Errors)
{
    SSFPluginErrors PluginErrors;
    LoadPluginConfig(PluginErrors);
    if (PluginErrors.IsValid())
    {
        SKYWALKER_SF_ERROR(Errors, "LoadPluginConfig Failed");
        return;
    }

    LoadPlugin(PluginErrors);
    if (PluginErrors.IsValid())
    {
        SKYWALKER_SF_ERROR(Errors, "LoadPlugin Failed");
        return;
    }

    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Init(Errors);
    }
}

void SSFCPluginManager::Awake(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Awake(Errors);
    }
}

void SSFCPluginManager::Start(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Start(Errors);
    }
}

void SSFCPluginManager::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Tick(Errors, DelayMS);
    }
}

void SSFCPluginManager::Stop(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Stop(Errors);
    }
}

void SSFCPluginManager::Sleep(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Sleep(Errors);
    }
}

void SSFCPluginManager::Destroy(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Destroy(Errors);
    }
}

void SSFCPluginManager::Release(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Release(Errors);
    }

    delete this;
}

#pragma endregion Object Base Interface

#pragma region SSFIPluginManager

void SSFCPluginManager::RegisterPlugin(SSFPluginErrors &Errors, SSF_PTR_PLUGIN Plugin)
{
    if (Plugin == nullptr)
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Register_nullptr);
        return;
    }

    std::string PluginName = Plugin->GetName();
    if (PluginName.empty())
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Register_NameEmpty);
        return;
    }

    TMap_Plugin::iterator it = PluginMap.find(PluginName);
    if (it != PluginMap.end())
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Register_Repeat);
        return;
    }

    PluginMap.insert(std::make_pair(PluginName, Plugin));
}

void SSFCPluginManager::UnregisterPlugin(SSFPluginErrors &Errors, SSF_PTR_PLUGIN Plugin)
{
    if (Plugin == nullptr)
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Unregister_nullptr);
        return;
    }

    std::string PluginName = Plugin->GetName();
    if (PluginName.empty())
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Unregister_NameEmpty);
        return;
    }

    TMap_Plugin::iterator it = PluginMap.find(PluginName);
    if (it == PluginMap.end())
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Unregister_NotFound)
        return;
    }

    PluginMap.erase(it);
}

SSF_PTR_PLUGIN SSFCPluginManager::GetPlugin(const std::string &PluginName)
{
    TMap_Plugin::iterator it = PluginMap.find(PluginName);
    if (it != PluginMap.end())
    {
        return it->second;
    }

    return nullptr;
}

void SSFCPluginManager::RegisterModule(SSFModuleErrors &Errors, SSF_PTR_MODULE Module)
{
    if (!SKYWALKER_SF_PTR_VALID(Module))
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Module_Register_nullptr)
        return;
    }

    std::string ModuleName = Module->GetName();
    if (ModuleName.empty())
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Module_Register_NameEmpty);
        return;
    }

    TMap_Module::iterator it = ModuleMap.find(ModuleName);
    if (it != ModuleMap.end())
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Module_Register_Repeat);
        return;
    }

    ModuleNameMap.insert(std::make_pair(ModuleName, true));

    ModuleMap.insert(std::make_pair(ModuleName, Module));
}

void SSFCPluginManager::UnregisterModule(SSFModuleErrors &Errors, SSF_PTR_MODULE Module)
{
    if (!SKYWALKER_SF_PTR_VALID(Module))
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Module_Unregister_nullptr)
        return;
    }

    std::string ModuleName = Module->GetName();
    if (ModuleName.empty())
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Module_Unregister_NameEmpty);
        return;
    }

    TMap_Module::iterator it = ModuleMap.find(ModuleName);
    if (it == ModuleMap.end())
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Module_Unregister_NotFound);
        return;
    }

    ModuleNameMap.erase(ModuleName);

    ModuleMap.erase(it);
}

SSF_PTR_MODULE SSFCPluginManager::GetModule(const std::string &ModuleName)
{
    TMap_Module::iterator it = ModuleMap.find(ModuleName);
    if (it != ModuleMap.end())
    {
        return it->second;
    }

    return nullptr;
}

#pragma endregion SSFIPluginManager

void SSFCPluginManager::LoadPluginConfig(SSFPluginErrors &Errors)
{
    // TODO Shyfan 临时写的
    PluginNameMap.insert(std::make_pair("SSFPlugin_Network", true));
    PluginNameMap.insert(std::make_pair("SSFPlugin_LaunchState", true));
}

void SSFCPluginManager::LoadPlugin(SSFPluginErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterName, PluginNameMap)
    {
        if (IterName->second)
        {
            LoadPluginLib(Errors, IterName->first);
            if (Errors.IsValid())
            {
                return;
            }
        }
    }
}

void SSFCPluginManager::LoadPluginLib(SSFPluginErrors &Errors, const std::string &PluginName)
{
    if (DynamicLibMap.find(PluginName) != DynamicLibMap.end())
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Load_Repeated);
        return;
    }

    SSFCDynamicLib *DynamicLib = new SSFCDynamicLib(PluginName);
    if (DynamicLib == nullptr)
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Load_DynamicLibNullptr);
        return;
    }

    DynamicLibMap.insert(std::make_pair(PluginName, DynamicLib));

    // 加载
    if (!DynamicLib->Load())
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Load_Failed);
        return;
    }

    DLL_START_PLUGIN_FUNC DllStartPluginFunc = (DLL_START_PLUGIN_FUNC)DynamicLib->GetSymbol("DllStartPlugin");
    if (DllStartPluginFunc == nullptr)
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Load_EntryNullptr);
        return;
    }

    DllStartPluginFunc((SKYWALKER_SF_PTR_PLUGIN_MANAGER)(this));

    SKYWALKER_SF_LOG_INFO("LoadPluginLib " << PluginName << " Success");
}
