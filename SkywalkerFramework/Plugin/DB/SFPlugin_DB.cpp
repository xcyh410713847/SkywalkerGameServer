/*************************************************************************
**文件: SkywalkerFramework\Plugin\DB\SFPlugin_DB.cpp
**作者: shyfan
**日期: 2023/09/28 10:47:44
**功能: DB插件
*************************************************************************/

#include "SFPlugin_DB.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SFPluginManager.h"

#include "Module/SFModule_DBAccess.h"
#include "Module/SFModule_PlayerRepository.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFPlugin_DB, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SFPlugin_DB)

#pragma region SSFPlugin

void SFPlugin_DB::Install(SFObjectErrors &Errors)
{
    SF_REGISTER_MODULE(SFModule_DBAccess)
    SF_REGISTER_MODULE(SFModule_PlayerRepository)
}

void SFPlugin_DB::Uninstall(SFObjectErrors &Errors)
{
    SF_UNREGISTER_MODULE(SFModule_DBAccess)
    SF_UNREGISTER_MODULE(SFModule_PlayerRepository)
}

#pragma endregion SSFPlugin
