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
