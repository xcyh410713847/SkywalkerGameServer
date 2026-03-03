/*************************************************************************
**文件: SkywalkerFramework\Plugin\Profiler\Module\SFModule_HealthCheck.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 健康检查模块
*************************************************************************/

#include "SFModule_HealthCheck.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_HealthCheck, ESFLogLevel::Debug);

void SFModule_HealthCheck::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
    SF_LOG_FRAMEWORK("HealthCheck module init");
}

void SFModule_HealthCheck::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);
}

void SFModule_HealthCheck::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("HealthCheck module destroy");
    SSFModule::Destroy(Errors);
}

bool SFModule_HealthCheck::IsHealthy() const
{
    return bHealthy == TRUE;
}
