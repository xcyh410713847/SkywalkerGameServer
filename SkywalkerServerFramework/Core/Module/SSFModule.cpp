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

#define SSF_LOG_DEBUG_MODULE(content) SSF_LOG_DEBUG(GetName() << " " << content)

#pragma region Object

void SSFOModule::Init(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_MODULE("Init");

    SSFObject::Init(Errors);
}

void SSFOModule::Awake(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_MODULE("Awake");

    SSFObject::Awake(Errors);
}

void SSFOModule::Start(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_MODULE("Start");

    SSFObject::Start(Errors);
}

void SSFOModule::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SSFObject::Tick(Errors, DelayMS);
}

void SSFOModule::Stop(SSFObjectErrors &Errors)
{
    SSFObject::Stop(Errors);

    SSF_LOG_DEBUG_MODULE("Stop");
}

void SSFOModule::Sleep(SSFObjectErrors &Errors)
{
    SSFObject::Sleep(Errors);

    SSF_LOG_DEBUG_MODULE("Sleep");
}

void SSFOModule::Destroy(SSFObjectErrors &Errors)
{
    SSFObject::Destroy(Errors);

    SSF_LOG_DEBUG_MODULE("Destroy");
}

void SSFOModule::Release(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_MODULE("Release");

    SSFObject::Release(Errors);
}

#pragma endregion Object

SSFOModule::SSFOModule(SSF_PTR_PLUGIN_MANAGER InPluginManager)
    : SSFObject(), PluginManager(InPluginManager)
{
}

SSFOModule::~SSFOModule()
{
}
