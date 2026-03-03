/*************************************************************************
**文件: SkywalkerFramework\Plugin\CommandLine\Module\SSFModule_AdminCommand.h
**作者: shyfan
**日期: 2026/03/02
**功能: 管理命令模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_ADMINCOMMAND_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_ADMINCOMMAND_H__

#include "Core/Module/SSFModule.h"

#include <unordered_set>
#include <vector>

SF_NAMESPACE_BEGIN

class SSFModule_AdminCommand : public SSFModule
{
#pragma region Object

public:
    virtual void Init(SFObjectErrors &Errors) override;
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SSFModule_AdminCommand(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SSFModule_AdminCommand() {};

    bool ExecuteCommand(const SFString &CommandLine);

private:
    bool ParseRoleAndCommand(const SFString &CommandLine, SFString &OutRole, SFString &OutResolvedCommandLine) const;
    bool HasPermission(const SFString &Role, const SFString &Command) const;

private:
    std::vector<SFString> SupportedCommands;
    std::unordered_set<SFString> AdminOnlyCommands;
    std::unordered_set<SFString> OperatorCommands;
    std::unordered_set<SFString> ObserverCommands;
    SFUInt64 ExecuteSuccessCount = 0;
    SFUInt64 ExecuteDeniedCount = 0;
    SFUInt64 ExecuteFailureCount = 0;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_ADMINCOMMAND_H__
