/*************************************************************************
**文件: SkywalkerFramework\Plugin\Game\SFPlugin_Game.cpp
**作者: shyfan
**日期: 2023/09/28 10:15:05
**功能: Game插件
*************************************************************************/

#include "SFPlugin_Game.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SFPluginManager.h"

#include "Module/SFModule_GameFlow.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFPlugin_Game, Framework);

SF_PLUGIN_EXPORT(SFPlugin_Game)

#pragma region SSFPlugin

void SFPlugin_Game::Install(SFObjectErrors &Errors)
{
    SF_REGISTER_MODULE(SFModule_GameFlow)
}

void SFPlugin_Game::Uninstall(SFObjectErrors &Errors)
{
    SF_UNREGISTER_MODULE(SFModule_GameFlow)
}

#pragma endregion SSFPlugin
