/*************************************************************************
**文件: SkywalkerServerFramework\Core\Service\SSFService.cpp
**作者: shyfan
**日期: 2024/01/16 21:10:33
**功能: SSFService
*************************************************************************/

#include "SSFService.h"

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFService, LogLevel_Debug);

SSFService::SSFService(SSFServiceContext &InContext, SSFObjectErrors &InErrors)
    : SSFObject(InContext, InErrors)
{
}

SSFService::~SSFService()
{
}
