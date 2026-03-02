/*************************************************************************
**文件: SkywalkerFramework\Plugin\Level\Module\SSFModule_WorldManager.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 世界管理模块
*************************************************************************/

#include "SSFModule_WorldManager.h"

#include "Include/SFILog.h"

#include "Core/Service/SSFGameplayServiceGateway.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_WorldManager, ESFLogLevel::Debug);

void SSFModule_WorldManager::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);

    SF_LOG_FRAMEWORK("WorldManager module init");
}

void SSFModule_WorldManager::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

    SSFGameplayServiceGateway::Instance().RegisterWorldEnter(
        [this](SFUInt64 PlayerId, SFUInt64 WorldId)
        {
            return EnterWorld(PlayerId, WorldId);
        });

    SSFGameplayServiceGateway::Instance().RegisterWorldLeave(
        [this](SFUInt64 PlayerId, SFUInt64 WorldId)
        {
            return LeaveWorld(PlayerId, WorldId);
        });

    SF_LOG_FRAMEWORK("WorldManager module start");
}

void SSFModule_WorldManager::Stop(SFObjectErrors &Errors)
{
    SSFGameplayServiceGateway::Instance().RegisterWorldEnter(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterWorldLeave(nullptr);

    WorldPlayers.clear();
    PlayerWorldMap.clear();

    SF_LOG_FRAMEWORK("WorldManager module stop");

    SSFModule::Stop(Errors);
}

void SSFModule_WorldManager::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("WorldManager module destroy");

    SSFModule::Destroy(Errors);
}

bool SSFModule_WorldManager::CreateWorld(SFUInt64 WorldId)
{
    if (WorldId == 0)
    {
        return false;
    }

    WorldPlayers[WorldId];
    return true;
}

bool SSFModule_WorldManager::EnterWorld(SFUInt64 PlayerId, SFUInt64 WorldId)
{
    if (PlayerId == 0 || WorldId == 0)
    {
        return false;
    }

    if (!CreateWorld(WorldId))
    {
        return false;
    }

    auto IterPlayer = PlayerWorldMap.find(PlayerId);
    if (IterPlayer != PlayerWorldMap.end() && IterPlayer->second != WorldId)
    {
        LeaveWorld(PlayerId, IterPlayer->second);
    }

    PlayerWorldMap[PlayerId] = WorldId;
    WorldPlayers[WorldId].insert(PlayerId);

    SF_LOG_FRAMEWORK("EnterWorld, PlayerId " << PlayerId << " WorldId " << WorldId
                                             << " WorldPlayerCount " << WorldPlayers[WorldId].size());
    return true;
}

bool SSFModule_WorldManager::LeaveWorld(SFUInt64 PlayerId, SFUInt64 WorldId)
{
    if (PlayerId == 0 || WorldId == 0)
    {
        return false;
    }

    auto IterPlayer = PlayerWorldMap.find(PlayerId);
    if (IterPlayer == PlayerWorldMap.end() || IterPlayer->second != WorldId)
    {
        return false;
    }

    auto IterWorld = WorldPlayers.find(WorldId);
    if (IterWorld != WorldPlayers.end())
    {
        IterWorld->second.erase(PlayerId);
        if (IterWorld->second.empty())
        {
            WorldPlayers.erase(IterWorld);
        }
    }

    PlayerWorldMap.erase(IterPlayer);

    SF_LOG_FRAMEWORK("LeaveWorld, PlayerId " << PlayerId << " WorldId " << WorldId);
    return true;
}

SFUInt64 SSFModule_WorldManager::GetWorldPlayerCount(SFUInt64 WorldId) const
{
    auto Iter = WorldPlayers.find(WorldId);
    if (Iter == WorldPlayers.end())
    {
        return 0;
    }

    return static_cast<SFUInt64>(Iter->second.size());
}
