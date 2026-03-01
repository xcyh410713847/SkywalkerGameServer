/*************************************************************************
**文件: SkywalkerFramework\Plugin\Profiler\SSFPlugin_Profiler.cpp
**作者: shyfan
**日期: 2023/09/25 20:23:25
**功能: 分析器插件
*************************************************************************/

#include "SSFPlugin_Profiler.h"

#include "Include/SFCore.h"
#include "Include/SFILog.h"

#include "Core/Plugin/SSFPluginManager.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFPlugin_Profiler, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SSFPlugin_Profiler)

#pragma region SSFPlugin

void SSFPlugin_Profiler::Install(SFObjectErrors &Errors)
{
}

void SSFPlugin_Profiler::Uninstall(SFObjectErrors &Errors)
{
}

#pragma endregion SSFPlugin
