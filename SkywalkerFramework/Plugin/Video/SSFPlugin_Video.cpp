/*************************************************************************
**文件: SkywalkerFramework\Plugin\Video\SSFPlugin_Video.cpp
**作者: shyfan
**日期: 2023/09/25 23:02:15
**功能: 录像插件
*************************************************************************/

#include "SSFPlugin_Video.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SSFPluginManager.h"

#include "Module/SSFModule_ReplayRecorder.h"
#include "Module/SSFModule_ReplayPlayer.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFPlugin_Video, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SSFPlugin_Video)

#pragma region SSFPlugin

void SSFPlugin_Video::Install(SFObjectErrors &Errors)
{
	SF_REGISTER_MODULE(SSFModule_ReplayRecorder)
	SF_REGISTER_MODULE(SSFModule_ReplayPlayer)
}

void SSFPlugin_Video::Uninstall(SFObjectErrors &Errors)
{
	SF_UNREGISTER_MODULE(SSFModule_ReplayRecorder)
	SF_UNREGISTER_MODULE(SSFModule_ReplayPlayer)
}

#pragma endregion SSFPlugin
