/*************************************************************************
**文件: SkywalkerFramework\Plugin\CommandLine\Module\SSFModule_AdminCommand.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 管理命令模块
*************************************************************************/

#include "SSFModule_AdminCommand.h"

#include "Include/SFILog.h"

#include "Core/Service/SSFGameplayServiceGateway.h"

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
        "show_ai_stats",
        "show_ai_audit",
        "clear_ai_audit",
        "show_ai_strategies",
        "set_ai_strategy",
        "show_replay_stats",
        "show_replay_event",
        "show_replay_events",
        "start_replay_record",
        "stop_replay_record",
        "start_replay",
        "stop_replay",
    };

    AdminOnlyCommands = {
        "reload_config",
        "kick_player",
        "ban_ip",
        "unban_ip",
        "set_ai_strategy",
        "clear_ai_audit",
        "start_replay_record",
        "stop_replay_record",
        "start_replay",
        "stop_replay",
    };

    OperatorCommands = {
        "show_stats",
        "show_ai_stats",
        "show_ai_audit",
        "show_ai_strategies",
        "show_replay_stats",
        "show_replay_event",
        "show_replay_events",
    };

    ObserverCommands = OperatorCommands;

    SF_LOG_FRAMEWORK("AdminCommand module init");
}

void SSFModule_AdminCommand::Destroy(SFObjectErrors &Errors)
{
    SupportedCommands.clear();
    AdminOnlyCommands.clear();
    OperatorCommands.clear();
    ObserverCommands.clear();

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

    SFString Role;
    SFString ResolvedCommandLine;
    if (!ParseRoleAndCommand(CommandLine, Role, ResolvedCommandLine))
    {
        SF_LOG_ERROR("Execute admin command failed: invalid role command line " << CommandLine);
        return false;
    }

    std::istringstream Stream(ResolvedCommandLine);
    SFString Command;
    Stream >> Command;
    if (Command.empty())
    {
        SF_LOG_ERROR("Execute admin command failed: invalid command line " << CommandLine);
        return false;
    }

    if (!HasPermission(Role, Command))
    {
        SF_LOG_ERROR("Execute admin command denied, Role " << Role << " Command " << Command);
        return false;
    }

    if (Command == "start_replay_record")
    {
        SFUInt64 SessionId = 0;
        Stream >> SessionId;
        if (SessionId == 0)
        {
            SF_LOG_ERROR("start_replay_record failed: invalid SessionId");
            return false;
        }

        bool bResult = SSFGameplayServiceGateway::Instance().StartReplayRecord(SessionId);
        SF_LOG_FRAMEWORK("start_replay_record SessionId " << SessionId << " Result " << bResult);
        return bResult;
    }

    if (Command == "show_replay_stats")
    {
        SFString RecordStats = SSFGameplayServiceGateway::Instance().GetReplayRecordStats();
        SFString PlayStats = SSFGameplayServiceGateway::Instance().GetReplayPlayStats();
        SF_LOG_FRAMEWORK("show_replay_stats Record[" << RecordStats << "] Play[" << PlayStats << "]");
        return true;
    }

    if (Command == "show_replay_event")
    {
        SFUInt64 EventIndex = static_cast<SFUInt64>(-1);
        Stream >> EventIndex;
        if (EventIndex == static_cast<SFUInt64>(-1))
        {
            SF_LOG_ERROR("show_replay_event failed: invalid EventIndex");
            return false;
        }

        SFString ReplayEvent = SSFGameplayServiceGateway::Instance().GetReplayEventByIndex(EventIndex);
        if (ReplayEvent.empty() || ReplayEvent == "ReplayEventUnavailable")
        {
            SF_LOG_ERROR("show_replay_event failed, EventIndex " << EventIndex << " Value " << ReplayEvent);
            return false;
        }

        SF_LOG_FRAMEWORK("show_replay_event " << ReplayEvent);
        return true;
    }

    if (Command == "show_replay_events")
    {
        SFUInt64 StartIndex = static_cast<SFUInt64>(-1);
        SFUInt64 Count = 0;
        Stream >> StartIndex;
        Stream >> Count;
        if (StartIndex == static_cast<SFUInt64>(-1) || Count == 0)
        {
            SF_LOG_ERROR("show_replay_events failed: invalid StartIndex or Count");
            return false;
        }

        SFString ReplayEvents = SSFGameplayServiceGateway::Instance().GetReplayEventsRange(StartIndex, Count);
        if (ReplayEvents.empty() || ReplayEvents == "ReplayEventsRangeUnavailable")
        {
            SF_LOG_ERROR("show_replay_events failed, StartIndex " << StartIndex << " Count " << Count << " Value " << ReplayEvents);
            return false;
        }

        SF_LOG_FRAMEWORK("show_replay_events " << ReplayEvents);
        return true;
    }

    if (Command == "stop_replay_record")
    {
        bool bResult = SSFGameplayServiceGateway::Instance().StopReplayRecord();
        SF_LOG_FRAMEWORK("stop_replay_record Result " << bResult);
        return bResult;
    }

    if (Command == "start_replay")
    {
        SFUInt64 SessionId = 0;
        Stream >> SessionId;
        if (SessionId == 0)
        {
            SF_LOG_ERROR("start_replay failed: invalid SessionId");
            return false;
        }

        bool bResult = SSFGameplayServiceGateway::Instance().StartReplayPlay(SessionId);
        SF_LOG_FRAMEWORK("start_replay SessionId " << SessionId << " Result " << bResult);
        return bResult;
    }

    if (Command == "stop_replay")
    {
        bool bResult = SSFGameplayServiceGateway::Instance().StopReplayPlay();
        SF_LOG_FRAMEWORK("stop_replay Result " << bResult);
        return bResult;
    }

    if (Command == "set_ai_strategy")
    {
        SFString StrategyName;
        Stream >> StrategyName;
        if (StrategyName.empty())
        {
            SF_LOG_ERROR("set_ai_strategy failed: invalid strategy name");
            return false;
        }

        bool bResult = SSFGameplayServiceGateway::Instance().SetAIStrategy(StrategyName);
        SF_LOG_FRAMEWORK("set_ai_strategy " << StrategyName << " Result " << bResult);
        return bResult;
    }

    if (Command == "show_ai_stats")
    {
        SFString AIStats = SSFGameplayServiceGateway::Instance().GetAIStats();
        SF_LOG_FRAMEWORK("show_ai_stats " << AIStats);
        return true;
    }

    if (Command == "show_ai_audit")
    {
        SFString AIAudit = SSFGameplayServiceGateway::Instance().GetAIAudit();
        SF_LOG_FRAMEWORK("show_ai_audit " << AIAudit);
        return true;
    }

    if (Command == "clear_ai_audit")
    {
        bool bResult = SSFGameplayServiceGateway::Instance().ClearAIAudit();
        SF_LOG_FRAMEWORK("clear_ai_audit Result " << bResult);
        return bResult;
    }

    if (Command == "show_ai_strategies")
    {
        SFString AIStrategies = SSFGameplayServiceGateway::Instance().GetAIStrategies();
        SF_LOG_FRAMEWORK("show_ai_strategies " << AIStrategies);
        return true;
    }

    for (const auto &Supported : SupportedCommands)
    {
        if (ResolvedCommandLine.rfind(Supported, 0) == 0)
        {
            SF_LOG_FRAMEWORK("Execute admin command: " << ResolvedCommandLine << " Role " << Role);
            return true;
        }
    }

    SF_LOG_ERROR("Unknown admin command: " << CommandLine << " SupportedCount " << SupportedCommands.size());
    return false;
}

bool SSFModule_AdminCommand::ParseRoleAndCommand(const SFString &CommandLine, SFString &OutRole, SFString &OutResolvedCommandLine) const
{
    OutRole = "admin";
    OutResolvedCommandLine = CommandLine;

    const SFString RolePrefix = "role=";
    if (CommandLine.rfind(RolePrefix, 0) != 0)
    {
        return true;
    }

    const size_t SpacePos = CommandLine.find(' ');
    if (SpacePos == SFString::npos)
    {
        return false;
    }

    OutRole = CommandLine.substr(RolePrefix.size(), SpacePos - RolePrefix.size());
    if (OutRole.empty())
    {
        return false;
    }

    OutResolvedCommandLine = CommandLine.substr(SpacePos + 1);
    return !OutResolvedCommandLine.empty();
}

bool SSFModule_AdminCommand::HasPermission(const SFString &Role, const SFString &Command) const
{
    if (Role == "admin")
    {
        return true;
    }

    if (Role == "operator")
    {
        return OperatorCommands.find(Command) != OperatorCommands.end();
    }

    if (Role == "observer")
    {
        return ObserverCommands.find(Command) != ObserverCommands.end();
    }

    return false;
}
