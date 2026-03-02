/*************************************************************************
**文件: SkywalkerFramework\Plugin\Auth\SSFPlugin_Auth.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 认证插件
*************************************************************************/

#include "SSFPlugin_Auth.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SSFPluginManager.h"

#include "Module/SSFModule_Auth.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFPlugin_Auth, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SSFPlugin_Auth)

#pragma region SSFPlugin

void SSFPlugin_Auth::Install(SFObjectErrors &Errors)
{
    SF_REGISTER_MODULE(SSFModule_Auth)
}

void SSFPlugin_Auth::Uninstall(SFObjectErrors &Errors)
{
    SF_UNREGISTER_MODULE(SSFModule_Auth)
}

#pragma endregion SSFPlugin
