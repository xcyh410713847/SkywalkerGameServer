/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPluginManager.cpp
**作者: shyfan
**日期: 2023/08/07 23:53:15
**功能: 插件管理器
*************************************************************************/

#include "SSFPluginManager.h"

#include "SSFPlugin.h"

SKYWALKER_SF_NAMESPACE_USE

SKYWALKER_SINGLETON_IMPLEMENT(SSFCPluginManager);

#pragma region Object Base Interface

void SSFCPluginManager::Init(SSFObjectErrors &Errors)
{
    SSFPluginErrors PluginErrors;
    LoadPluginConfig(PluginErrors);
    if (PluginErrors.IsValid())
    {
        SKYWALKER_ERRORS_WRAP(Errors, "LoadPluginConfig Failed");
        return;
    }

    LoadPlugin(PluginErrors);
    if (PluginErrors.IsValid())
    {
        SKYWALKER_ERRORS_WRAP(Errors, "LoadPlugin Failed");
        return;
    }

    SKYWALKER_SF_MAP_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Init(Errors);
    }
}

void SSFCPluginManager::Awake(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_MAP_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Awake(Errors);
    }
}

void SSFCPluginManager::Start(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_MAP_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Start(Errors);
    }
}

void SSFCPluginManager::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SKYWALKER_SF_MAP_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Tick(Errors, DelayMS);
    }
}

void SSFCPluginManager::Stop(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_MAP_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Stop(Errors);
    }
}

void SSFCPluginManager::Sleep(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_MAP_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Sleep(Errors);
    }
}

void SSFCPluginManager::Destroy(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_MAP_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Destroy(Errors);
    }
}

void SSFCPluginManager::Release(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_MAP_ITERATOR(IterPlugin, PluginMap)
    {
        ((SSFCPlugin *)IterPlugin->second)->Release(Errors);
    }

    delete this;
}

#pragma endregion Object Base Interface

#pragma region SSFIPluginManager

void SSFCPluginManager::RegisterPlugin(SSFPluginErrors &Errors, SSFPtr_IPlugin Plugin)
{
    if (Plugin == nullptr)
    {
        SKYWALKER_ERRORS_WRAP_TRACE(Errors, PluginError_Register_Plugin_nullptr);
        return;
    }

    std::string PluginName = Plugin->GetPluginName();
    if (PluginName.empty())
    {
        return;
    }

    TMap_Plugin::iterator it = PluginMap.find(PluginName);
    if (it != PluginMap.end())
    {
        return;
    }

    PluginMap.insert(std::make_pair(PluginName, Plugin));
}

void SSFCPluginManager::UnregisterPlugin(SSFPluginErrors &Errors, SSFPtr_IPlugin Plugin)
{
    if (Plugin == nullptr)
    {
        return;
    }

    std::string PluginName = Plugin->GetPluginName();
    if (PluginName.empty())
    {
        return;
    }

    TMap_Plugin::iterator it = PluginMap.find(PluginName);
    if (it == PluginMap.end())
    {
        return;
    }

    PluginMap.erase(it);
}

SSFPtr_IPlugin SSFCPluginManager::GetPlugin(const std::string &PluginName)
{
    TMap_Plugin::iterator it = PluginMap.find(PluginName);
    if (it != PluginMap.end())
    {
        return it->second;
    }

    return nullptr;
}

#pragma endregion SSFIPluginManager

void SSFCPluginManager::LoadPluginConfig(SSFPluginErrors &Errors)
{
    // TODO Shyfan 临时写的
    PluginNameMap.insert(std::make_pair("SSFPlugin_LaunchState", true));
}

void SSFCPluginManager::LoadPlugin(SSFPluginErrors &Errors)
{
    SKYWALKER_SF_MAP_ITERATOR(IterName, PluginNameMap)
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
        SKYWALKER_ERRORS_WRAP(Errors, PluginError_LoadPlugin_Repeat);
        return;
    }

    SSFCDynamicLib *DynamicLib = new SSFCDynamicLib(PluginName);
    if (DynamicLib == nullptr)
    {
        SKYWALKER_ERRORS_WRAP(Errors, PluginError_LoadPlugin_DynamicLib_nullptr);
        return;
    }

    DynamicLibMap.insert(std::make_pair(PluginName, DynamicLib));

    // 加载
    if (!DynamicLib->Load())
    {
        SKYWALKER_ERRORS_WRAP(Errors, PluginError_LoadPlugin_Failed);
        return;
    }

    DLL_START_PLUGIN_FUNC DllStartPluginFunc = (DLL_START_PLUGIN_FUNC)DynamicLib->GetSymbol("DllStartPlugin");
    if (DllStartPluginFunc == nullptr)
    {
        SKYWALKER_ERRORS_WRAP(Errors, PluginError_LoadPlugin_Entry_nullptr);
        return;
    }

    DllStartPluginFunc(SSFPtr_IPluginManager(this));
}
