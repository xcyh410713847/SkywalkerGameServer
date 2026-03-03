/*************************************************************************
**文件: SkywalkerFramework\Plugin\Auth\SFPlugin_Auth.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 认证插件
*************************************************************************/

#include "SFPlugin_Auth.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SFPluginManager.h"

#include "Module/SFModule_Auth.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFPlugin_Auth, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SFPlugin_Auth)

#pragma region SSFPlugin

void SFPlugin_Auth::Install(SFObjectErrors &Errors)
{
    SF_REGISTER_MODULE(SFModule_Auth)
}

void SFPlugin_Auth::Uninstall(SFObjectErrors &Errors)
{
    SF_UNREGISTER_MODULE(SFModule_Auth)
}

#pragma endregion SSFPlugin
