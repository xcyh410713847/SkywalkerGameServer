/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Level\SSFPlugin_Level.cpp
**作者: shyfan
**日期: 2023/09/26 20:51:23
**功能: 关卡插件
*************************************************************************/

#include "SSFPlugin_Level.h"

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SKYWALKER_SF_LOG_DEFINE(SSFPlugin_Level, LogLevel_Debug);

SSF_PLUGIN_EXPORT(SSFPlugin_Level)

#pragma region SSFOPlugin

void SSFPlugin_Level::Install(SSFModuleErrors &Errors)
{
}

void SSFPlugin_Level::Uninstall(SSFModuleErrors &Errors)
{
}

#pragma endregion SSFOPlugin
