/*************************************************************************
**文件: SkywalkerFramework\Core\Plugin\SSFPlugin.cpp
**作者: shyfan
**日期: 2023/08/07 23:52:54
**功能: 插件
*************************************************************************/

#include "SSFPlugin.h"

#include "Include/SFCore.h"
#include "Include/SFErrors.h"
#include "Include/SFILog.h"
#include "Include/SFEvent.h"

SF_NAMESPACE_USING
SKYWALKER_EVENT_NAMESPACE_USING

SF_LOG_DEFINE(SFPlugin, ESFLogLevel::Debug)

SFPlugin::SFPlugin(SFPluginContext &InContext, SFObjectErrors &InErrors)
    : SSFObjectManager(InContext, InErrors)
{
    PluginManager = InContext.PluginManager;
}

SFPlugin::~SFPlugin()
{
}

void SFPlugin::Release(SFObjectErrors &Errors)
{
    Uninstall(Errors);

    SF_LOG_DEBUG_MODULE("Release");

    SSFObjectManager::Release(Errors);
}

#pragma region Plugin Process

void SFPlugin::Init(SFObjectErrors &Errors)
{
    SF_LOG_DEBUG_MODULE("Init");

    // 加载模块
    Install(Errors);

    // 发送插件初始化事件
    SSFEventPluginAll EventPluginAll;
    EventPluginAll.Plugin = this;
    SKYWALKER_TRIGGER_EVENT(ESFEventMainType::Plugin, SFEventSubType_Plugin_Init, EventPluginAll);
}

void SFPlugin::Awake(SFObjectErrors &Errors)
{
    SF_LOG_DEBUG_MODULE("Awake");

    SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        auto IterObject = FindObject(IterModule->second);
        if (SF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SSFModule *)IterObject)->Awake(Errors);
    }
}

void SFPlugin::Start(SFObjectErrors &Errors)
{
    SF_LOG_DEBUG_MODULE("Start");

    SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        auto Module = GetModule(IterModule->first);
        if (SF_PTR_INVALID(Module))
        {
            continue;
        }
        Module->Start(Errors);
    }
}

void SFPlugin::Tick(SFObjectErrors &Errors, int DelayMS)
{
    SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        auto Module = GetModule(IterModule->first);
        if (SF_PTR_INVALID(Module))
        {
            continue;
        }
        Module->Tick(Errors, DelayMS);
    }
}

void SFPlugin::Stop(SFObjectErrors &Errors)
{
    SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        auto Module = GetModule(IterModule->first);
        if (SF_PTR_INVALID(Module))
        {
            continue;
        }
        Module->Stop(Errors);
    }

    SF_LOG_DEBUG_MODULE("Stop");
}

void SFPlugin::Sleep(SFObjectErrors &Errors)
{
    SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        auto Module = GetModule(IterModule->first);
        if (SF_PTR_INVALID(Module))
        {
            continue;
        }
        Module->Sleep(Errors);
    }

    SF_LOG_DEBUG_MODULE("Sleep");
}

void SFPlugin::Destroy(SFObjectErrors &Errors)
{
    SF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        auto Module = GetModule(IterModule->first);
        if (SF_PTR_INVALID(Module))
        {
            continue;
        }
        Module->Destroy(Errors);
    }

    SF_LOG_DEBUG_MODULE("Destroy");
}

#pragma endregion Plugin Process

#pragma region Module

void SFPlugin::SetConfigModules(const SFMap<SFString, bool> &ModuleNames)
{
    ConfigModules = ModuleNames;
}

void SFPlugin::RegisterModule(SFObjectErrors &Errors, SF_PTR(SSFModule) Module)
{
    if (!SF_PTR_VALID(Module))
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Module_Register_nullptr, "Plugin Register Module nullptr");
        return;
    }

    const SFString &ModuleName = Module->GetObjectClassName();
    if (ModuleName.empty())
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Module_Register_NameEmpty, "Plugin Register Module NameEmpty");
        return;
    }

    if (ModuleMap.find(ModuleName) != ModuleMap.end())
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Module_Register_Repeat, "Plugin Register Module Repeat");
        return;
    }

    if (!ConfigModules.empty())
    {
        auto Iter = ConfigModules.find(ModuleName);
        if (Iter == ConfigModules.end())
        {
            SF_LOG_DEBUG("Module [" << ModuleName << "] not in config, skip loading");
            return;
        }
    }

    AddObject(Errors, Module);
    if (Errors.IsValid())
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Module_Register_AddObjectFailed, "Plugin Register Module AddObjectFailed");
        return;
    }
    ModuleMap.insert(std::make_pair(ModuleName, Module->GetObjectGUID()));
}

void SFPlugin::UnregisterModule(SFObjectErrors &Errors, SF_PTR(SSFModule) Module)
{
    if (!SF_PTR_VALID(Module))
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Module_Unregister_nullptr, "Plugin Unregister Module nullptr");
        return;
    }

    const SFString &ModuleName = Module->GetObjectClassName();
    if (ModuleName.empty())
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Module_Unregister_NameEmpty, "Plugin Unregister Module NameEmpty");
        return;
    }

    auto Iter = ModuleMap.find(ModuleName);
    if (Iter == ModuleMap.end())
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Module_Unregister_NotFound, "Plugin Unregister Module NotFound");
        return;
    }

    RemoveObject(Errors, Module);
    if (Errors.IsValid())
    {
        SF_ERROR_DESC_TRACE(Errors, ESFError::Module_Unregister_RemoveObjectFailed, "Plugin Unregister Module RemoveObjectFailed");
        return;
    }
    ModuleMap.erase(Iter);
}

#pragma endregion Module
