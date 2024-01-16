/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\CommandLine\Module\SSFModule_OS.cpp
**作者: shyfan
**日期: 2024/01/16 14:46:57
**功能: 操作系统模块
*************************************************************************/

#include "SSFModule_OS.h"

#include "Include/SSFCore.h"
#include "Include/SSFILog.h"
#include "Include/SSFramework.h"

#include "Core/Plugin/SSFPluginManager.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFModule_OS, LogLevel_Debug);

static SSF_SHARED_PTR(SSFModule_OS) SelfModule;

SSFModule_OS::SSFModule_OS(SSF_PTR_PLUGIN_MANAGER InPluginManager)
    : SSFOModule(InPluginManager)
{
    SelfModule.reset(this);
    // Ctrl+C
    signal(SIGINT, &SSFModule_OS::SignalHandler);
}

SSFModule_OS::~SSFModule_OS()
{
}

void SSFModule_OS::SignalHandler(int Signal)
{
    SSF_LOG_DEBUG("SignalHandler Signal: " << Signal);
    SelfModule->GetFramework()->Close();
}
