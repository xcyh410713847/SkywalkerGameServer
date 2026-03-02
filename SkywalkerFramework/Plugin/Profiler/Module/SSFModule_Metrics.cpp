/*************************************************************************
**文件: SkywalkerFramework\Plugin\Profiler\Module\SSFModule_Metrics.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 指标采集模块
*************************************************************************/

#include "SSFModule_Metrics.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_Metrics, ESFLogLevel::Debug);

void SSFModule_Metrics::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
    SF_LOG_FRAMEWORK("Metrics module init");
}

void SSFModule_Metrics::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);
    ++TickCount;
}

void SSFModule_Metrics::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("Metrics module destroy, TickCount " << TickCount);
    SSFModule::Destroy(Errors);
}
