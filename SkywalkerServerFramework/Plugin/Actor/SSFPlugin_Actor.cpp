/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Actor\SSFPlugin_Actor.cpp
**作者: shyfan
**日期: 2023/09/28 10:15:05
**功能: Actor插件
*************************************************************************/

#include "SSFPlugin_Actor.h"

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SKYWALKER_SF_LOG_DEFINE(SSFPlugin_Actor, LogLevel_Debug);

SSF_PLUGIN_EXPORT(SSFPlugin_Actor)

#pragma region SSFOPlugin

void SSFPlugin_Actor::Install(SSFModuleErrors &Errors)
{
}

void SSFPlugin_Actor::Uninstall(SSFModuleErrors &Errors)
{
}

#pragma endregion SSFOPlugin
