/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SSFFrameworkService.cpp
**作者: shyfan
**日期: 2024/01/22 12:29:48
**功能: SSFFrameworkService
*************************************************************************/

#include "SSFFrameworkService.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFFrameworkService, ESFLogLevel::Debug);

SSFFrameworkService::SSFFrameworkService(SSFServiceContext &InContext, SFObjectErrors &InErrors)
    : SSFService(InContext, InErrors)
{
}

SSFFrameworkService::~SSFFrameworkService()
{
}

#pragma region Process

bool SSFFrameworkService::Init(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Init");

    return true;
}

bool SSFFrameworkService::Start(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Start");

    return true;
}

bool SSFFrameworkService::Tick(SFObjectErrors &InErrors)
{
    return true;
}

bool SSFFrameworkService::Stop(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Stop");

    return true;
}

void SSFFrameworkService::Destroy(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Destroy");
}

#pragma endregion Process
