/*************************************************************************
**文件: SkywalkerFramework\Plugin\CommandLine\SSFPlugin_CommandLine.cpp
**作者: shyfan
**日期: 2024/01/16 11:47:43
**功能: 命令行插件
*************************************************************************/

#include "SSFPlugin_CommandLine.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SSFPluginManager.h"

#include "Module/SSFModule_OS.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFPlugin_CommandLine, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SSFPlugin_CommandLine)

#pragma region SSFPlugin

void SSFPlugin_CommandLine::Install(SFObjectErrors &Errors)
{
    SF_REGISTER_MODULE(SSFModule_OS);
}

void SSFPlugin_CommandLine::Uninstall(SFObjectErrors &Errors)
{
    SF_UNREGISTER_MODULE(SSFModule_OS);
}

#pragma endregion SSFPlugin
