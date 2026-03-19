/*************************************************************************
**文件: SkywalkerFramework\Plugin\Actor\Module\SFModule_ActorFactory.cpp
**作者: shyfan
**日期: 2026/03/19
**功能: Actor工厂模块
*************************************************************************/

#include "SFModule_ActorFactory.h"

#include "Include/SFCore.h"
#include "Include/SFILog.h"

#include "../Object/SFPlayer.h"
#include "../Object/SFNPC.h"
#include "../Object/SFMonster.h"

#include <cstring>

#if defined(SKYWALKER_PLATFORM_WINDOWS)
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_ActorFactory, Framework);

#pragma region Object

void SFModule_ActorFactory::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
}

void SFModule_ActorFactory::Awake(SFObjectErrors &Errors)
{
    SSFModule::Awake(Errors);
}

void SFModule_ActorFactory::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);
}

void SFModule_ActorFactory::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);
}

void SFModule_ActorFactory::Stop(SFObjectErrors &Errors)
{
    ClearActors();
    SSFModule::Stop(Errors);
}

void SFModule_ActorFactory::Sleep(SFObjectErrors &Errors)
{
    SSFModule::Sleep(Errors);
}

void SFModule_ActorFactory::Destroy(SFObjectErrors &Errors)
{
    ClearActors();
    SSFModule::Destroy(Errors);
}

#pragma endregion Object

#pragma region ISFActorFactory

SFActorId SFModule_ActorFactory::CreatePlayer(SFUInt32 PlayerId, SFUInt32 SessionId,
                                              const SFString &PlayerName)
{
    SFPlayer *Player = new SFPlayer();
    Player->ActorId = NextActorId++;
    Player->PlayerId = PlayerId;
    Player->SessionId = SessionId;
    Player->PlayerName = PlayerName;
    ActorMap.insert(std::make_pair(Player->ActorId, Player));
    return Player->ActorId;
}

SFActorId SFModule_ActorFactory::CreateNPC(SFUInt32 NPCId, const SFVector3 &SpawnPos,
                                           const std::vector<SFVector3> &PatrolPath)
{
    SFNPC *NPC = new SFNPC();
    NPC->ActorId = NextActorId++;
    NPC->NPCId = NPCId;
    NPC->Position = SpawnPos;
    NPC->PatrolPath = PatrolPath;
    NPC->CurrentPatrolIndex = 0;
    ActorMap.insert(std::make_pair(NPC->ActorId, NPC));
    return NPC->ActorId;
}

SFActorId SFModule_ActorFactory::CreateMonster(SFUInt32 MonsterId, const SFVector3 &SpawnPos,
                                               float AggroRange)
{
    SFMonster *Monster = new SFMonster();
    Monster->ActorId = NextActorId++;
    Monster->MonsterId = MonsterId;
    Monster->Position = SpawnPos;
    Monster->SpawnPosition = SpawnPos;
    Monster->AggroRange = AggroRange;
    ActorMap.insert(std::make_pair(Monster->ActorId, Monster));
    return Monster->ActorId;
}

void SFModule_ActorFactory::DestroyActor(SFActorId ActorId)
{
    auto Iter = ActorMap.find(ActorId);
    if (Iter == ActorMap.end())
    {
        return;
    }

    if (Iter->second != nullptr)
    {
        delete Iter->second;
    }

    ActorMap.erase(Iter);
}

ESFActorType SFModule_ActorFactory::GetActorType(SFActorId ActorId)
{
    SFActor *Actor = FindActor(ActorId);
    return (Actor != nullptr) ? Actor->ActorType : ESFActorType::None;
}

SFVector3 SFModule_ActorFactory::GetActorPosition(SFActorId ActorId)
{
    SFActor *Actor = FindActor(ActorId);
    return (Actor != nullptr) ? Actor->Position : SFVector3();
}

void SFModule_ActorFactory::SetActorPosition(SFActorId ActorId, const SFVector3 &Pos)
{
    SFActor *Actor = FindActor(ActorId);
    if (Actor != nullptr)
    {
        Actor->Position = Pos;
    }
}

float SFModule_ActorFactory::GetActorYaw(SFActorId ActorId)
{
    SFActor *Actor = FindActor(ActorId);
    return (Actor != nullptr) ? Actor->Yaw : 0.0f;
}

SFUInt32 SFModule_ActorFactory::GetActorHP(SFActorId ActorId)
{
    SFActor *Actor = FindActor(ActorId);
    if (Actor == nullptr)
    {
        return 0;
    }

    SFCreature *Creature = dynamic_cast<SFCreature *>(Actor);
    return (Creature != nullptr) ? Creature->HP : 0;
}

SFUInt32 SFModule_ActorFactory::GetActorMaxHP(SFActorId ActorId)
{
    SFActor *Actor = FindActor(ActorId);
    if (Actor == nullptr)
    {
        return 0;
    }

    SFCreature *Creature = dynamic_cast<SFCreature *>(Actor);
    return (Creature != nullptr) ? Creature->MaxHP : 0;
}

SFUInt32 SFModule_ActorFactory::GetActorSessionId(SFActorId ActorId)
{
    SFActor *Actor = FindActor(ActorId);
    if (Actor == nullptr)
    {
        return 0;
    }

    SFPlayer *Player = dynamic_cast<SFPlayer *>(Actor);
    return (Player != nullptr) ? Player->SessionId : 0;
}

SFUInt32 SFModule_ActorFactory::GetActorPlayerId(SFActorId ActorId)
{
    SFActor *Actor = FindActor(ActorId);
    if (Actor == nullptr)
    {
        return 0;
    }

    SFPlayer *Player = dynamic_cast<SFPlayer *>(Actor);
    return (Player != nullptr) ? Player->PlayerId : 0;
}

SFActorId SFModule_ActorFactory::FindActorByPlayerId(SFUInt32 PlayerId)
{
    SF_COMMON_ITERATOR(IterActor, ActorMap)
    {
        SFPlayer *Player = dynamic_cast<SFPlayer *>(IterActor->second);
        if (Player != nullptr && Player->PlayerId == PlayerId)
        {
            return IterActor->first;
        }
    }
    return 0;
}

SFSize SFModule_ActorFactory::SerializeActorInfo(SFActorId ActorId, char *OutBuffer, SFSize BufferSize)
{
    if (OutBuffer == nullptr || BufferSize < SF_ACTOR_INFO_SIZE)
    {
        return 0;
    }

    SFActor *Actor = FindActor(ActorId);
    if (Actor == nullptr)
    {
        return 0;
    }

    OutBuffer[0] = static_cast<char>(Actor->ActorType);

    SFUInt32 NetActorId = htonl(Actor->ActorId);
    std::memcpy(OutBuffer + 1, &NetActorId, 4);

    std::memcpy(OutBuffer + 5, &Actor->Position.X, 4);
    std::memcpy(OutBuffer + 9, &Actor->Position.Y, 4);
    std::memcpy(OutBuffer + 13, &Actor->Position.Z, 4);
    std::memcpy(OutBuffer + 17, &Actor->Yaw, 4);

    SFUInt32 HP = GetActorHP(ActorId);
    SFUInt32 MaxHP = GetActorMaxHP(ActorId);
    SFUInt32 NetHP = htonl(HP);
    SFUInt32 NetMaxHP = htonl(MaxHP);
    std::memcpy(OutBuffer + 21, &NetHP, 4);
    std::memcpy(OutBuffer + 25, &NetMaxHP, 4);

    return SF_ACTOR_INFO_SIZE;
}

void SFModule_ActorFactory::TickAllActors(SFUInt64 DeltaMS)
{
    SF_COMMON_ITERATOR(IterActor, ActorMap)
    {
        SFActor *Actor = IterActor->second;
        if (Actor != nullptr && Actor->bActive)
        {
            Actor->Tick(DeltaMS);
        }
    }
}

std::vector<SFActorId> SFModule_ActorFactory::GetAllActorIds()
{
    std::vector<SFActorId> Ids;
    Ids.reserve(ActorMap.size());
    SF_COMMON_ITERATOR(IterActor, ActorMap)
    {
        Ids.push_back(IterActor->first);
    }
    return Ids;
}

float SFModule_ActorFactory::GetActorMoveSpeed(SFActorId ActorId)
{
    SFActor *Actor = FindActor(ActorId);
    if (Actor == nullptr)
    {
        return 0.0f;
    }
    SFCreature *Creature = dynamic_cast<SFCreature *>(Actor);
    return (Creature != nullptr) ? Creature->MoveSpeed : 0.0f;
}

std::vector<SFVector3> SFModule_ActorFactory::GetNPCPatrolPath(SFActorId ActorId)
{
    SFActor *Actor = FindActor(ActorId);
    if (Actor == nullptr)
    {
        return {};
    }
    SFNPC *NPC = dynamic_cast<SFNPC *>(Actor);
    return (NPC != nullptr) ? NPC->PatrolPath : std::vector<SFVector3>{};
}

SFUInt32 SFModule_ActorFactory::GetNPCPatrolIndex(SFActorId ActorId)
{
    SFActor *Actor = FindActor(ActorId);
    if (Actor == nullptr)
    {
        return 0;
    }
    SFNPC *NPC = dynamic_cast<SFNPC *>(Actor);
    return (NPC != nullptr) ? NPC->CurrentPatrolIndex : 0;
}

void SFModule_ActorFactory::SetNPCPatrolIndex(SFActorId ActorId, SFUInt32 Index)
{
    SFActor *Actor = FindActor(ActorId);
    if (Actor == nullptr)
    {
        return;
    }
    SFNPC *NPC = dynamic_cast<SFNPC *>(Actor);
    if (NPC != nullptr)
    {
        NPC->CurrentPatrolIndex = Index;
    }
}

SFVector3 SFModule_ActorFactory::GetMonsterSpawnPosition(SFActorId ActorId)
{
    SFActor *Actor = FindActor(ActorId);
    if (Actor == nullptr)
    {
        return SFVector3();
    }
    SFMonster *Monster = dynamic_cast<SFMonster *>(Actor);
    return (Monster != nullptr) ? Monster->SpawnPosition : SFVector3();
}

float SFModule_ActorFactory::GetMonsterAggroRange(SFActorId ActorId)
{
    SFActor *Actor = FindActor(ActorId);
    if (Actor == nullptr)
    {
        return 0.0f;
    }
    SFMonster *Monster = dynamic_cast<SFMonster *>(Actor);
    return (Monster != nullptr) ? Monster->AggroRange : 0.0f;
}

#pragma endregion ISFActorFactory

SFActor *SFModule_ActorFactory::FindActor(SFActorId ActorId)
{
    auto Iter = ActorMap.find(ActorId);
    if (Iter == ActorMap.end())
    {
        return nullptr;
    }
    return Iter->second;
}

void SFModule_ActorFactory::ClearActors()
{
    SF_COMMON_ITERATOR(IterActor, ActorMap)
    {
        if (IterActor->second != nullptr)
        {
            delete IterActor->second;
        }
    }
    ActorMap.clear();
}
