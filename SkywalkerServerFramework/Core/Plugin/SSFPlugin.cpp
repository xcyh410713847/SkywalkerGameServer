/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPlugin.cpp
**作者: shyfan
**日期: 2023/08/07 23:52:54
**功能: 插件
*************************************************************************/

#include "SSFPlugin.h"

SKYWALKER_SF_NAMESPACE_USE

SSFCPlugin::SSFCPlugin(SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager)
    : PluginManager(InPluginManager)
{
}

SSFCPlugin::~SSFCPlugin()
{
}

#pragma region Object Base Interface

void SSFCPlugin::Init(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Init
}

void SSFCPlugin::Awake(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Awake
}

void SSFCPlugin::Start(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Start
}

void SSFCPlugin::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    // TODO Shyfan 插件管理器Tick
}

void SSFCPlugin::Stop(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Stop
}

void SSFCPlugin::Sleep(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Sleep
}

void SSFCPlugin::Destroy(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Destroy
}

void SSFCPlugin::Release(SSFObjectErrors &Errors)
{
    delete this;
}

#pragma endregion Object Base Interface

template <typename T>
SKYWALKER_SF_PTR(T)
SSFCPlugin::GetModule(const std::string &ModuleName)
{
    return nullptr;
}

void SSFCPlugin::Install(SSFModuleErrors &Errors)
{
    // TODO Shyfan 插件管理器Install
}

void SSFCPlugin::Uninstall(SSFModuleErrors &Errors)
{
    // TODO Shyfan 插件管理器Uninstall
}

void SSFCPlugin::AddModule(const std::string &ModuleName, SSF_PTR_MODULE Module)
{
    // TODO Shyfan 插件管理器AddModule
}

void SSFCPlugin::RemoveModule(const std::string &ModuleName)
{
    // TODO Shyfan 插件管理器RemoveModule
}
