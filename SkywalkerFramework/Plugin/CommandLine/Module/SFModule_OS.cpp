/*************************************************************************
**文件: SkywalkerFramework\Plugin\CommandLine\Module\SFModule_OS.cpp
**作者: shyfan
**日期: 2024/01/16 14:46:57
**功能: 操作系统模块
*************************************************************************/

#include "SFModule_OS.h"

#include <csignal>
#include <signal.h>

#include "Include/SFCore.h"
#include "Include/SFILog.h"
#include "Include/SFFramework.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_OS, ESFLogLevel::Debug);

static SF_PTR(SFModule_OS) SelfModule{};

#pragma region Module

void SFModule_OS::Init(SFObjectErrors &Errors)
{
    SelfModule = this;

    // 注册系统信号处理函数
    signal(SIGINT, &SFModule_OS::SignalHandler); // Ctrl+C
}

void SFModule_OS::Destroy(SFObjectErrors &Errors)
{
    SelfModule = nullptr;

    // 注销系统信号处理函数
    signal(SIGINT, SIG_DFL); // Ctrl+C
}

#pragma endregion Module

SFModule_OS::SFModule_OS(SFModuleContext &InContext, SFObjectErrors &InErrors)
    : SSFModule(InContext, InErrors)
{
}

void SFModule_OS::SignalHandler(int Signal)
{
    SF_LOG_DEBUG("SignalHandler Signal: " << Signal);
    SelfModule->GetFramework()->Close();
}
