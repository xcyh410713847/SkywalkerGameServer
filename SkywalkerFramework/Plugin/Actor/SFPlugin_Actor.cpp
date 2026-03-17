/*************************************************************************
**文件: SkywalkerFramework\Plugin\Actor\SFPlugin_Actor.cpp
**作者: shyfan
**日期: 2023/09/28 10:15:05
**功能: Actor插件
*************************************************************************/

#include "SFPlugin_Actor.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SFPluginManager.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFPlugin_Actor, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SFPlugin_Actor)

#pragma region SSFPlugin

void SFPlugin_Actor::Install(SFObjectErrors &Errors)
{
}

void SFPlugin_Actor::Uninstall(SFObjectErrors &Errors)
{
}

#pragma endregion SSFPlugin
