/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPluginManager.cpp
**作者: shyfan
**日期: 2023/08/07 23:53:15
**功能: 插件管理器
*************************************************************************/

#include "SSFPluginManager.h"

SKYWALKER_SF_NAMESPACE_USE

SKYWALKER_SINGLETON_IMPLEMENT(SSFCPluginManager);

#pragma region SSFIPluginManager

void SSFCPluginManager::LoadPluginConfig(SSFPluginErrors &Errors)
{
    // TODO Shyfan 临时写的
    PluginNameMap.insert(std::make_pair("SSFPlugin_LaunchState", true));
}

void SSFCPluginManager::LoadPlugin(SSFPluginErrors &Errors)
{
    for (TMap_PluginName::iterator it = PluginNameMap.begin(); it != PluginNameMap.end(); ++it)
    {
        if (it->second)
        {
            LoadPluginLib(it->first);
        }
    }
}

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

void SSFCPluginManager::Tick()
{
    // TODO Shyfan 插件管理器Tick
}

void SSFCPluginManager::Stop()
{
    // TODO Shyfan 插件管理器Stop
}

void SSFCPluginManager::Release()
{
    delete this;
}

#pragma endregion SSFIPluginManager

void SSFCPluginManager::LoadPluginLib(const std::string &PluginName)
{
    if (DynamicLibMap.find(PluginName) != DynamicLibMap.end())
    {
        return;
    }

    SSFCDynamicLib *DynamicLib = new SSFCDynamicLib(PluginName);
    if (DynamicLib == nullptr)
    {
        return;
    }

    DynamicLibMap.insert(std::make_pair(PluginName, DynamicLib));

    // 加载
    DynamicLib->Load();

    DLL_START_PLUGIN_FUNC DllStartPluginFunc = (DLL_START_PLUGIN_FUNC)DynamicLib->GetSymbol("DllStartPlugin");
    if (DllStartPluginFunc == nullptr)
    {
        return;
    }

    DllStartPluginFunc(SSFPtr_IPluginManager(this));
}
