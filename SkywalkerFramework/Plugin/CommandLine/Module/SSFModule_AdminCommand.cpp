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
        "set_ai_strategy",
        "show_replay_stats",
        "start_replay_record",
        "stop_replay_record",
        "start_replay",
        "stop_replay",
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
