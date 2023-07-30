/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPluginManager.cpp
**作者: shyfan
**日期: 2023/07/30 12:34:18
**功能: 插件管理器
*************************************************************************/

#include "SSFPluginManager.h"

using namespace Skywalker::ServerFramework;

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
        Skywalker::Errors::CPP_ERRORS_WRAP_TRACE(Errors, PluginError_Register_Plugin_nullptr);
        return;
    }

    TMap_Plugin::iterator it = PluginMap.find(Plugin->GetPluginName());
    if (it != PluginMap.end())
    {
        return;
    }

    PluginMap.insert(std::make_pair(Plugin->GetPluginName(), Plugin));

    // TODO 初始化插件资源
}

void SSFCPluginManager::UnregisterPlugin(SSFPluginErrors &Errors, SSFIPlugin *Plugin)
{
    if (Plugin == nullptr)
    {
        return;
    }

    TMap_Plugin::iterator it = PluginMap.find(Plugin->GetPluginName());
    if (it == PluginMap.end())
    {
        return;
    }

    // TODO 释放插件资源
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
