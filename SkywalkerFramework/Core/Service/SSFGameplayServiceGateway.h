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

    bool ValidateToken(const SFString &Token) const;
    bool LoadPlayer(SFUInt64 PlayerId) const;
    bool EnterWorld(SFUInt64 PlayerId, SFUInt64 WorldId) const;
    bool LeaveWorld(SFUInt64 PlayerId, SFUInt64 WorldId) const;
    bool StartReplayRecord(SFUInt64 SessionId) const;
    bool StopReplayRecord() const;
    bool StartReplayPlay(SFUInt64 SessionId) const;
    bool StopReplayPlay() const;

private:
    AuthValidator Validator;
    PlayerLoader Loader;
    WorldEnter Enter;
    WorldLeave Leave;
    ReplayStartRecord StartRecord;
    ReplayStopRecord StopRecord;
    ReplayStartPlay StartPlay;
    ReplayStopPlay StopPlay;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFGAMEPLAYSERVICEGATEWAY_H__
