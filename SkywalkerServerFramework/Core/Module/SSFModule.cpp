/*************************************************************************
**文件: SkywalkerServerFramework\Core\Module\SSFModule.cpp
**作者: shyfan
**日期: 2023/08/07 23:52:19
**功能: 模块
*************************************************************************/

#include "SSFModule.h"

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFModule, LogLevel_Debug);

#pragma region Object

void SSFModule::Init(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_MODULE("Init");
}

void SSFModule::Awake(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_MODULE("Awake");
}

void SSFModule::Start(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_MODULE("Start");
}

void SSFModule::Tick(SSFObjectErrors &Errors, int DelayMS)
{
}

void SSFModule::Stop(SSFObjectErrors &Errors)
{

    SSF_LOG_DEBUG_MODULE("Stop");
}

void SSFModule::Sleep(SSFObjectErrors &Errors)
{

    SSF_LOG_DEBUG_MODULE("Sleep");
}

void SSFModule::Destroy(SSFObjectErrors &Errors)
{

    SSF_LOG_DEBUG_MODULE("Destroy");
}

#pragma endregion Object

SSFModule::SSFModule(SSFModuleContext &InContext, SSFObjectErrors &InErrors)
    : SSFObject(InContext, InErrors)
{
    Plugin = InContext.Plugin;
}

SSFModule::~SSFModule()
{
}
