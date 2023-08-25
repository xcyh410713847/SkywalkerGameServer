/*************************************************************************
**文件: SkywalkerServerFramework\Core\Module\SSFModule.cpp
**作者: shyfan
**日期: 2023/08/07 23:52:19
**功能: 模块
*************************************************************************/

#include "SSFModule.h"

#include "Include/SSFILog.h"

SKYWALKER_SF_NAMESPACE_USE

SKYWALKER_SF_LOG_DEFINE(SSFModule, LogLevel_Debug);

#define SKYWALKER_SF_LOG_DEBUG_MODULE(content) SKYWALKER_SF_LOG_DEBUG(GetName() << " " << content)

#pragma region Object

void SSFOModule::Init(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_LOG_DEBUG_MODULE("Init");

    SSFObject::Init(Errors);
}

void SSFOModule::Awake(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_LOG_DEBUG_MODULE("Awake");

    SSFObject::Awake(Errors);
}

void SSFOModule::Start(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_LOG_DEBUG_MODULE("Start");

    SSFObject::Start(Errors);
}

void SSFOModule::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SSFObject::Tick(Errors, DelayMS);
}

void SSFOModule::Stop(SSFObjectErrors &Errors)
{
    SSFObject::Stop(Errors);

    SKYWALKER_SF_LOG_DEBUG_MODULE("Stop");
}

void SSFOModule::Sleep(SSFObjectErrors &Errors)
{
    SSFObject::Sleep(Errors);

    SKYWALKER_SF_LOG_DEBUG_MODULE("Sleep");
}

void SSFOModule::Destroy(SSFObjectErrors &Errors)
{
    SSFObject::Destroy(Errors);

    SKYWALKER_SF_LOG_DEBUG_MODULE("Destroy");
}

void SSFOModule::Release(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_LOG_DEBUG_MODULE("Release");

    SSFObject::Release(Errors);
}

#pragma endregion Object

SSFOModule::SSFOModule(SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager)
    : PluginManager(InPluginManager)
{
}

SSFOModule::~SSFOModule()
{
}
