/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Profiler\SSFPlugin_Profiler.cpp
**作者: shyfan
**日期: 2023/09/25 20:23:25
**功能: 分析器插件
*************************************************************************/

#include "SSFPlugin_Profiler.h"

#include "Include/SSFCore.h"
#include "Include/SSFILog.h"

#include "Core/Plugin/SSFPluginManager.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFPlugin_Profiler, LogLevel_Debug);

SSF_PLUGIN_EXPORT(SSFPlugin_Profiler)

#pragma region SSFOPlugin

void SSFPlugin_Profiler::Install(SSFModuleErrors &Errors)
{
}

void SSFPlugin_Profiler::Uninstall(SSFModuleErrors &Errors)
{
}

#pragma endregion SSFOPlugin
