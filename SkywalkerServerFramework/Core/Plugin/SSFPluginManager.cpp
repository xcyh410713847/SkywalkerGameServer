/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPluginManager.cpp
**作者: shyfan
**日期: 2023/08/07 23:53:15
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

void SSFCPluginManager::RegisterPlugin(SSFPluginErrors &Errors, SSFSharedPtr_IPlugin Plugin)
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

void SSFCPluginManager::UnregisterPlugin(SSFPluginErrors &Errors, SSFSharedPtr_IPlugin Plugin)
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

SSFSharedPtr_IPlugin SSFCPluginManager::GetPlugin(const std::string &PluginName)
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
