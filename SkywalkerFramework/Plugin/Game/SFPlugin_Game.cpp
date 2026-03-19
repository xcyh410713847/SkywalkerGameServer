/*************************************************************************
**文件: SkywalkerFramework\Plugin\Game\SFPlugin_Game.cpp
**作者: shyfan
**日期: 2023/09/28 10:15:05
**功能: Game插件
*************************************************************************/

#include "SFPlugin_Game.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SFPluginManager.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFPlugin_Game, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SFPlugin_Game)

#pragma region SSFPlugin

void SFPlugin_Game::Install(SFObjectErrors &Errors)
{
}

void SFPlugin_Game::Uninstall(SFObjectErrors &Errors)
{
}

#pragma endregion SSFPlugin
