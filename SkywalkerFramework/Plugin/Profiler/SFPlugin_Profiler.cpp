/*************************************************************************
**文件: SkywalkerFramework\Plugin\Profiler\SFPlugin_Profiler.cpp
**作者: shyfan
**日期: 2023/09/25 20:23:25
**功能: 分析器插件
*************************************************************************/

#include "SFPlugin_Profiler.h"

#include "Include/SFCore.h"
#include "Include/SFILog.h"

#include "Core/Plugin/SFPluginManager.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFPlugin_Profiler, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SFPlugin_Profiler)

#pragma region SSFPlugin

void SFPlugin_Profiler::Install(SFObjectErrors &Errors)
{
}

void SFPlugin_Profiler::Uninstall(SFObjectErrors &Errors)
{
}

#pragma endregion SSFPlugin
