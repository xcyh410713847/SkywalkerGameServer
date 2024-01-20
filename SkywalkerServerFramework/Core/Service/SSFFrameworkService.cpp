/*************************************************************************
**文件: SkywalkerServerFramework\Core\Service\SSFFrameworkService.cpp
**作者: shyfan
**日期: 2024/01/20 15:18:53
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
