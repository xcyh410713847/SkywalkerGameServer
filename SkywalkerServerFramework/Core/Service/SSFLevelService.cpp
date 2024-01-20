/*************************************************************************
**文件: SkywalkerServerFramework\Core\Service\SSFLevelService.cpp
**作者: shyfan
**日期: 2024/01/20 15:23:52
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
