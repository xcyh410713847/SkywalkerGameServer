/*************************************************************************
**文件: SkywalkerServerFramework\Core\SSFPluginManager.cpp
**作者: shyfan
**日期: 2023/07/30 12:57:23
**功能: 插件管理器
*************************************************************************/

#include "SSFPluginManager.h"

SKYWALKER_SF_NAMESPACE_USE

SSFCPluginManager::SSFCPluginManager()
{
}

SSFCPluginManager::~SSFCPluginManager()
{
}

#pragma region SSFIPluginManager

void SSFCPluginManager::RegisterPlugin(SSFPluginErrors &Errors, SSFIPlugin *Plugin)
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

void SSFCPluginManager::UnregisterPlugin(SSFPluginErrors &Errors, SSFIPlugin *Plugin)
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

SSFIPlugin *SSFCPluginManager::GetPlugin(const std::string &PluginName)
{
    TMap_Plugin::iterator it = PluginMap.find(PluginName);
    if (it != PluginMap.end())
    {
        return it->second;
    }

    return nullptr;
}

#pragma endregion SSFIPluginManager
