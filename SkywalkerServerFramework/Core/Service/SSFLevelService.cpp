/*************************************************************************
**文件: SkywalkerServerFramework\Core\Service\SSFLevelService.cpp
**作者: shyfan
**日期: 2024/01/22 12:28:39
**功能: SSFLevelService
*************************************************************************/

#include "SSFLevelService.h"

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFLevelService, LogLevel_Debug);

SSFLevelService::SSFLevelService(SSFServiceContext &InContext, SSFObjectErrors &InErrors)
    : SSFService(InContext, InErrors)
{
}

SSFLevelService::~SSFLevelService()
{
}

#pragma region Process

bool SSFLevelService::Init(SSFObjectErrors &InErrors)
{
    SSF_LOG_DEBUG_MODULE("Init");

    return true;
}

bool SSFLevelService::Start(SSFObjectErrors &InErrors)
{
    SSF_LOG_DEBUG_MODULE("Start");

    return true;
}

bool SSFLevelService::Tick(SSFObjectErrors &InErrors)
{
    return true;
}

bool SSFLevelService::Stop(SSFObjectErrors &InErrors)
{
    SSF_LOG_DEBUG_MODULE("Stop");

    return true;
}

void SSFLevelService::Destroy(SSFObjectErrors &InErrors)
{
    SSF_LOG_DEBUG_MODULE("Destroy");
}

#pragma endregion Process
