/*************************************************************************
**文件: SkywalkerFramework\Plugin\Actor\SSFPlugin_Actor.cpp
**作者: shyfan
**日期: 2023/09/28 10:15:05
**功能: Actor插件
*************************************************************************/

#include "SSFPlugin_Actor.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SSFPluginManager.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFPlugin_Actor, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SSFPlugin_Actor)

#pragma region SSFPlugin

void SSFPlugin_Actor::Install(SFObjectErrors &Errors)
{
}

void SSFPlugin_Actor::Uninstall(SFObjectErrors &Errors)
{
}

#pragma endregion SSFPlugin
