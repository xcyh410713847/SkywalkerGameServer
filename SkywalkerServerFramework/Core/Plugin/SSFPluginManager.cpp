/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPluginManager.cpp
**作者: shyfan
**日期: 2023/08/07 23:53:15
**功能: 插件管理器
*************************************************************************/

#include "SSFPluginManager.h"

#include "Include/SSFILog.h"

#include "Core/Plugin/SSFPlugin.h"
#include "Core/Module/SSFModule.h"

SKYWALKER_SF_NAMESPACE_USE

SKYWALKER_SF_LOG_DEFINE(SSFPluginManager, LogLevel_Debug);

SKYWALKER_SINGLETON_IMPLEMENT(SSFOPluginManager);

#pragma region Object

void SSFOPluginManager::Init(SSFObjectErrors &Errors)
{
    SSFPluginErrors PluginErrors;
    LoadPluginConfig(PluginErrors);
    if (PluginErrors.IsValid())
    {
        SKYWALKER_SF_ERROR_DESC(Errors, SkywalkerSFError_Object_Init_Failed, "LoadPluginConfig Failed");
        return;
    }

    LoadPlugin(PluginErrors);
    if (PluginErrors.IsValid())
    {
        SKYWALKER_SF_ERROR_DESC(Errors, SkywalkerSFError_Object_Init_Failed, "LoadPlugin Failed");
        return;
    }

    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Init(Errors);
    }
}

void SSFOPluginManager::Awake(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Awake(Errors);
    }
}

void SSFOPluginManager::Start(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Start(Errors);
    }
}

void SSFOPluginManager::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Tick(Errors, DelayMS);
    }
}

void SSFOPluginManager::Stop(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Stop(Errors);
    }
}

void SSFOPluginManager::Sleep(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Sleep(Errors);
    }
}

void SSFOPluginManager::Destroy(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Destroy(Errors);
    }
}

void SSFOPluginManager::Release(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFOPlugin *)IterPlugin->second)->Release(Errors);
    }

    delete this;
}

#pragma endregion Object

#pragma region SSFIPluginManager

void SSFOPluginManager::RegisterPlugin(SSFPluginErrors &Errors, SKYWALKER_SF_PTR_PLUGIN Plugin)
{
    if (!SKYWALKER_SF_PTR_VALID(Plugin))
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

void SSFOPluginManager::UnregisterPlugin(SSFPluginErrors &Errors, SKYWALKER_SF_PTR_PLUGIN Plugin)
{
    if (!SKYWALKER_SF_PTR_VALID(Plugin))
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

SKYWALKER_SF_PTR_PLUGIN SSFOPluginManager::GetPlugin(const std::string &PluginName)
{
    TMap_Plugin::iterator it = PluginMap.find(PluginName);
    if (it != PluginMap.end())
    {
        return it->second;
    }

    return nullptr;
}

void SSFOPluginManager::RegisterModule(SSFModuleErrors &Errors, SKYWALKER_SF_PTR_PLUGIN Plugin, SKYWALKER_SF_PTR_MODULE Module)
{
    if (!SKYWALKER_SF_PTR_VALID(Plugin))
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Register_nullptr, "Plugin is nullptr");
        return;
    }

    if (!SKYWALKER_SF_PTR_VALID(Module))
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Register_nullptr, "Module is nullptr");
        return;
    }

    std::string PluginName = Plugin->GetName();
    if (PluginName.empty())
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Register_NameEmpty, "PluginName is empty");
        return;
    }

    std::string ModuleName = Module->GetName();
    if (ModuleName.empty())
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Register_NameEmpty, "ModuleName is empty");
        return;
    }

    auto it = ModuleToPluginMap.find(ModuleName);
    if (it != ModuleToPluginMap.end())
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Module_Register_Repeat);
        return;
    }

    ModuleToPluginMap.insert(std::make_pair(ModuleName, PluginName));
}

void SSFOPluginManager::UnregisterModule(SSFModuleErrors &Errors, SKYWALKER_SF_PTR_PLUGIN Plugin, SKYWALKER_SF_PTR_MODULE Module)
{
    if (!SKYWALKER_SF_PTR_VALID(Plugin))
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Unregister_nullptr, "Plugin is nullptr");
        return;
    }

    if (!SKYWALKER_SF_PTR_VALID(Module))
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Unregister_nullptr, "Module is nullptr");
        return;
    }

    std::string PluginName = Plugin->GetName();
    if (PluginName.empty())
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Unregister_NameEmpty, "PluginName is empty");
        return;
    }

    std::string ModuleName = Module->GetName();
    if (ModuleName.empty())
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Unregister_NameEmpty, "ModuleName is empty");
        return;
    }

    auto it = ModuleToPluginMap.find(ModuleName);
    if (it == ModuleToPluginMap.end())
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Module_Unregister_NotFound);
        return;
    }

    ModuleToPluginMap.erase(it);
}

SKYWALKER_SF_PTR_MODULE SSFOPluginManager::GetModule(const std::string &ModuleName)
{
    // TODO Shyfan 临时写的
    return nullptr;
}

#pragma endregion SSFIPluginManager

void SSFOPluginManager::LoadPluginConfig(SSFPluginErrors &Errors)
{
    // TODO Shyfan 临时写的
    PluginNameMap.insert(std::make_pair("SSFPlugin_Network", true));
    PluginNameMap.insert(std::make_pair("SSFPlugin_LaunchState", true));
}

void SSFOPluginManager::LoadPlugin(SSFPluginErrors &Errors)
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

void SSFOPluginManager::LoadPluginLib(SSFPluginErrors &Errors, const std::string &PluginName)
{
    if (DynamicLibMap.find(PluginName) != DynamicLibMap.end())
    {
        SKYWALKER_SF_ERROR_TRACE(Errors, SkywalkerSFError_Plugin_Load_Repeated);
        return;
    }

    SSFDynamicLib *DynamicLib = new SSFDynamicLib(PluginName);
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
