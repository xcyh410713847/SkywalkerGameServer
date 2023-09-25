/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Video\SSFPlugin_Video.cpp
**作者: shyfan
**日期: 2023/09/25 23:02:15
**功能: 录像插件
*************************************************************************/

#include "SSFPlugin_Video.h"

#include "Include/SSFILog.h"

SKYWALKER_SF_NAMESPACE_USING

SKYWALKER_SF_LOG_DEFINE(SSFPlugin_Video, LogLevel_Debug);

SKYWALKER_SF_PLUGIN_EXPORT(SSFPlugin_Video)

#pragma region SSFOPlugin

void SSFPlugin_Video::Install(SSFModuleErrors &Errors)
{
}

void SSFPlugin_Video::Uninstall(SSFModuleErrors &Errors)
{
}

#pragma endregion SSFOPlugin
