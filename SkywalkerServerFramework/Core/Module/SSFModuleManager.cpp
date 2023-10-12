/*************************************************************************
**文件: SkywalkerServerFramework\Core\Module\SSFModuleManager.cpp
**作者: shyfan
**日期: 2023/08/25 12:12:30
**功能: 模块管理器
*************************************************************************/

#include "SSFModuleManager.h"

#include "Core/Plugin/SSFPlugin.h"

SSF_NAMESPACE_USING

void SSFOModuleManager::RegisterModule(SSFModuleErrors &Errors, SSF_PTR_MODULE Module)
{
    if (!SSF_PTR_VALID(Module))
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

void SSFOModuleManager::UnregisterModule(SSFModuleErrors &Errors, SSF_PTR_MODULE Module)
{
    if (!SSF_PTR_VALID(Module))
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

SSF_PTR_MODULE SSFOModuleManager::GetModule(const std::string &ModuleName)
{
    auto Iter = ModuleMap.find(ModuleName);
    if (Iter != ModuleMap.end())
    {
        return Iter->second;
    }

    return nullptr;
}
