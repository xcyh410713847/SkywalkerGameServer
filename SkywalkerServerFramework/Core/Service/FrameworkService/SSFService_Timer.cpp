/*************************************************************************
**文件: SkywalkerServerFramework\Core\Service\FrameworkService\SSFService_Timer.cpp
**作者: shyfan
**日期: 2024/01/24 19:46:34
**功能: 时间 Service
*************************************************************************/

#include "SSFService_Timer.h"

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFService_Timer, LogLevel_Debug);

SSFService_Timer::SSFService_Timer(SSFServiceContext &InContext, SSFObjectErrors &InErrors)
    : SSFFrameworkService(InContext, InErrors)
{
}

SSFService_Timer::~SSFService_Timer()
{
}

#pragma region Process

bool SSFService_Timer::Init(SSFObjectErrors &InErrors)
{
    SSFFrameworkService::Init(InErrors);

    SkywalkerSFTimer::Reset();

    return true;
}

bool SSFService_Timer::Start(SSFObjectErrors &InErrors)
{
    SSFFrameworkService::Start(InErrors);

    SkywalkerSFTimer::Tick();

    return true;
}

bool SSFService_Timer::Tick(SSFObjectErrors &InErrors)
{
    SSFFrameworkService::Tick(InErrors);

    SkywalkerSFTimer::Tick();

    return true;
}

bool SSFService_Timer::Stop(SSFObjectErrors &InErrors)
{
    SSFFrameworkService::Stop(InErrors);

    return true;
}

void SSFService_Timer::Destroy(SSFObjectErrors &InErrors)
{
    SSFFrameworkService::Destroy(InErrors);
}

#pragma endregion Process
