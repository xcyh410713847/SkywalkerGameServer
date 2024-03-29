﻿/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Level\SSFPlugin_Level.cpp
**作者: shyfan
**日期: 2023/09/26 20:51:23
**功能: 关卡插件
*************************************************************************/

#include "SSFPlugin_Level.h"

#include "Include/SSFILog.h"

#include "Core/Plugin/SSFPluginManager.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFPlugin_Level, LogLevel_Debug);

SSF_PLUGIN_EXPORT(SSFPlugin_Level)

#pragma region SSFPlugin

void SSFPlugin_Level::Install(SSFObjectErrors &Errors)
{
}

void SSFPlugin_Level::Uninstall(SSFObjectErrors &Errors)
{
}

#pragma endregion SSFPlugin
