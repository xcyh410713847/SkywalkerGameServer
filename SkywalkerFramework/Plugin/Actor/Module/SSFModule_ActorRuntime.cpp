/*************************************************************************
**文件: SkywalkerFramework\Plugin\Actor\Module\SSFModule_ActorRuntime.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: Actor运行时模块
*************************************************************************/

#include "SSFModule_ActorRuntime.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_ActorRuntime, ESFLogLevel::Debug);

void SSFModule_ActorRuntime::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
    SF_LOG_FRAMEWORK("ActorRuntime module init");
}

void SSFModule_ActorRuntime::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);
    SF_LOG_FRAMEWORK("ActorRuntime module start");
}

void SSFModule_ActorRuntime::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);
}

void SSFModule_ActorRuntime::Stop(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("ActorRuntime module stop");
    SSFModule::Stop(Errors);
}

void SSFModule_ActorRuntime::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("ActorRuntime module destroy");
    SSFModule::Destroy(Errors);
}
