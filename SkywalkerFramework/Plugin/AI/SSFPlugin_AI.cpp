/*************************************************************************
**文件: SkywalkerFramework\Plugin\AI\SSFPlugin_AI.cpp
**作者: shyfan
**日期: 2023/10/07 20:20:48
**功能: AI插件
*************************************************************************/

#include "SSFPlugin_AI.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SSFPluginManager.h"

#include "Module/SSFModule_AIRuntime.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFPlugin_AI, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SSFPlugin_AI)

#pragma region SSFPlugin

void SSFPlugin_AI::Install(SFObjectErrors &Errors)
{
	SF_REGISTER_MODULE(SSFModule_AIRuntime)
}

void SSFPlugin_AI::Uninstall(SFObjectErrors &Errors)
{
	SF_UNREGISTER_MODULE(SSFModule_AIRuntime)
}

#pragma endregion SSFPlugin
