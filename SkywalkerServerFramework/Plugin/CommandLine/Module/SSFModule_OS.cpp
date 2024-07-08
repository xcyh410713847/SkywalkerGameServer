/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\CommandLine\Module\SSFModule_OS.cpp
**作者: shyfan
**日期: 2024/01/16 14:46:57
**功能: 操作系统模块
*************************************************************************/

#include "SSFModule_OS.h"

#include <csignal>
#include <signal.h>

#include "Include/SSFCore.h"
#include "Include/SSFILog.h"
#include "Include/SSFramework.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFModule_OS, LogLevel_Debug);

static SSF_PTR(SSFModule_OS) SelfModule{};

#pragma region Module

void SSFModule_OS::Init(SSFObjectErrors &Errors)
{
    SelfModule = this;

    // 注册系统信号处理函数
    signal(SIGINT, &SSFModule_OS::SignalHandler); // Ctrl+C
}

void SSFModule_OS::Destroy(SSFObjectErrors &Errors)
{
    SelfModule = nullptr;

    // 注销系统信号处理函数
    signal(SIGINT, SIG_DFL); // Ctrl+C
}

#pragma endregion Module

SSFModule_OS::SSFModule_OS(SSFModuleContext &InContext, SSFObjectErrors &InErrors)
    : SSFModule(InContext, InErrors)
{
}

void SSFModule_OS::SignalHandler(int Signal)
{
    SSF_LOG_DEBUG("SignalHandler Signal: " << Signal);
    SelfModule->GetFramework()->Close();
}
