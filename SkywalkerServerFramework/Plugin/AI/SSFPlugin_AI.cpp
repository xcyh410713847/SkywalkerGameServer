/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\AI\SSFPlugin_AI.cpp
**作者: shyfan
**日期: 2023/10/07 20:20:48
**功能: AI插件
*************************************************************************/

#include "SSFPlugin_AI.h"

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SKYWALKER_SF_LOG_DEFINE(SSFPlugin_AI, LogLevel_Debug);

SSF_PLUGIN_EXPORT(SSFPlugin_AI)

#pragma region SSFOPlugin

void SSFPlugin_AI::Install(SSFModuleErrors &Errors)
{
}

void SSFPlugin_AI::Uninstall(SSFModuleErrors &Errors)
{
}

#pragma endregion SSFOPlugin
