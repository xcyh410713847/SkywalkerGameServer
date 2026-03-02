/*************************************************************************
**文件: SkywalkerFramework\Plugin\CommandLine\Module\SSFModule_AdminCommand.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 管理命令模块
*************************************************************************/

#include "SSFModule_AdminCommand.h"

#include "Include/SFILog.h"

#include <sstream>

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_AdminCommand, ESFLogLevel::Debug);

void SSFModule_AdminCommand::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);

    SupportedCommands = {
        "reload_config",
        "kick_player",
        "ban_ip",
        "unban_ip",
        "show_stats",
    };

    SF_LOG_FRAMEWORK("AdminCommand module init");
}

void SSFModule_AdminCommand::Destroy(SFObjectErrors &Errors)
{
    SupportedCommands.clear();

    SF_LOG_FRAMEWORK("AdminCommand module destroy");

    SSFModule::Destroy(Errors);
}

bool SSFModule_AdminCommand::ExecuteCommand(const SFString &CommandLine)
{
    if (CommandLine.empty())
    {
        SF_LOG_ERROR("Execute admin command failed: empty command");
        return false;
    }

    std::istringstream Stream(CommandLine);
    SFString Command;
    Stream >> Command;
    if (Command.empty())
    {
        SF_LOG_ERROR("Execute admin command failed: invalid command line " << CommandLine);
        return false;
    }

    for (const auto &Supported : SupportedCommands)
    {
        if (CommandLine.rfind(Supported, 0) == 0)
        {
            SF_LOG_FRAMEWORK("Execute admin command: " << CommandLine);
            return true;
        }
    }

    SF_LOG_ERROR("Unknown admin command: " << CommandLine << " SupportedCount " << SupportedCommands.size());
    return false;
}
