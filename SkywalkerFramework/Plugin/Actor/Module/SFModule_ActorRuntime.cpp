/*************************************************************************
**文件: SkywalkerFramework\Plugin\Actor\Module\SFModule_ActorRuntime.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: Actor运行时模块
*************************************************************************/

#include "SFModule_ActorRuntime.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_ActorRuntime, ESFLogLevel::Debug);

void SFModule_ActorRuntime::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
    SF_LOG_FRAMEWORK("ActorRuntime module init");
}

void SFModule_ActorRuntime::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);
    SF_LOG_FRAMEWORK("ActorRuntime module start");
}

void SFModule_ActorRuntime::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);
}

void SFModule_ActorRuntime::Stop(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("ActorRuntime module stop");
    SSFModule::Stop(Errors);
}

void SFModule_ActorRuntime::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("ActorRuntime module destroy");
    SSFModule::Destroy(Errors);
}
