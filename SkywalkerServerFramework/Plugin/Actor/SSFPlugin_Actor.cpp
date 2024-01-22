/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Actor\SSFPlugin_Actor.cpp
**作者: shyfan
**日期: 2023/09/28 10:15:05
**功能: Actor插件
*************************************************************************/

#include "SSFPlugin_Actor.h"

#include "Include/SSFILog.h"

#include "Core/Plugin/SSFPluginManager.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFPlugin_Actor, LogLevel_Debug);

SSF_PLUGIN_EXPORT(SSFPlugin_Actor)

#pragma region SSFPlugin

void SSFPlugin_Actor::Install(SSFObjectErrors &Errors)
{
}

void SSFPlugin_Actor::Uninstall(SSFObjectErrors &Errors)
{
}

#pragma endregion SSFPlugin
