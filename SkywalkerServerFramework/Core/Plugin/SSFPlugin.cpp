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
    : PluginManager(InPluginManager)
{
}

SSFOPlugin::~SSFOPlugin()
{
}

void SSFOPlugin::RegisterModule(SSFModuleErrors &Errors, SKYWALKER_SF_PTR_MODULE Module)
{
    if (!SKYWALKER_SF_PTR_VALID(Module))
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Register_nullptr, "Plugin Register Module nullptr");
        return;
    }

    const std::string &ModuleName = Module->GetName();
    if (ModuleName.empty())
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Register_NameEmpty, "Plugin Register Module NameEmpty");
        return;
    }

    auto Iter = ModuleMap.find(ModuleName);
    if (Iter != ModuleMap.end())
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Register_Repeat, "Plugin Register Module Repeat");
        return;
    }

    ModuleMap.insert(std::make_pair(ModuleName, Module));
}

void SSFOPlugin::UnregisterModule(SSFModuleErrors &Errors, SKYWALKER_SF_PTR_MODULE Module)
{
    if (!SKYWALKER_SF_PTR_VALID(Module))
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Unregister_nullptr, "Plugin Unregister Module nullptr");
        return;
    }

    const std::string &ModuleName = Module->GetName();
    if (ModuleName.empty())
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Unregister_NameEmpty, "Plugin Unregister Module NameEmpty");
        return;
    }

    auto Iter = ModuleMap.find(ModuleName);
    if (Iter == ModuleMap.end())
    {
        SKYWALKER_SF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Unregister_NotFound, "Plugin Unregister Module NotFound");
        return;
    }

    ModuleMap.erase(Iter);
}

SKYWALKER_SF_PTR_MODULE SSFOPlugin::GetModule(const std::string &ModuleName)
{
    auto Iter = ModuleMap.find(ModuleName);
    if (Iter != ModuleMap.end())
    {
        return Iter->second;
    }

    return nullptr;
}
