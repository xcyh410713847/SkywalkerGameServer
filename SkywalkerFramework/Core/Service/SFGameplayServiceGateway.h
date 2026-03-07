/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SFGameplayServiceGateway.h
**作者: shyfan
**日期: 2026/03/02
**功能: 游戏业务服务网关
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFGAMEPLAYSERVICEGATEWAY_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFGAMEPLAYSERVICEGATEWAY_H__

#include "Include/SFCore.h"

#include <functional>

SF_NAMESPACE_BEGIN

/**
 * 游戏业务服务网关（单例）
 *
 * 说明：
 * 1. 通过 std::function 注册业务回调
 * 2. 向上层提供统一调用入口
 * 3. 当回调未注册时返回安全默认值
 */
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
    using ReplayGetEventsRange = std::function<SFString(SFUInt64 StartIndex, SFUInt64 Count)>;
    using ReplayFindEventsByKeyword = std::function<SFString(const SFString &Keyword, SFUInt64 MaxCount)>;
    using AISetStrategy = std::function<bool(const SFString &StrategyName)>;
    using AIGetStats = std::function<SFString()>;
    using AIGetStrategies = std::function<SFString()>;
    using AIGetAudit = std::function<SFString()>;
    using AIClearAudit = std::function<bool()>;

public:
    /** 获取单例实例 */
    static SSFGameplayServiceGateway &Instance();

    /** 注册鉴权回调 */
    void RegisterAuthValidator(const AuthValidator &InValidator);
    /** 注册加载玩家回调 */
    void RegisterPlayerLoader(const PlayerLoader &InLoader);
    /** 注册进入世界回调 */
    void RegisterWorldEnter(const WorldEnter &InEnter);
    /** 注册离开世界回调 */
    void RegisterWorldLeave(const WorldLeave &InLeave);
    /** 注册回放开始录制回调 */
    void RegisterReplayStartRecord(const ReplayStartRecord &InStartRecord);
    /** 注册回放停止录制回调 */
    void RegisterReplayStopRecord(const ReplayStopRecord &InStopRecord);
    /** 注册回放开始播放回调 */
    void RegisterReplayStartPlay(const ReplayStartPlay &InStartPlay);
    /** 注册回放停止播放回调 */
    void RegisterReplayStopPlay(const ReplayStopPlay &InStopPlay);
    /** 注册录制统计查询回调 */
    void RegisterReplayRecordStats(const ReplayRecordStats &InRecordStats);
    /** 注册播放统计查询回调 */
    void RegisterReplayPlayStats(const ReplayPlayStats &InPlayStats);
    /** 注册按索引获取回放事件回调 */
    void RegisterReplayGetEventByIndex(const ReplayGetEventByIndex &InGetEventByIndex);
    /** 注册范围获取回放事件回调 */
    void RegisterReplayGetEventsRange(const ReplayGetEventsRange &InGetEventsRange);
    /** 注册按关键字检索回放事件回调 */
    void RegisterReplayFindEventsByKeyword(const ReplayFindEventsByKeyword &InFindEventsByKeyword);
    /** 注册 AI 策略设置回调 */
    void RegisterAISetStrategy(const AISetStrategy &InSetStrategy);
    /** 注册 AI 统计查询回调 */
    void RegisterAIGetStats(const AIGetStats &InGetStats);
    /** 注册 AI 策略列表查询回调 */
    void RegisterAIGetStrategies(const AIGetStrategies &InGetStrategies);
    /** 注册 AI 审计查询回调 */
    void RegisterAIGetAudit(const AIGetAudit &InGetAudit);
    /** 注册 AI 审计清理回调 */
    void RegisterAIClearAudit(const AIClearAudit &InClearAudit);

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
    SFString GetReplayEventsRange(SFUInt64 StartIndex, SFUInt64 Count) const;
    SFString FindReplayEventsByKeyword(const SFString &Keyword, SFUInt64 MaxCount) const;
    bool SetAIStrategy(const SFString &StrategyName) const;
    SFString GetAIStats() const;
    SFString GetAIStrategies() const;
    SFString GetAIAudit() const;
    bool ClearAIAudit() const;

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
    ReplayGetEventsRange GetEventsRange;
    ReplayFindEventsByKeyword FindEventsByKeyword;
    AISetStrategy SetStrategy;
    AIGetStats GetStrategyStats;
    AIGetStrategies GetStrategies;
    AIGetAudit GetAudit;
    AIClearAudit ClearAudit;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFGAMEPLAYSERVICEGATEWAY_H__
