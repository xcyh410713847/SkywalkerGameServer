/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SSFService.cpp
**作者: shyfan
**日期: 2024/01/16 21:10:33
**功能: SSFService
*************************************************************************/

#include "SSFService.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFService, ESFLogLevel::Debug);

SSFService::SSFService(SSFServiceContext &InContext, SFObjectErrors &InErrors)
    : SSFObject(InContext, InErrors)
{
}

SSFService::~SSFService()
{
}
