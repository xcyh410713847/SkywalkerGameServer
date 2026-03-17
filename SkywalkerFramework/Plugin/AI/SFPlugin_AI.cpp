/*************************************************************************
**文件: SkywalkerFramework\Plugin\AI\SFPlugin_AI.cpp
**作者: shyfan
**日期: 2023/10/07 20:20:48
**功能: AI插件
*************************************************************************/
#include "SFPlugin_AI.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SFPluginManager.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFPlugin_AI, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SFPlugin_AI)

#pragma region SSFPlugin

void SFPlugin_AI::Install(SFObjectErrors &Errors)
{
}

void SFPlugin_AI::Uninstall(SFObjectErrors &Errors)
{
}

#pragma endregion SSFPlugin
