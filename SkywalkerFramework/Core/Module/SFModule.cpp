/*************************************************************************
**文件: SkywalkerFramework\Core\Module\SFModule.cpp
**作者: shyfan
**日期: 2023/08/07 23:52:19
**功能: 模块
*************************************************************************/

#include "SFModule.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule, ESFLogLevel::Debug);

#pragma region Object

void SSFModule::Init(SFObjectErrors &Errors)
{
    SF_LOG_DEBUG_MODULE("Init");
}

void SSFModule::Awake(SFObjectErrors &Errors)
{
    SF_LOG_DEBUG_MODULE("Awake");
}

void SSFModule::Start(SFObjectErrors &Errors)
{
    SF_LOG_DEBUG_MODULE("Start");
}

void SSFModule::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
}

void SSFModule::Stop(SFObjectErrors &Errors)
{

    SF_LOG_DEBUG_MODULE("Stop");
}

void SSFModule::Sleep(SFObjectErrors &Errors)
{

    SF_LOG_DEBUG_MODULE("Sleep");
}

void SSFModule::Destroy(SFObjectErrors &Errors)
{

    SF_LOG_DEBUG_MODULE("Destroy");
}

#pragma endregion Object

SSFModule::SSFModule(SSFModuleContext &InContext, SFObjectErrors &InErrors)
    : SSFObject(InContext, InErrors)
{
    Plugin = InContext.Plugin;
}

SSFModule::~SSFModule()
{
}
