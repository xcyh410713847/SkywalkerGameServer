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
    // TODO Shyfan 插件管理器Init
}

void SSFOPlugin::Awake(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Awake
}

void SSFOPlugin::Start(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Start
}

void SSFOPlugin::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    // TODO Shyfan 插件管理器Tick
}

void SSFOPlugin::Stop(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Stop
}

void SSFOPlugin::Sleep(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Sleep
}

void SSFOPlugin::Destroy(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Destroy
}

void SSFOPlugin::Release(SSFObjectErrors &Errors)
{
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
