/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Video\SSFPlugin_Video.cpp
**作者: shyfan
**日期: 2023/09/25 23:02:15
**功能: 录像插件
*************************************************************************/

#include "SSFPlugin_Video.h"

#include "Include/SSFILog.h"

#include "Core/Plugin/SSFPluginManager.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFPlugin_Video, LogLevel_Debug);

SSF_PLUGIN_EXPORT(SSFPlugin_Video)

#pragma region SSFPlugin

void SSFPlugin_Video::Install(SSFObjectErrors &Errors)
{
}

void SSFPlugin_Video::Uninstall(SSFObjectErrors &Errors)
{
}

#pragma endregion SSFPlugin
