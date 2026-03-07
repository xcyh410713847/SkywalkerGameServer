/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SFGameplayServiceGateway.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 游戏业务服务网关
*************************************************************************/

#include "SFGameplayServiceGateway.h"

SF_NAMESPACE_USING

/** 获取网关单例 */
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

void SSFGameplayServiceGateway::RegisterReplayGetEventsRange(const ReplayGetEventsRange &InGetEventsRange)
{
    GetEventsRange = InGetEventsRange;
}

void SSFGameplayServiceGateway::RegisterReplayFindEventsByKeyword(const ReplayFindEventsByKeyword &InFindEventsByKeyword)
{
    FindEventsByKeyword = InFindEventsByKeyword;
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

void SSFGameplayServiceGateway::RegisterAIClearAudit(const AIClearAudit &InClearAudit)
{
    ClearAudit = InClearAudit;
}

bool SSFGameplayServiceGateway::ValidateToken(const SFString &Token) const
{
    // 未注册回调时返回 false，确保默认拒绝
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
    // 未注册回调时返回占位字符串
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

SFString SSFGameplayServiceGateway::GetReplayEventsRange(SFUInt64 StartIndex, SFUInt64 Count) const
{
    if (!GetEventsRange)
    {
        return "ReplayEventsRangeUnavailable";
    }

    return GetEventsRange(StartIndex, Count);
}

SFString SSFGameplayServiceGateway::FindReplayEventsByKeyword(const SFString &Keyword, SFUInt64 MaxCount) const
{
    if (!FindEventsByKeyword)
    {
        return "ReplayFindEventsUnavailable";
    }

    return FindEventsByKeyword(Keyword, MaxCount);
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
    // 未注册回调时返回占位字符串
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

bool SSFGameplayServiceGateway::ClearAIAudit() const
{
    if (!ClearAudit)
    {
        return false;
    }

    return ClearAudit();
}
