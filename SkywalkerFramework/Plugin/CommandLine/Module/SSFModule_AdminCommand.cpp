/*************************************************************************
**文件: SkywalkerFramework\Plugin\CommandLine\Module\SSFModule_AdminCommand.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 管理命令模块
*************************************************************************/

#include "SSFModule_AdminCommand.h"

#include "Include/SFILog.h"

#include "Core/Service/SSFGameplayServiceGateway.h"

#include "SkywalkerScript/Include/SkywalkerScriptParse.h"

#include <sstream>
#include <algorithm>

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_AdminCommand, ESFLogLevel::Debug);

void SSFModule_AdminCommand::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);

    SupportedCommands = {
        "reload_config",
        "reload_acl",
        "kick_player",
        "ban_ip",
        "unban_ip",
        "show_stats",
        "show_ai_stats",
        "show_ai_audit",
        "show_ai_audit_stats",
        "clear_ai_audit",
        "show_ai_strategies",
        "show_admin_acl_stats",
        "set_ai_strategy",
        "show_replay_stats",
        "show_replay_query_stats",
        "show_replay_event",
        "show_replay_events",
        "find_replay_events",
        "start_replay_record",
        "stop_replay_record",
        "start_replay",
        "stop_replay",
    };

    ResetDefaultACL();
    ReloadACLFromConfig();

    ExecuteSuccessCount = 0;
    ExecuteDeniedCount = 0;
    ExecuteFailureCount = 0;

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
        ++ExecuteFailureCount;
        return false;
    }

    std::istringstream Stream(ResolvedCommandLine);
    SFString Command;
    Stream >> Command;
    if (Command.empty())
    {
        SF_LOG_ERROR("Execute admin command failed: invalid command line " << CommandLine);
        ++ExecuteFailureCount;
        return false;
    }

    if (!HasPermission(Role, Command))
    {
        SF_LOG_ERROR("Execute admin command denied, Role " << Role << " Command " << Command);
        ++ExecuteDeniedCount;
        return false;
    }

    if (Command == "start_replay_record")
    {
        SFUInt64 SessionId = 0;
        Stream >> SessionId;
        if (SessionId == 0)
        {
            SF_LOG_ERROR("start_replay_record failed: invalid SessionId");
            ++ExecuteFailureCount;
            return false;
        }

        bool bResult = SSFGameplayServiceGateway::Instance().StartReplayRecord(SessionId);
        SF_LOG_FRAMEWORK("start_replay_record SessionId " << SessionId << " Result " << bResult);
        if (bResult)
        {
            ++ExecuteSuccessCount;
        }
        else
        {
            ++ExecuteFailureCount;
        }
        return bResult;
    }

    if (Command == "show_replay_stats")
    {
        SFString RecordStats = SSFGameplayServiceGateway::Instance().GetReplayRecordStats();
        SFString PlayStats = SSFGameplayServiceGateway::Instance().GetReplayPlayStats();
        SF_LOG_FRAMEWORK("show_replay_stats Record[" << RecordStats << "] Play[" << PlayStats << "]");
        ++ExecuteSuccessCount;
        return true;
    }

    if (Command == "show_replay_query_stats")
    {
        SFString PlayStats = SSFGameplayServiceGateway::Instance().GetReplayPlayStats();
        if (PlayStats == "ReplayPlayStatsUnavailable")
        {
            SF_LOG_ERROR("show_replay_query_stats failed, Value " << PlayStats);
            ++ExecuteFailureCount;
            return false;
        }

        SF_LOG_FRAMEWORK("show_replay_query_stats " << PlayStats);
        ++ExecuteSuccessCount;
        return true;
    }

    if (Command == "show_replay_event")
    {
        SFUInt64 EventIndex = static_cast<SFUInt64>(-1);
        Stream >> EventIndex;
        if (EventIndex == static_cast<SFUInt64>(-1))
        {
            SF_LOG_ERROR("show_replay_event failed: invalid EventIndex");
            ++ExecuteFailureCount;
            return false;
        }

        SFString ReplayEvent = SSFGameplayServiceGateway::Instance().GetReplayEventByIndex(EventIndex);
        if (ReplayEvent.empty() || ReplayEvent == "ReplayEventUnavailable")
        {
            SF_LOG_ERROR("show_replay_event failed, EventIndex " << EventIndex << " Value " << ReplayEvent);
            ++ExecuteFailureCount;
            return false;
        }

        SF_LOG_FRAMEWORK("show_replay_event " << ReplayEvent);
        ++ExecuteSuccessCount;
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
            ++ExecuteFailureCount;
            return false;
        }

        SFString ReplayEvents = SSFGameplayServiceGateway::Instance().GetReplayEventsRange(StartIndex, Count);
        if (ReplayEvents.empty() || ReplayEvents == "ReplayEventsRangeUnavailable")
        {
            SF_LOG_ERROR("show_replay_events failed, StartIndex " << StartIndex << " Count " << Count << " Value " << ReplayEvents);
            ++ExecuteFailureCount;
            return false;
        }

        SF_LOG_FRAMEWORK("show_replay_events " << ReplayEvents);
        ++ExecuteSuccessCount;
        return true;
    }

    if (Command == "find_replay_events")
    {
        SFString Keyword;
        SFUInt64 MaxCount = 0;
        Stream >> Keyword;
        Stream >> MaxCount;
        if (Keyword.empty() || MaxCount == 0)
        {
            SF_LOG_ERROR("find_replay_events failed: invalid Keyword or MaxCount");
            ++ExecuteFailureCount;
            return false;
        }

        SFString ReplayEvents = SSFGameplayServiceGateway::Instance().FindReplayEventsByKeyword(Keyword, MaxCount);
        if (ReplayEvents.empty() || ReplayEvents == "ReplayFindEventsUnavailable")
        {
            SF_LOG_ERROR("find_replay_events failed, Keyword " << Keyword << " MaxCount " << MaxCount << " Value " << ReplayEvents);
            ++ExecuteFailureCount;
            return false;
        }

        SF_LOG_FRAMEWORK("find_replay_events " << ReplayEvents);
        ++ExecuteSuccessCount;
        return true;
    }

    if (Command == "stop_replay_record")
    {
        bool bResult = SSFGameplayServiceGateway::Instance().StopReplayRecord();
        SF_LOG_FRAMEWORK("stop_replay_record Result " << bResult);
        if (bResult)
        {
            ++ExecuteSuccessCount;
        }
        else
        {
            ++ExecuteFailureCount;
        }
        return bResult;
    }

    if (Command == "start_replay")
    {
        SFUInt64 SessionId = 0;
        Stream >> SessionId;
        if (SessionId == 0)
        {
            SF_LOG_ERROR("start_replay failed: invalid SessionId");
            ++ExecuteFailureCount;
            return false;
        }

        bool bResult = SSFGameplayServiceGateway::Instance().StartReplayPlay(SessionId);
        SF_LOG_FRAMEWORK("start_replay SessionId " << SessionId << " Result " << bResult);
        if (bResult)
        {
            ++ExecuteSuccessCount;
        }
        else
        {
            ++ExecuteFailureCount;
        }
        return bResult;
    }

    if (Command == "stop_replay")
    {
        bool bResult = SSFGameplayServiceGateway::Instance().StopReplayPlay();
        SF_LOG_FRAMEWORK("stop_replay Result " << bResult);
        if (bResult)
        {
            ++ExecuteSuccessCount;
        }
        else
        {
            ++ExecuteFailureCount;
        }
        return bResult;
    }

    if (Command == "set_ai_strategy")
    {
        SFString StrategyName;
        Stream >> StrategyName;
        if (StrategyName.empty())
        {
            SF_LOG_ERROR("set_ai_strategy failed: invalid strategy name");
            ++ExecuteFailureCount;
            return false;
        }

        bool bResult = SSFGameplayServiceGateway::Instance().SetAIStrategy(StrategyName);
        SF_LOG_FRAMEWORK("set_ai_strategy " << StrategyName << " Result " << bResult);
        if (bResult)
        {
            ++ExecuteSuccessCount;
        }
        else
        {
            ++ExecuteFailureCount;
        }
        return bResult;
    }

    if (Command == "show_ai_stats")
    {
        SFString AIStats = SSFGameplayServiceGateway::Instance().GetAIStats();
        SF_LOG_FRAMEWORK("show_ai_stats " << AIStats);
        ++ExecuteSuccessCount;
        return true;
    }

    if (Command == "show_ai_audit")
    {
        SFString AIAudit = SSFGameplayServiceGateway::Instance().GetAIAudit();
        SF_LOG_FRAMEWORK("show_ai_audit " << AIAudit);
        ++ExecuteSuccessCount;
        return true;
    }

    if (Command == "show_ai_audit_stats")
    {
        SFString AIAudit = SSFGameplayServiceGateway::Instance().GetAIAudit();
        if (AIAudit == "AIAuditUnavailable")
        {
            SF_LOG_ERROR("show_ai_audit_stats failed, Value " << AIAudit);
            ++ExecuteFailureCount;
            return false;
        }

        SFUInt64 AuditItemCount = GetAuditItemCount(AIAudit);
        SF_LOG_FRAMEWORK("show_ai_audit_stats AuditItemCount " << AuditItemCount << " Raw " << AIAudit);
        ++ExecuteSuccessCount;
        return true;
    }

    if (Command == "clear_ai_audit")
    {
        bool bResult = SSFGameplayServiceGateway::Instance().ClearAIAudit();
        SF_LOG_FRAMEWORK("clear_ai_audit Result " << bResult);
        if (bResult)
        {
            ++ExecuteSuccessCount;
        }
        else
        {
            ++ExecuteFailureCount;
        }
        return bResult;
    }

    if (Command == "show_ai_strategies")
    {
        SFString AIStrategies = SSFGameplayServiceGateway::Instance().GetAIStrategies();
        SF_LOG_FRAMEWORK("show_ai_strategies " << AIStrategies);
        ++ExecuteSuccessCount;
        return true;
    }

    if (Command == "show_admin_acl_stats")
    {
        SF_LOG_FRAMEWORK("show_admin_acl_stats SuccessCount " << ExecuteSuccessCount
                                                              << " DeniedCount " << ExecuteDeniedCount
                                                               << " FailureCount " << ExecuteFailureCount
                                                               << " AdminOnlyCount " << AdminOnlyCommands.size()
                                                               << " OperatorCount " << OperatorCommands.size()
                                                               << " ObserverCount " << ObserverCommands.size());
        ++ExecuteSuccessCount;
        return true;
    }

    if (Command == "reload_acl")
    {
        bool bResult = ReloadACLFromConfig();
        SF_LOG_FRAMEWORK("reload_acl Result " << bResult
                                               << " AdminOnlyCount " << AdminOnlyCommands.size()
                                               << " OperatorCount " << OperatorCommands.size()
                                               << " ObserverCount " << ObserverCommands.size());
        if (bResult)
        {
            ++ExecuteSuccessCount;
        }
        else
        {
            ++ExecuteFailureCount;
        }

        return bResult;
    }

    for (const auto &Supported : SupportedCommands)
    {
        if (ResolvedCommandLine.rfind(Supported, 0) == 0)
        {
            SF_LOG_FRAMEWORK("Execute admin command: " << ResolvedCommandLine << " Role " << Role);
            ++ExecuteSuccessCount;
            return true;
        }
    }

    SF_LOG_ERROR("Unknown admin command: " << CommandLine << " SupportedCount " << SupportedCommands.size());
    ++ExecuteFailureCount;
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

void SSFModule_AdminCommand::ResetDefaultACL()
{
    AdminOnlyCommands = {
        "reload_config",
        "reload_acl",
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
        "show_ai_audit_stats",
        "show_ai_strategies",
        "show_admin_acl_stats",
        "show_replay_stats",
        "show_replay_query_stats",
        "show_replay_event",
        "show_replay_events",
        "find_replay_events",
    };

    ObserverCommands = OperatorCommands;
}

bool SSFModule_AdminCommand::ReloadACLFromConfig()
{
    const char *ConfigPath = nullptr;
#if defined(_WIN32) || defined(_WIN64)
    char *ConfigPathBuffer = nullptr;
    size_t ConfigPathLen = 0;
    _dupenv_s(&ConfigPathBuffer, &ConfigPathLen, "SKYWALKER_SERVER_CONFIG");
    ConfigPath = ConfigPathBuffer;
#else
    ConfigPath = getenv("SKYWALKER_SERVER_CONFIG");
#endif
    SFString ServerConfigPath = ConfigPath ? ConfigPath : "ServerConfig.skywalkerC";
#if defined(_WIN32) || defined(_WIN64)
    if (ConfigPathBuffer != nullptr)
    {
        free(ConfigPathBuffer);
        ConfigPathBuffer = nullptr;
    }
#endif

    ResetDefaultACL();

    SKYWALKER_PTR_SCRIPT_PARSE ConfigParse = new SKYWALKER_SCRIPT_NAMESPACE::CSkywalkerScriptParse();
    if (!ConfigParse->LoadScript(ServerConfigPath.c_str()))
    {
        return true;
    }

    SKYWALKER_PTR_SCRIPT_NODE RootNode = ConfigParse->GetRootNode();
    if (RootNode == nullptr)
    {
        return true;
    }

    for (size_t i = 0; i < RootNode->GetChildNodeNum(); i++)
    {
        SKYWALKER_PTR_SCRIPT_NODE ConfigNode = RootNode->GetChildNodeFromIndex(i);
        if (ConfigNode == nullptr)
        {
            continue;
        }

        SKYWALKER_PTR_SCRIPT_NODE AdminNode = ConfigNode->GetChildNodeFromName("AdminACL_Admin");
        if (AdminNode != nullptr)
        {
            ApplyCommandsFromConfig(AdminNode->GetNodeValueString(), AdminOnlyCommands);
        }

        SKYWALKER_PTR_SCRIPT_NODE OperatorNode = ConfigNode->GetChildNodeFromName("AdminACL_Operator");
        if (OperatorNode != nullptr)
        {
            ApplyCommandsFromConfig(OperatorNode->GetNodeValueString(), OperatorCommands);
        }

        SKYWALKER_PTR_SCRIPT_NODE ObserverNode = ConfigNode->GetChildNodeFromName("AdminACL_Observer");
        if (ObserverNode != nullptr)
        {
            ApplyCommandsFromConfig(ObserverNode->GetNodeValueString(), ObserverCommands);
        }
    }

    return true;
}

void SSFModule_AdminCommand::ApplyCommandsFromConfig(const SFString &RawValue, std::unordered_set<SFString> &CommandSet)
{
    if (RawValue.empty())
    {
        return;
    }

    std::unordered_set<SFString> ParsedSet;
    SFString Normalized = RawValue;
    std::replace(Normalized.begin(), Normalized.end(), ',', '|');

    std::stringstream Stream(Normalized);
    SFString Item;
    while (std::getline(Stream, Item, '|'))
    {
        const size_t LeftPos = Item.find_first_not_of(" \t\r\n");
        if (LeftPos == SFString::npos)
        {
            continue;
        }

        const size_t RightPos = Item.find_last_not_of(" \t\r\n");
        SFString Trimmed = Item.substr(LeftPos, RightPos - LeftPos + 1);
        if (!Trimmed.empty())
        {
            ParsedSet.insert(Trimmed);
        }
    }

    if (!ParsedSet.empty())
    {
        CommandSet = std::move(ParsedSet);
    }
}

SFUInt64 SSFModule_AdminCommand::GetAuditItemCount(const SFString &AuditText) const
{
    if (AuditText.empty() ||
        AuditText == "AIAuditUnavailable" ||
        AuditText == "AIStrategyAudit=Empty")
    {
        return 0;
    }

    const SFString Prefix = "AIStrategyAudit=";
    if (AuditText.rfind(Prefix, 0) != 0)
    {
        return 0;
    }

    SFString Body = AuditText.substr(Prefix.size());
    if (Body.empty())
    {
        return 0;
    }

    SFUInt64 Count = 1;
    for (const char Ch : Body)
    {
        if (Ch == '|')
        {
            ++Count;
        }
    }

    return Count;
}
