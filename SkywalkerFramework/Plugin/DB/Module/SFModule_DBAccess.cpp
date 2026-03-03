/*************************************************************************
**文件: SkywalkerFramework\Plugin\DB\Module\SFModule_DBAccess.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 数据访问模块
*************************************************************************/

#include "SFModule_DBAccess.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_DBAccess, ESFLogLevel::Debug);

void SFModule_DBAccess::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);

    SF_LOG_FRAMEWORK("DBAccess module init");
}

void SFModule_DBAccess::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

    bReady = TRUE;
    SF_LOG_FRAMEWORK("DBAccess module start");
}

void SFModule_DBAccess::Stop(SFObjectErrors &Errors)
{
    bReady = FALSE;
    SF_LOG_FRAMEWORK("DBAccess module stop");

    SSFModule::Stop(Errors);
}

void SFModule_DBAccess::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("DBAccess module destroy");

    SSFModule::Destroy(Errors);
}

bool SFModule_DBAccess::IsReady() const
{
    return bReady == TRUE;
}
