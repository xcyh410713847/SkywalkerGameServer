/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SFLevelService.cpp
**作者: shyfan
**日期: 2024/01/22 12:28:39
**功能: SSFLevelService
*************************************************************************/

#include "SFLevelService.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFLevelService, ESFLogLevel::Debug);

SSFLevelService::SSFLevelService(SSFServiceContext &InContext, SFObjectErrors &InErrors)
    : SSFService(InContext, InErrors)
{
}

SSFLevelService::~SSFLevelService()
{
}

#pragma region Process

bool SSFLevelService::Init(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Init");

    return true;
}

bool SSFLevelService::Start(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Start");

    return true;
}

bool SSFLevelService::Tick(SFObjectErrors &InErrors)
{
    return true;
}

bool SSFLevelService::Stop(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Stop");

    return true;
}

void SSFLevelService::Destroy(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Destroy");
}

#pragma endregion Process
