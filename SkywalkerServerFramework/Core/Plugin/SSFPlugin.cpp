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

#pragma region Object

void SSFOPlugin::Init(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_LOG_DEBUG("Init");

    // 加载模块
    Install(Errors);
}

void SSFOPlugin::Awake(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_LOG_DEBUG("Awake");

    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Awake(Errors);
    }
}

void SSFOPlugin::Start(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_LOG_DEBUG("Start");

    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Start(Errors);
    }
}

void SSFOPlugin::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Tick(Errors, DelayMS);
    }
}

void SSFOPlugin::Stop(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_LOG_DEBUG("Stop");

    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Stop(Errors);
    }
}

void SSFOPlugin::Sleep(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_LOG_DEBUG("Sleep");

    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Sleep(Errors);
    }
}

void SSFOPlugin::Destroy(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_LOG_DEBUG("Destroy");
    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Destroy(Errors);
    }
}

void SSFOPlugin::Release(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_LOG_DEBUG("Release");

    Uninstall(Errors);

    delete this;
}

#pragma endregion Object

SSFOPlugin::SSFOPlugin(SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager)
    : PluginManager(InPluginManager)
{
}

SSFOPlugin::~SSFOPlugin()
{
}

template <typename T>
SKYWALKER_SF_PTR(T)
SSFOPlugin::GetModule()
{
    return nullptr;
}

void SSFOPlugin::Install(SSFModuleErrors &Errors)
{
    // TODO Shyfan 插件管理器Install
}

void SSFOPlugin::Uninstall(SSFModuleErrors &Errors)
{
    // TODO Shyfan 插件管理器Uninstall
}

void SSFOPlugin::AddModule(const std::string &ModuleName, SKYWALKER_SF_PTR_MODULE Module)
{
    // TODO Shyfan 插件管理器AddModule
}

void SSFOPlugin::RemoveModule(const std::string &ModuleName)
{
    // TODO Shyfan 插件管理器RemoveModule
}
