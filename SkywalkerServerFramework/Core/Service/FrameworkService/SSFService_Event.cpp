/*************************************************************************
**文件: SkywalkerServerFramework\Core\Service\FrameworkService\SSFService_Event.cpp
**作者: shyfan
**日期: 2024/01/22 16:06:00
**功能: SSFService_Event
*************************************************************************/

#include "SSFService_Event.h"

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFService_Event, LogLevel_Debug);

SSFService_Event::SSFService_Event(SSFServiceContext &InContext, SSFObjectErrors &InErrors)
    : SSFFrameworkService(InContext, InErrors)
{
}

SSFService_Event::~SSFService_Event()
{
}

#pragma region Process

bool SSFService_Event::Init(SSFObjectErrors &InErrors)
{
    SSFFrameworkService::Init(InErrors);

    return true;
}

bool SSFService_Event::Start(SSFObjectErrors &InErrors)
{
    SSFFrameworkService::Start(InErrors);

    return true;
}

bool SSFService_Event::Tick(SSFObjectErrors &InErrors)
{
    SSFFrameworkService::Tick(InErrors);

    return true;
}

bool SSFService_Event::Stop(SSFObjectErrors &InErrors)
{
    SSFFrameworkService::Stop(InErrors);

    return true;
}

void SSFService_Event::Destroy(SSFObjectErrors &InErrors)
{
    SSFFrameworkService::Destroy(InErrors);
}

#pragma endregion Process
