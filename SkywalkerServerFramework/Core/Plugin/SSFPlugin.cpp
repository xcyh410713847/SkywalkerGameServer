/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPlugin.cpp
**作者: shyfan
**日期: 2023/08/07 23:52:54
**功能: 插件
*************************************************************************/

#include "SSFPlugin.h"

#include "Include/SSFILog.h"

SKYWALKER_SF_NAMESPACE_USE

SKYWALKER_SF_LOG_DEFINE(SSFPlugin, LogLevel_Debug);

#define SKYWALKER_SF_LOG_DEBUG_PLUGIN(content) SKYWALKER_SF_LOG_DEBUG(GetName() << " " << content)

#pragma region Object

void SSFOPlugin::Init(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_LOG_DEBUG_PLUGIN("Init");

    SSFObject::Init(Errors);

    // 加载模块
    Install(Errors);
}

void SSFOPlugin::Awake(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_LOG_DEBUG_PLUGIN("Awake");

    SSFObject::Awake(Errors);

    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Awake(Errors);
    }
}

void SSFOPlugin::Start(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_LOG_DEBUG_PLUGIN("Start");

    SSFObject::Start(Errors);

    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Start(Errors);
    }
}

void SSFOPlugin::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SSFObject::Tick(Errors, DelayMS);

    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Tick(Errors, DelayMS);
    }
}

void SSFOPlugin::Stop(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Stop(Errors);
    }

    SSFObject::Stop(Errors);

    SKYWALKER_SF_LOG_DEBUG_PLUGIN("Stop");
}

void SSFOPlugin::Sleep(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Sleep(Errors);
    }

    SSFObject::Sleep(Errors);

    SKYWALKER_SF_LOG_DEBUG_PLUGIN("Sleep");
}

void SSFOPlugin::Destroy(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Destroy(Errors);
    }

    SSFObject::Destroy(Errors);

    SKYWALKER_SF_LOG_DEBUG_PLUGIN("Destroy");
}

void SSFOPlugin::Release(SSFObjectErrors &Errors)
{
    Uninstall(Errors);

    SKYWALKER_SF_LOG_DEBUG_PLUGIN("Release");

    SSFObject::Release(Errors);
}

#pragma endregion Object

SSFOPlugin::SSFOPlugin(SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager)
    : SSFOModuleManager(), PluginManager(InPluginManager)
{
}

SSFOPlugin::~SSFOPlugin()
{
}
