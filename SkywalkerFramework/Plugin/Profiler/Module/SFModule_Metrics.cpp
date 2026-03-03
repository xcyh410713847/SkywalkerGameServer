/*************************************************************************
**文件: SkywalkerFramework\Plugin\Profiler\Module\SFModule_Metrics.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 指标采集模块
*************************************************************************/

#include "SFModule_Metrics.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_Metrics, ESFLogLevel::Debug);

void SFModule_Metrics::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
    SF_LOG_FRAMEWORK("Metrics module init");
}

void SFModule_Metrics::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);
    ++TickCount;
}

void SFModule_Metrics::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("Metrics module destroy, TickCount " << TickCount);
    SSFModule::Destroy(Errors);
}
