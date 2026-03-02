/*************************************************************************
**文件: SkywalkerFramework\Plugin\AI\Module\SSFModule_AIRuntime.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: AI运行时模块
*************************************************************************/

#include "SSFModule_AIRuntime.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_AIRuntime, ESFLogLevel::Debug);

void SSFModule_AIRuntime::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
    SF_LOG_FRAMEWORK("AIRuntime module init, TickBudgetMS " << TickBudgetMS);
}

void SSFModule_AIRuntime::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);
    ++TickCounter;
}

void SSFModule_AIRuntime::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("AIRuntime module destroy, TickCounter " << TickCounter);
    SSFModule::Destroy(Errors);
}

void SSFModule_AIRuntime::SetTickBudgetMS(SFUInt64 InTickBudgetMS)
{
    TickBudgetMS = InTickBudgetMS;
}

SFUInt64 SSFModule_AIRuntime::GetTickBudgetMS() const
{
    return TickBudgetMS;
}
