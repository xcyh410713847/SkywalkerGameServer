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
    SSFObject::Init(Errors);

    SKYWALKER_SF_LOG_DEBUG_PLUGIN("Init");

    // 加载模块
    Install(Errors);
}

void SSFOPlugin::Awake(SSFObjectErrors &Errors)
{
    SSFObject::Awake(Errors);

    SKYWALKER_SF_LOG_DEBUG_PLUGIN("Awake");

    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Awake(Errors);
    }
}

void SSFOPlugin::Start(SSFObjectErrors &Errors)
{
    SSFObject::Start(Errors);

    SKYWALKER_SF_LOG_DEBUG_PLUGIN("Start");

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
    SSFObject::Stop(Errors);

    SKYWALKER_SF_LOG_DEBUG_PLUGIN("Stop");

    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Stop(Errors);
    }
}

void SSFOPlugin::Sleep(SSFObjectErrors &Errors)
{
    SSFObject::Sleep(Errors);

    SKYWALKER_SF_LOG_DEBUG_PLUGIN("Sleep");

    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Sleep(Errors);
    }
}

void SSFOPlugin::Destroy(SSFObjectErrors &Errors)
{
    SSFObject::Destroy(Errors);

    SKYWALKER_SF_LOG_DEBUG_PLUGIN("Destroy");
    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Destroy(Errors);
    }
}

void SSFOPlugin::Release(SSFObjectErrors &Errors)
{
    SSFObject::Release(Errors);

    SKYWALKER_SF_LOG_DEBUG_PLUGIN("Release");

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
