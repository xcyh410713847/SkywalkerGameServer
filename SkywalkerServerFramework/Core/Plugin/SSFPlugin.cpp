/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPlugin.cpp
**作者: shyfan
**日期: 2023/08/07 23:52:54
**功能: 插件
*************************************************************************/

#include "SSFPlugin.h"

#include "Include/SSFCore.h"
#include "Include/SSFErrors.h"
#include "Include/SSFILog.h"
#include "Include/SSFEvent.h"

SSF_NAMESPACE_USING
SKYWALKER_EVENT_NAMESPACE_USING

SSF_LOG_DEFINE(SSFPlugin, LogLevel_Debug);

#define SSF_LOG_DEBUG_PLUGIN(content) SSF_LOG_DEBUG(GetObjectClassName() << " " << content)

SSFPlugin::SSFPlugin(SSFPluginContext &InContext, SSFObjectErrors &InErrors)
    : SSFObjectManager(InContext, InErrors)
{
    PluginManager = InContext.PluginManager;
}

SSFPlugin::~SSFPlugin()
{
}

void SSFPlugin::Release(SSFObjectErrors &Errors)
{
    Uninstall(Errors);

    SSF_LOG_DEBUG_PLUGIN("Release");

    SSFObjectManager::Release(Errors);
}

#pragma region Plugin Process

void SSFPlugin::Init(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_PLUGIN("Init");

    // 加载模块
    Install(Errors);

    // 发送插件初始化事件
    SSFEventPluginAll EventPluginAll;
    EventPluginAll.Plugin = this;
    SKYWALKER_TRIGGER_EVENT(SSFEventMainType_Plugin, SSFEventSubType_Plugin_Init, EventPluginAll);
}

void SSFPlugin::Awake(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_PLUGIN("Awake");

    SSF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        auto IterObject = FindObject(IterModule->second);
        if (SSF_PTR_INVALID(IterObject))
        {
            continue;
        }
        ((SSFModule *)IterObject)->Awake(Errors);
    }
}

void SSFPlugin::Start(SSFObjectErrors &Errors)
{
    SSF_LOG_DEBUG_PLUGIN("Start");

    SSF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        auto Module = GetModule(IterModule->first);
        if (SSF_PTR_INVALID(Module))
        {
            continue;
        }
        Module->Start(Errors);
    }
}

void SSFPlugin::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    SSF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        auto Module = GetModule(IterModule->first);
        if (SSF_PTR_INVALID(Module))
        {
            continue;
        }
        Module->Tick(Errors, DelayMS);
    }
}

void SSFPlugin::Stop(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        auto Module = GetModule(IterModule->first);
        if (SSF_PTR_INVALID(Module))
        {
            continue;
        }
        Module->Stop(Errors);
    }

    SSF_LOG_DEBUG_PLUGIN("Stop");
}

void SSFPlugin::Sleep(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        auto Module = GetModule(IterModule->first);
        if (SSF_PTR_INVALID(Module))
        {
            continue;
        }
        Module->Sleep(Errors);
    }

    SSF_LOG_DEBUG_PLUGIN("Sleep");
}

void SSFPlugin::Destroy(SSFObjectErrors &Errors)
{
    SSF_COMMON_ITERATOR(IterModule, ModuleMap)
    {
        auto Module = GetModule(IterModule->first);
        if (SSF_PTR_INVALID(Module))
        {
            continue;
        }
        Module->Destroy(Errors);
    }

    SSF_LOG_DEBUG_PLUGIN("Destroy");
}

#pragma endregion Plugin Process

#pragma region Module

void SSFPlugin::RegisterModule(SSFObjectErrors &Errors, SSF_PTR(SSFModule) Module)
{
    if (!SSF_PTR_VALID(Module))
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Register_nullptr, "Plugin Register Module nullptr");
        return;
    }

    const std::string &ModuleName = Module->GetObjectClassName();
    if (ModuleName.empty())
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Register_NameEmpty, "Plugin Register Module NameEmpty");
        return;
    }

    if (ModuleMap.find(ModuleName) != ModuleMap.end())
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Register_Repeat, "Plugin Register Module Repeat");
        return;
    }

    AddObject(Errors, Module);
    if (Errors.IsValid())
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Register_AddObjectFailed, "Plugin Register Module AddObjectFailed");
        return;
    }
    ModuleMap.insert(std::make_pair(ModuleName, Module->GetObjectGUID()));
}

void SSFPlugin::UnregisterModule(SSFObjectErrors &Errors, SSF_PTR(SSFModule) Module)
{
    if (!SSF_PTR_VALID(Module))
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Unregister_nullptr, "Plugin Unregister Module nullptr");
        return;
    }

    const std::string &ModuleName = Module->GetObjectClassName();
    if (ModuleName.empty())
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Unregister_NameEmpty, "Plugin Unregister Module NameEmpty");
        return;
    }

    auto Iter = ModuleMap.find(ModuleName);
    if (Iter == ModuleMap.end())
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Unregister_NotFound, "Plugin Unregister Module NotFound");
        return;
    }

    RemoveObject(Errors, Module);
    if (Errors.IsValid())
    {
        SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Module_Unregister_RemoveObjectFailed, "Plugin Unregister Module RemoveObjectFailed");
        return;
    }
    ModuleMap.erase(Iter);
}

#pragma endregion Module
