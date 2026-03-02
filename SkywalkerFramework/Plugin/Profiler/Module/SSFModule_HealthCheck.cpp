/*************************************************************************
**文件: SkywalkerFramework\Plugin\Profiler\Module\SSFModule_HealthCheck.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 健康检查模块
*************************************************************************/

#include "SSFModule_HealthCheck.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_HealthCheck, ESFLogLevel::Debug);

void SSFModule_HealthCheck::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
    SF_LOG_FRAMEWORK("HealthCheck module init");
}

void SSFModule_HealthCheck::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);
}

void SSFModule_HealthCheck::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("HealthCheck module destroy");
    SSFModule::Destroy(Errors);
}

bool SSFModule_HealthCheck::IsHealthy() const
{
    return bHealthy == TRUE;
}
