/*************************************************************************
**文件: SkywalkerFramework\Plugin\AI\Module\SFModule_AIRuntime.cpp
**作者: shyfan
**日期: 2026/03/19
**功能: AI运行时模块
*************************************************************************/

#include "SFModule_AIRuntime.h"

#include "Include/SFCore.h"
#include "Include/SFILog.h"
#include "Include/SFActorTypes.h"
#include "Include/SFNetworkInterface.h"

#include "Core/Plugin/SFPlugin.h"
#include "Core/Plugin/SFPluginManager.h"

#include <cstring>

#if defined(SKYWALKER_PLATFORM_WINDOWS)
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_AIRuntime, Framework);

/* 巡逻到达路点的距离阈值 */
static const float PATROL_ARRIVE_THRESHOLD = 0.5f;

/* Monster 返回出生点的距离阈值 */
static const float RETURN_ARRIVE_THRESHOLD = 0.5f;

#pragma region Object

void SFModule_AIRuntime::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
}

void SFModule_AIRuntime::Awake(SFObjectErrors &Errors)
{
    SSFModule::Awake(Errors);

    NetworkServer = FindNetworkServer();
    ActorFactory = FindActorFactory();
    SceneManager = FindSceneManager();

    if (NetworkServer == nullptr)
    {
        SF_LOG_ERROR("AIRuntime: NetworkServer not found");
    }
    if (ActorFactory == nullptr)
    {
        SF_LOG_ERROR("AIRuntime: ActorFactory not found");
    }
    if (SceneManager == nullptr)
    {
        SF_LOG_ERROR("AIRuntime: SceneManager not found");
    }

    SF_LOG_FRAMEWORK("AIRuntime initialized");
}

void SFModule_AIRuntime::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);
}

void SFModule_AIRuntime::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);

    if (ActorFactory == nullptr || SceneManager == nullptr || NetworkServer == nullptr)
    {
        return;
    }

    float DeltaSec = static_cast<float>(DelayMS) / 1000.0f;
    if (DeltaSec <= 0.0f || DeltaSec > 1.0f)
    {
        /* 防止过大的 delta（首帧或卡顿），限制到合理范围 */
        DeltaSec = 0.016f;
    }

    std::vector<SFActorId> AllActors = ActorFactory->GetAllActorIds();
    for (SFActorId ActorId : AllActors)
    {
        ESFActorType ActorType = ActorFactory->GetActorType(ActorId);
        if (ActorType == ESFActorType::NPC)
        {
            TickNPCPatrol(ActorId, DeltaSec);
        }
        else if (ActorType == ESFActorType::Monster)
        {
            TickMonsterChase(ActorId, DeltaSec);
        }
    }
}

void SFModule_AIRuntime::Stop(SFObjectErrors &Errors)
{
    SSFModule::Stop(Errors);
}

void SFModule_AIRuntime::Sleep(SFObjectErrors &Errors)
{
    SSFModule::Sleep(Errors);
}

void SFModule_AIRuntime::Destroy(SFObjectErrors &Errors)
{
    SSFModule::Destroy(Errors);
}

#pragma endregion Object

void SFModule_AIRuntime::TickNPCPatrol(SFActorId ActorId, float DeltaSec)
{
    std::vector<SFVector3> PatrolPath = ActorFactory->GetNPCPatrolPath(ActorId);
    if (PatrolPath.empty())
    {
        return;
    }

    SFUInt32 PatrolIndex = ActorFactory->GetNPCPatrolIndex(ActorId);
    if (PatrolIndex >= static_cast<SFUInt32>(PatrolPath.size()))
    {
        PatrolIndex = 0;
        ActorFactory->SetNPCPatrolIndex(ActorId, 0);
    }

    SFVector3 CurrentPos = ActorFactory->GetActorPosition(ActorId);
    SFVector3 TargetPos = PatrolPath[PatrolIndex];
    float Distance = CurrentPos.DistanceTo(TargetPos);

    if (Distance < PATROL_ARRIVE_THRESHOLD)
    {
        /* 到达当前路点，切换到下一个 */
        SFUInt32 NextIndex = (PatrolIndex + 1) % static_cast<SFUInt32>(PatrolPath.size());
        ActorFactory->SetNPCPatrolIndex(ActorId, NextIndex);
        return;
    }

    /* 朝目标移动 */
    float MoveSpeed = ActorFactory->GetActorMoveSpeed(ActorId);
    float MoveDistance = MoveSpeed * DeltaSec;
    if (MoveDistance > Distance)
    {
        MoveDistance = Distance;
    }

    SFVector3 Direction = (TargetPos - CurrentPos).Normalized();
    SFVector3 NewPos = CurrentPos + Direction * MoveDistance;
    ActorFactory->SetActorPosition(ActorId, NewPos);

    BroadcastActorMove(ActorId, NewPos);
}

void SFModule_AIRuntime::TickMonsterChase(SFActorId ActorId, float DeltaSec)
{
    SFVector3 MonsterPos = ActorFactory->GetActorPosition(ActorId);
    float AggroRange = ActorFactory->GetMonsterAggroRange(ActorId);
    SFVector3 SpawnPos = ActorFactory->GetMonsterSpawnPosition(ActorId);

    /* 获取 Monster 所在场景 */
    SFUInt32 SceneId = SceneManager->GetActorSceneId(ActorId);
    if (SceneId == 0)
    {
        return;
    }

    /* 查找仇恨范围内最近的 Player */
    std::vector<SFActorId> SceneActors = SceneManager->GetSceneActors(SceneId);
    SFActorId ClosestPlayerId = 0;
    float ClosestDistance = AggroRange + 1.0f;

    for (SFActorId OtherActorId : SceneActors)
    {
        if (ActorFactory->GetActorType(OtherActorId) != ESFActorType::Player)
        {
            continue;
        }

        SFVector3 PlayerPos = ActorFactory->GetActorPosition(OtherActorId);
        float Dist = MonsterPos.DistanceTo(PlayerPos);
        if (Dist <= AggroRange && Dist < ClosestDistance)
        {
            ClosestPlayerId = OtherActorId;
            ClosestDistance = Dist;
        }
    }

    SFVector3 TargetPos;
    if (ClosestPlayerId != 0)
    {
        /* 追击最近的 Player */
        TargetPos = ActorFactory->GetActorPosition(ClosestPlayerId);
    }
    else
    {
        /* 无目标，返回出生点 */
        float DistToSpawn = MonsterPos.DistanceTo(SpawnPos);
        if (DistToSpawn < RETURN_ARRIVE_THRESHOLD)
        {
            /* 已经在出生点附近，不移动 */
            return;
        }
        TargetPos = SpawnPos;
    }

    float Distance = MonsterPos.DistanceTo(TargetPos);
    if (Distance < 0.1f)
    {
        return;
    }

    float MoveSpeed = ActorFactory->GetActorMoveSpeed(ActorId);
    float MoveDistance = MoveSpeed * DeltaSec;
    if (MoveDistance > Distance)
    {
        MoveDistance = Distance;
    }

    SFVector3 Direction = (TargetPos - MonsterPos).Normalized();
    SFVector3 NewPos = MonsterPos + Direction * MoveDistance;
    ActorFactory->SetActorPosition(ActorId, NewPos);

    BroadcastActorMove(ActorId, NewPos);
}

void SFModule_AIRuntime::BroadcastActorMove(SFActorId ActorId, const SFVector3 &Pos)
{
    SFUInt32 SceneId = SceneManager->GetActorSceneId(ActorId);
    if (SceneId == 0)
    {
        return;
    }

    std::vector<SFUInt32> SessionIds = SceneManager->GetScenePlayerSessionIds(SceneId);
    if (SessionIds.empty())
    {
        return;
    }

    char MovePayload[16] = {};
    SFUInt32 NetActorId = htonl(ActorId);
    std::memcpy(MovePayload, &NetActorId, 4);
    std::memcpy(MovePayload + 4, &Pos.X, 4);
    std::memcpy(MovePayload + 8, &Pos.Y, 4);
    std::memcpy(MovePayload + 12, &Pos.Z, 4);

    NetworkServer->BroadcastTo(SessionIds, SF_MSGID_MOVE_BROADCAST, MovePayload, 16);
}

ISFNetworkServer *SFModule_AIRuntime::FindNetworkServer()
{
    SF_PTR(SFPlugin) OwnerPlugin = GetPlugin();
    if (SF_PTR_INVALID(OwnerPlugin))
    {
        return nullptr;
    }

    SF_PTR(SFPluginManager) PluginMgr = OwnerPlugin->GetPluginManager();
    if (SF_PTR_INVALID(PluginMgr))
    {
        return nullptr;
    }

    SFString PluginName = "Skywalker::Framework::SFPlugin_Network";
    SF_PTR(SFPlugin) Plugin = PluginMgr->GetPlugin(PluginName);
    if (SF_PTR_INVALID(Plugin))
    {
        return nullptr;
    }

    SFString ModuleName = "Skywalker::Framework::SFModule_NetworkServer";
    SF_PTR(SSFModule) Module = Plugin->GetModule(ModuleName);
    if (SF_PTR_INVALID(Module))
    {
        return nullptr;
    }

    return dynamic_cast<ISFNetworkServer *>(Module);
}

ISFActorFactory *SFModule_AIRuntime::FindActorFactory()
{
    SF_PTR(SFPlugin) OwnerPlugin = GetPlugin();
    if (SF_PTR_INVALID(OwnerPlugin))
    {
        return nullptr;
    }

    SF_PTR(SFPluginManager) PluginMgr = OwnerPlugin->GetPluginManager();
    if (SF_PTR_INVALID(PluginMgr))
    {
        return nullptr;
    }

    SFString PluginName = "Skywalker::Framework::SFPlugin_Actor";
    SF_PTR(SFPlugin) Plugin = PluginMgr->GetPlugin(PluginName);
    if (SF_PTR_INVALID(Plugin))
    {
        return nullptr;
    }

    SFString ModuleName = "Skywalker::Framework::SFModule_ActorFactory";
    SF_PTR(SSFModule) Module = Plugin->GetModule(ModuleName);
    if (SF_PTR_INVALID(Module))
    {
        return nullptr;
    }

    return dynamic_cast<ISFActorFactory *>(Module);
}

ISFSceneManager *SFModule_AIRuntime::FindSceneManager()
{
    SF_PTR(SFPlugin) OwnerPlugin = GetPlugin();
    if (SF_PTR_INVALID(OwnerPlugin))
    {
        return nullptr;
    }

    SF_PTR(SFPluginManager) PluginMgr = OwnerPlugin->GetPluginManager();
    if (SF_PTR_INVALID(PluginMgr))
    {
        return nullptr;
    }

    SFString PluginName = "Skywalker::Framework::SFPlugin_Level";
    SF_PTR(SFPlugin) Plugin = PluginMgr->GetPlugin(PluginName);
    if (SF_PTR_INVALID(Plugin))
    {
        return nullptr;
    }

    SFString ModuleName = "Skywalker::Framework::SFModule_SceneManager";
    SF_PTR(SSFModule) Module = Plugin->GetModule(ModuleName);
    if (SF_PTR_INVALID(Module))
    {
        return nullptr;
    }

    return dynamic_cast<ISFSceneManager *>(Module);
}
