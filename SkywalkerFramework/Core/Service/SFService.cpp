/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SFService.cpp
**作者: shyfan
**日期: 2024/01/16 21:10:33
**功能: SSFService
*************************************************************************/

#include "SFService.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFService, ESFLogLevel::Debug);

/** 构造函数 */
SSFService::SSFService(SSFServiceContext &InContext, SFObjectErrors &InErrors)
    : SSFObject(InContext, InErrors)
{
}

/** 析构函数 */
SSFService::~SSFService()
{
}
