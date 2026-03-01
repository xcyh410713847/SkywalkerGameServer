/*************************************************************************
**文件: SkywalkerFramework\Plugin\DB\SSFPlugin_DB.cpp
**作者: shyfan
**日期: 2023/09/28 10:47:44
**功能: DB插件
*************************************************************************/

#include "SSFPlugin_DB.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SSFPluginManager.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFPlugin_DB, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SSFPlugin_DB)

#pragma region SSFPlugin

void SSFPlugin_DB::Install(SFObjectErrors &Errors)
{
}

void SSFPlugin_DB::Uninstall(SFObjectErrors &Errors)
{
}

#pragma endregion SSFPlugin
