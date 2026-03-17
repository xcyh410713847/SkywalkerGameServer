/*************************************************************************
**文件: SkywalkerFramework\Plugin\CommandLine\SFPlugin_CommandLine.cpp
**作者: shyfan
**日期: 2024/01/16 11:47:43
**功能: 命令行插件
*************************************************************************/

#include "SFPlugin_CommandLine.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SFPluginManager.h"

#include "Module/SFModule_OS.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFPlugin_CommandLine, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SFPlugin_CommandLine)

#pragma region SSFPlugin

void SFPlugin_CommandLine::Install(SFObjectErrors &Errors)
{
    SF_REGISTER_MODULE(SFModule_OS);
}

void SFPlugin_CommandLine::Uninstall(SFObjectErrors &Errors)
{
    SF_UNREGISTER_MODULE(SFModule_OS);
}

#pragma endregion SSFPlugin
