/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SSFGameplayServiceGateway.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 游戏业务服务网关
*************************************************************************/

#include "SSFGameplayServiceGateway.h"

SF_NAMESPACE_USING

SSFGameplayServiceGateway &SSFGameplayServiceGateway::Instance()
{
    static SSFGameplayServiceGateway Gateway;
    return Gateway;
}

void SSFGameplayServiceGateway::RegisterAuthValidator(const AuthValidator &InValidator)
{
    Validator = InValidator;
}

void SSFGameplayServiceGateway::RegisterPlayerLoader(const PlayerLoader &InLoader)
{
    Loader = InLoader;
}

void SSFGameplayServiceGateway::RegisterWorldEnter(const WorldEnter &InEnter)
{
    Enter = InEnter;
}

void SSFGameplayServiceGateway::RegisterWorldLeave(const WorldLeave &InLeave)
{
    Leave = InLeave;
}

void SSFGameplayServiceGateway::RegisterReplayStartRecord(const ReplayStartRecord &InStartRecord)
{
    StartRecord = InStartRecord;
}

void SSFGameplayServiceGateway::RegisterReplayStopRecord(const ReplayStopRecord &InStopRecord)
{
    StopRecord = InStopRecord;
}

void SSFGameplayServiceGateway::RegisterReplayStartPlay(const ReplayStartPlay &InStartPlay)
{
    StartPlay = InStartPlay;
}

void SSFGameplayServiceGateway::RegisterReplayStopPlay(const ReplayStopPlay &InStopPlay)
{
    StopPlay = InStopPlay;
}

void SSFGameplayServiceGateway::RegisterReplayRecordStats(const ReplayRecordStats &InRecordStats)
{
    RecordStats = InRecordStats;
}

void SSFGameplayServiceGateway::RegisterReplayPlayStats(const ReplayPlayStats &InPlayStats)
{
    PlayStats = InPlayStats;
}

void SSFGameplayServiceGateway::RegisterReplayGetEventByIndex(const ReplayGetEventByIndex &InGetEventByIndex)
{
    GetEventByIndex = InGetEventByIndex;
}

void SSFGameplayServiceGateway::RegisterAISetStrategy(const AISetStrategy &InSetStrategy)
{
    SetStrategy = InSetStrategy;
}

void SSFGameplayServiceGateway::RegisterAIGetStats(const AIGetStats &InGetStats)
{
    GetStrategyStats = InGetStats;
}

void SSFGameplayServiceGateway::RegisterAIGetStrategies(const AIGetStrategies &InGetStrategies)
{
    GetStrategies = InGetStrategies;
}

void SSFGameplayServiceGateway::RegisterAIGetAudit(const AIGetAudit &InGetAudit)
{
    GetAudit = InGetAudit;
}

bool SSFGameplayServiceGateway::ValidateToken(const SFString &Token) const
{
    if (!Validator)
    {
        return false;
    }

    return Validator(Token);
}

bool SSFGameplayServiceGateway::LoadPlayer(SFUInt64 PlayerId) const
{
    if (!Loader)
    {
        return false;
    }

    return Loader(PlayerId);
}

bool SSFGameplayServiceGateway::EnterWorld(SFUInt64 PlayerId, SFUInt64 WorldId) const
{
    if (!Enter)
    {
        return false;
    }

    return Enter(PlayerId, WorldId);
}

bool SSFGameplayServiceGateway::LeaveWorld(SFUInt64 PlayerId, SFUInt64 WorldId) const
{
    if (!Leave)
    {
        return false;
    }

    return Leave(PlayerId, WorldId);
}

bool SSFGameplayServiceGateway::StartReplayRecord(SFUInt64 SessionId) const
{
    if (!StartRecord)
    {
        return false;
    }

    return StartRecord(SessionId);
}

bool SSFGameplayServiceGateway::StopReplayRecord() const
{
    if (!StopRecord)
    {
        return false;
    }

    return StopRecord();
}

bool SSFGameplayServiceGateway::StartReplayPlay(SFUInt64 SessionId) const
{
    if (!StartPlay)
    {
        return false;
    }

    return StartPlay(SessionId);
}

bool SSFGameplayServiceGateway::StopReplayPlay() const
{
    if (!StopPlay)
    {
        return false;
    }

    return StopPlay();
}

SFString SSFGameplayServiceGateway::GetReplayRecordStats() const
{
    if (!RecordStats)
    {
        return "ReplayRecordStatsUnavailable";
    }

    return RecordStats();
}

SFString SSFGameplayServiceGateway::GetReplayPlayStats() const
{
    if (!PlayStats)
    {
        return "ReplayPlayStatsUnavailable";
    }

    return PlayStats();
}

SFString SSFGameplayServiceGateway::GetReplayEventByIndex(SFUInt64 EventIndex) const
{
    if (!GetEventByIndex)
    {
        return "ReplayEventUnavailable";
    }

    return GetEventByIndex(EventIndex);
}

bool SSFGameplayServiceGateway::SetAIStrategy(const SFString &StrategyName) const
{
    if (!SetStrategy)
    {
        return false;
    }

    return SetStrategy(StrategyName);
}

SFString SSFGameplayServiceGateway::GetAIStats() const
{
    if (!GetStrategyStats)
    {
        return "AIStatsUnavailable";
    }

    return GetStrategyStats();
}

SFString SSFGameplayServiceGateway::GetAIStrategies() const
{
    if (!GetStrategies)
    {
        return "AIStrategiesUnavailable";
    }

    return GetStrategies();
}

SFString SSFGameplayServiceGateway::GetAIAudit() const
{
    if (!GetAudit)
    {
        return "AIAuditUnavailable";
    }

    return GetAudit();
}
