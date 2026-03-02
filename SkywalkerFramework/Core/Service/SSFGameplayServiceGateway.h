/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SSFGameplayServiceGateway.h
**作者: shyfan
**日期: 2026/03/02
**功能: 游戏业务服务网关
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFGAMEPLAYSERVICEGATEWAY_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFGAMEPLAYSERVICEGATEWAY_H__

#include "Include/SFCore.h"

#include <functional>

SF_NAMESPACE_BEGIN

class SSFGameplayServiceGateway
{
public:
    using AuthValidator = std::function<bool(const SFString &Token)>;
    using PlayerLoader = std::function<bool(SFUInt64 PlayerId)>;
    using WorldEnter = std::function<bool(SFUInt64 PlayerId, SFUInt64 WorldId)>;
    using WorldLeave = std::function<bool(SFUInt64 PlayerId, SFUInt64 WorldId)>;
    using ReplayStartRecord = std::function<bool(SFUInt64 SessionId)>;
    using ReplayStopRecord = std::function<bool()>;
    using ReplayStartPlay = std::function<bool(SFUInt64 SessionId)>;
    using ReplayStopPlay = std::function<bool()>;
    using ReplayRecordStats = std::function<SFString()>;
    using ReplayPlayStats = std::function<SFString()>;
    using ReplayGetEventByIndex = std::function<SFString(SFUInt64 EventIndex)>;
    using AISetStrategy = std::function<bool(const SFString &StrategyName)>;
    using AIGetStats = std::function<SFString()>;
    using AIGetStrategies = std::function<SFString()>;
    using AIGetAudit = std::function<SFString()>;

public:
    static SSFGameplayServiceGateway &Instance();

    void RegisterAuthValidator(const AuthValidator &InValidator);
    void RegisterPlayerLoader(const PlayerLoader &InLoader);
    void RegisterWorldEnter(const WorldEnter &InEnter);
    void RegisterWorldLeave(const WorldLeave &InLeave);
    void RegisterReplayStartRecord(const ReplayStartRecord &InStartRecord);
    void RegisterReplayStopRecord(const ReplayStopRecord &InStopRecord);
    void RegisterReplayStartPlay(const ReplayStartPlay &InStartPlay);
    void RegisterReplayStopPlay(const ReplayStopPlay &InStopPlay);
    void RegisterReplayRecordStats(const ReplayRecordStats &InRecordStats);
    void RegisterReplayPlayStats(const ReplayPlayStats &InPlayStats);
    void RegisterReplayGetEventByIndex(const ReplayGetEventByIndex &InGetEventByIndex);
    void RegisterAISetStrategy(const AISetStrategy &InSetStrategy);
    void RegisterAIGetStats(const AIGetStats &InGetStats);
    void RegisterAIGetStrategies(const AIGetStrategies &InGetStrategies);
    void RegisterAIGetAudit(const AIGetAudit &InGetAudit);

    bool ValidateToken(const SFString &Token) const;
    bool LoadPlayer(SFUInt64 PlayerId) const;
    bool EnterWorld(SFUInt64 PlayerId, SFUInt64 WorldId) const;
    bool LeaveWorld(SFUInt64 PlayerId, SFUInt64 WorldId) const;
    bool StartReplayRecord(SFUInt64 SessionId) const;
    bool StopReplayRecord() const;
    bool StartReplayPlay(SFUInt64 SessionId) const;
    bool StopReplayPlay() const;
    SFString GetReplayRecordStats() const;
    SFString GetReplayPlayStats() const;
    SFString GetReplayEventByIndex(SFUInt64 EventIndex) const;
    bool SetAIStrategy(const SFString &StrategyName) const;
    SFString GetAIStats() const;
    SFString GetAIStrategies() const;
    SFString GetAIAudit() const;

private:
    AuthValidator Validator;
    PlayerLoader Loader;
    WorldEnter Enter;
    WorldLeave Leave;
    ReplayStartRecord StartRecord;
    ReplayStopRecord StopRecord;
    ReplayStartPlay StartPlay;
    ReplayStopPlay StopPlay;
    ReplayRecordStats RecordStats;
    ReplayPlayStats PlayStats;
    ReplayGetEventByIndex GetEventByIndex;
    AISetStrategy SetStrategy;
    AIGetStats GetStrategyStats;
    AIGetStrategies GetStrategies;
    AIGetAudit GetAudit;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFGAMEPLAYSERVICEGATEWAY_H__
