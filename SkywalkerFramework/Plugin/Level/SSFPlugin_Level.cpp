/*************************************************************************
**文件: SkywalkerFramework\Plugin\Level\SSFPlugin_Level.cpp
**作者: shyfan
**日期: 2023/09/26 20:51:23
**功能: 关卡插件
*************************************************************************/

#include "SSFPlugin_Level.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SSFPluginManager.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFPlugin_Level, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SSFPlugin_Level)

#pragma region SSFPlugin

void SSFPlugin_Level::Install(SFObjectErrors &Errors)
{
}

void SSFPlugin_Level::Uninstall(SFObjectErrors &Errors)
{
}

#pragma endregion SSFPlugin
