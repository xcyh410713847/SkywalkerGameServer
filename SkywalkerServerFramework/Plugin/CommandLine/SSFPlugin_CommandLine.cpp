﻿/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\CommandLine\SSFPlugin_CommandLine.cpp
**作者: shyfan
**日期: 2024/01/16 11:47:43
**功能: 命令行插件
*************************************************************************/

#include "SSFPlugin_CommandLine.h"

#include "Include/SSFILog.h"

#include "Core/Plugin/SSFPluginManager.h"

#include "Module/SSFModule_OS.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFPlugin_CommandLine, LogLevel_Debug);

SSF_PLUGIN_EXPORT(SSFPlugin_CommandLine)

#pragma region SSFPlugin

void SSFPlugin_CommandLine::Install(SSFObjectErrors &Errors)
{
    SSF_REGISTER_MODULE(SSFModule_OS);
}

void SSFPlugin_CommandLine::Uninstall(SSFObjectErrors &Errors)
{
    SSF_UNREGISTER_MODULE(SSFModule_OS);
}

#pragma endregion SSFPlugin
