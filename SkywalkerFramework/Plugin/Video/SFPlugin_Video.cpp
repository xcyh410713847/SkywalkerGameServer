/*************************************************************************
**文件: SkywalkerFramework\Plugin\Video\SFPlugin_Video.cpp
**作者: shyfan
**日期: 2023/09/25 23:02:15
**功能: 录像插件
*************************************************************************/

#include "SFPlugin_Video.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SFPluginManager.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFPlugin_Video, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SFPlugin_Video)

#pragma region SSFPlugin

void SFPlugin_Video::Install(SFObjectErrors &Errors)
{
}

void SFPlugin_Video::Uninstall(SFObjectErrors &Errors)
{
}

#pragma endregion SSFPlugin
