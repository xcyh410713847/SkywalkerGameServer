/*************************************************************************
**文件: SkywalkerServerFramework\Core\Object\SSFObjectSimple.cpp
**作者: shyfan
**日期: 2023/10/09 20:07:39
**功能: 简单对象
*************************************************************************/

#include "SSFObjectSimple.h"

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFObjectSimple, LogLevel_Debug);

SSFMap<std::string, SKYWALKER_POOL_PTR(SSFObjectSimple)> SSFObjectSimple::ObjectPoolMap;

SSFObjectSimple::SSFObjectSimple()
{
}

SSFObjectSimple::~SSFObjectSimple()
{
}

void SSFObjectSimple::Release()
{
    SSFObjectSimple::RemoveObject(this);
}
