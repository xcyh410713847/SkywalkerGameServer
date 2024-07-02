/*************************************************************************
**文件: SkywalkerServerFramework\Core\Service\SSFFrameworkService.cpp
**作者: shyfan
**日期: 2024/01/22 12:29:48
**功能: SSFFrameworkService
*************************************************************************/

#include "SSFFrameworkService.h"

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFFrameworkService, LogLevel_Debug);

SSFFrameworkService::SSFFrameworkService(SSFServiceContext &InContext, SSFObjectErrors &InErrors)
    : SSFService(InContext, InErrors)
{
}

SSFFrameworkService::~SSFFrameworkService()
{
}

#pragma region Process

bool SSFFrameworkService::Init(SSFObjectErrors &InErrors)
{
    SSF_LOG_DEBUG_MODULE("Init");

    return true;
}

bool SSFFrameworkService::Start(SSFObjectErrors &InErrors)
{
    SSF_LOG_DEBUG_MODULE("Start");

    return true;
}

bool SSFFrameworkService::Tick(SSFObjectErrors &InErrors)
{
    return true;
}

bool SSFFrameworkService::Stop(SSFObjectErrors &InErrors)
{
    SSF_LOG_DEBUG_MODULE("Stop");

    return true;
}

void SSFFrameworkService::Destroy(SSFObjectErrors &InErrors)
{
    SSF_LOG_DEBUG_MODULE("Destroy");
}

#pragma endregion Process
