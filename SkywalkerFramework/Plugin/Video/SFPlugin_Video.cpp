/*************************************************************************
**文件: SkywalkerFramework\Plugin\Video\SFPlugin_Video.cpp
**作者: shyfan
**日期: 2023/09/25 23:02:15
**功能: 录像插件
*************************************************************************/

#include "SFPlugin_Video.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SFPluginManager.h"

#include "Module/SFModule_ReplayRecorder.h"
#include "Module/SFModule_ReplayPlayer.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFPlugin_Video, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SFPlugin_Video)

#pragma region SSFPlugin

void SFPlugin_Video::Install(SFObjectErrors &Errors)
{
    SF_REGISTER_MODULE(SFModule_ReplayRecorder)
    SF_REGISTER_MODULE(SFModule_ReplayPlayer)
}

void SFPlugin_Video::Uninstall(SFObjectErrors &Errors)
{
    SF_UNREGISTER_MODULE(SFModule_ReplayRecorder)
    SF_UNREGISTER_MODULE(SFModule_ReplayPlayer)
}

#pragma endregion SSFPlugin
