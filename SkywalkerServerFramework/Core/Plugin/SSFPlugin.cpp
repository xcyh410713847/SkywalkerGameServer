/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPlugin.cpp
**作者: shyfan
**日期: 2023/08/07 23:52:54
**功能: 插件
*************************************************************************/

#include "SSFPlugin.h"

#include "Include/SSFILog.h"
#include "Include/SSFEvent.h"

SSF_NAMESPACE_USING
SKYWALKER_EVENT_NAMESPACE_USING

SSF_LOG_DEFINE(SSFPlugin, LogLevel_Debug);

#define SSF_LOG_DEBUG_PLUGIN(content) SSF_LOG_DEBUG(GetObjectClassName() << " " << content)

#pragma region Object

void SSFOPlugin::Init(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_PLUGIN("Init");

    SSFOModuleManager::Init(Errors);

    // 加载模块
    Install(Errors);

    // 发送插件初始化事件
    SSFEventPluginAll EventPluginAll;
    EventPluginAll.Plugin = this;
    SKYWALKER_TRIGGER_EVENT(SSFEventMainType_Plugin, SSFEventSubType_Plugin_Init, EventPluginAll);
}

void SSFOPlugin::Awake(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_PLUGIN("Awake");

    SSFOModuleManager::Awake(Errors);

    SSF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Awake(Errors);
    }
}

void SSFOPlugin::Start(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_PLUGIN("Start");

    SSFOModuleManager::Start(Errors);

    SSF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Start(Errors);
    }
}

void SSFOPlugin::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SSFOModuleManager::Tick(Errors, DelayMS);

    SSF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Tick(Errors, DelayMS);
    }
}

void SSFOPlugin::Stop(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Stop(Errors);
    }

    SSFOModuleManager::Stop(Errors);

    SSF_LOG_DEBUG_PLUGIN("Stop");
}

void SSFOPlugin::Sleep(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Sleep(Errors);
    }

    SSFOModuleManager::Sleep(Errors);

    SSF_LOG_DEBUG_PLUGIN("Sleep");
}

void SSFOPlugin::Destroy(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Destroy(Errors);
    }

    SSFOModuleManager::Destroy(Errors);

    SSF_LOG_DEBUG_PLUGIN("Destroy");
}

void SSFOPlugin::Release(SSFObjectErrors &Errors)
{
    Uninstall(Errors);

    SSF_LOG_DEBUG_PLUGIN("Release");

    SSFOModuleManager::Release(Errors);
}

#pragma endregion Object

SSFOPlugin::SSFOPlugin(SSFPluginContext &InContext, SSFObjectErrors &InErrors)
    : SSFOModuleManager(InContext, InErrors)
{
}

SSFOPlugin::~SSFOPlugin()
{
}
