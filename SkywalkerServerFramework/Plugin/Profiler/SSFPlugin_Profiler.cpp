/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Profiler\SSFPlugin_Profiler.cpp
**作者: shyfan
**日期: 2023/09/25 20:23:25
**功能: 分析器插件
*************************************************************************/

#include "SSFPlugin_Profiler.h"

#include "Include/SSFCore.h"
#include "Include/SSFILog.h"

SKYWALKER_SF_NAMESPACE_USING

SKYWALKER_SF_LOG_DEFINE(SSFPlugin_Profiler, LogLevel_Debug);

// 插件导出
#ifdef SKYWALKER_SF_DYNAMIC_PLUGIN

SKYWALKER_SF_PLUGIN_EXPORT(SSFPlugin_Profiler)

#endif // SKYWALKER_SF_DYNAMIC_PLUGIN

#pragma region SSFOPlugin

void SSFPlugin_Profiler::Install(SSFModuleErrors &Errors)
{
}

void SSFPlugin_Profiler::Uninstall(SSFModuleErrors &Errors)
{
}

#pragma endregion SSFOPlugin
