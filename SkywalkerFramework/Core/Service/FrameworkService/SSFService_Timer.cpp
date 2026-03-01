/*************************************************************************
**文件: SkywalkerFramework\Core\Service\FrameworkService\SSFService_Timer.cpp
**作者: shyfan
**日期: 2024/01/24 19:46:34
**功能: 时间 Service
*************************************************************************/

#include "SSFService_Timer.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFService_Timer, ESFLogLevel::Debug);

SSFService_Timer::SSFService_Timer(SSFServiceContext &InContext, SFObjectErrors &InErrors)
    : SSFFrameworkService(InContext, InErrors)
{
}

SSFService_Timer::~SSFService_Timer()
{
}

#pragma region Process

bool SSFService_Timer::Init(SFObjectErrors &InErrors)
{
    SSFFrameworkService::Init(InErrors);

    SkywalkerSFTimer::Reset();

    return true;
}

bool SSFService_Timer::Start(SFObjectErrors &InErrors)
{
    SSFFrameworkService::Start(InErrors);

    SkywalkerSFTimer::Tick();

    return true;
}

bool SSFService_Timer::Tick(SFObjectErrors &InErrors)
{
    SSFFrameworkService::Tick(InErrors);

    SkywalkerSFTimer::Tick();

    return true;
}

bool SSFService_Timer::Stop(SFObjectErrors &InErrors)
{
    SSFFrameworkService::Stop(InErrors);

    return true;
}

void SSFService_Timer::Destroy(SFObjectErrors &InErrors)
{
    SSFFrameworkService::Destroy(InErrors);
}

#pragma endregion Process
