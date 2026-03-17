/*************************************************************************
**文件: SkywalkerFramework\Plugin\Level\SFPlugin_Level.cpp
**作者: shyfan
**日期: 2023/09/26 20:51:23
**功能: 关卡插件
*************************************************************************/

#include "SFPlugin_Level.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SFPluginManager.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFPlugin_Level, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SFPlugin_Level)

#pragma region SSFPlugin

void SFPlugin_Level::Install(SFObjectErrors &Errors)
{
}

void SFPlugin_Level::Uninstall(SFObjectErrors &Errors)
{
}

#pragma endregion SSFPlugin
