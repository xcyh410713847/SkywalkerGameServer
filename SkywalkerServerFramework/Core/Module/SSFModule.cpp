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

#define SSF_LOG_DEBUG_MODULE(content) SSF_LOG_DEBUG(GetObjectClassName() << " " << content)

#pragma region Object

void SSFOModule::Init(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_MODULE("Init");
}

void SSFOModule::Awake(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_MODULE("Awake");
}

void SSFOModule::Start(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_MODULE("Start");
}

void SSFOModule::Tick(SSFObjectErrors &Errors, int DelayMS)
{
}

void SSFOModule::Stop(SSFObjectErrors &Errors)
{

    SSF_LOG_DEBUG_MODULE("Stop");
}

void SSFOModule::Sleep(SSFObjectErrors &Errors)
{

    SSF_LOG_DEBUG_MODULE("Sleep");
}

void SSFOModule::Destroy(SSFObjectErrors &Errors)
{

    SSF_LOG_DEBUG_MODULE("Destroy");
}

void SSFOModule::Release(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_MODULE("Release");

    delete this;
}

#pragma endregion Object

SSFOModule::SSFOModule(SSF_PTR_PLUGIN_MANAGER InPluginManager)
    : SSFObject(), PluginManager(InPluginManager)
{
}

SSFOModule::~SSFOModule()
{
}
