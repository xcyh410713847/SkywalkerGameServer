/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\DB\SSFPlugin_DB.cpp
**作者: shyfan
**日期: 2023/09/28 10:47:44
**功能: DB插件
*************************************************************************/

#include "SSFPlugin_DB.h"

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFPlugin_DB, LogLevel_Debug);

SSF_PLUGIN_EXPORT(SSFPlugin_DB)

#pragma region SSFOPlugin

void SSFPlugin_DB::Install(SSFModuleErrors &Errors)
{
}

void SSFPlugin_DB::Uninstall(SSFModuleErrors &Errors)
{
}

#pragma endregion SSFOPlugin
