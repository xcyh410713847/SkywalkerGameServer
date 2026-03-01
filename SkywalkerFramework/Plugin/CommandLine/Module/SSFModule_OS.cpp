/*************************************************************************
**文件: SkywalkerFramework\Plugin\CommandLine\Module\SSFModule_OS.cpp
**作者: shyfan
**日期: 2024/01/16 14:46:57
**功能: 操作系统模块
*************************************************************************/

#include "SSFModule_OS.h"

#include <csignal>
#include <signal.h>

#include "Include/SFCore.h"
#include "Include/SFILog.h"
#include "Include/SFFramework.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_OS, ESFLogLevel::Debug);

static SF_PTR(SSFModule_OS) SelfModule{};

#pragma region Module

void SSFModule_OS::Init(SFObjectErrors &Errors)
{
    SelfModule = this;

    // 注册系统信号处理函数
    signal(SIGINT, &SSFModule_OS::SignalHandler); // Ctrl+C
}

void SSFModule_OS::Destroy(SFObjectErrors &Errors)
{
    SelfModule = nullptr;

    // 注销系统信号处理函数
    signal(SIGINT, SIG_DFL); // Ctrl+C
}

#pragma endregion Module

SSFModule_OS::SSFModule_OS(SFModuleContext &InContext, SFObjectErrors &InErrors)
    : SSFModule(InContext, InErrors)
{
}

void SSFModule_OS::SignalHandler(int Signal)
{
    SF_LOG_DEBUG("SignalHandler Signal: " << Signal);
    SelfModule->GetFramework()->Close();
}
