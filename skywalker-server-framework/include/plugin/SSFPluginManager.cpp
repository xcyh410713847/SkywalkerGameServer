/*************************************************************************
**文件: SSFPluginManager.cpp
**作者: shyfan
**日期: 2023/07/29 16:01:44
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
