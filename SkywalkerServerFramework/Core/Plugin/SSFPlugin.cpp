/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPlugin.cpp
**作者: shyfan
**日期: 2023/08/07 23:52:54
**功能: 插件
*************************************************************************/

#include "SSFPlugin.h"

SKYWALKER_SF_NAMESPACE_USE

#pragma region Object

void SSFOPlugin::Init(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Init(Errors);
    }
}

void SSFOPlugin::Awake(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Awake(Errors);
    }
}

void SSFOPlugin::Start(SSFObjectErrors &Errors)
{
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
    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Stop(Errors);
    }
}

void SSFOPlugin::Sleep(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Sleep(Errors);
    }
}

void SSFOPlugin::Destroy(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Destroy(Errors);
    }
}

void SSFOPlugin::Release(SSFObjectErrors &Errors)
{
    SKYWALKER_SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        ((SSFOModule *)IterModule->second)->Release(Errors);
    }

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
SSFOPlugin::GetModule(const std::string &ModuleName)
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
