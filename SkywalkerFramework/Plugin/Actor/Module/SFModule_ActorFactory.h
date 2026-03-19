/*************************************************************************
**文件: SkywalkerFramework\Plugin\Actor\Module\SFModule_ActorFactory.h
**作者: shyfan
**日期: 2026/03/19
**功能: Actor工厂模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_ACTOR_FACTORY_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_ACTOR_FACTORY_H__

#include "Include/SFCore.h"
#include "Include/SFActorTypes.h"

#include "Core/Module/SFModule.h"

#include "../Object/SFActor.h"

SF_NAMESPACE_BEGIN

class SFModule_ActorFactory : public SSFModule, public ISFActorFactory
{
#pragma region Object

public:
    virtual void Init(SFObjectErrors &Errors) override;
    virtual void Awake(SFObjectErrors &Errors) override;
    virtual void Start(SFObjectErrors &Errors) override;
    virtual void Tick(SFObjectErrors &Errors, SFUInt64 DelayMS) override;
    virtual void Stop(SFObjectErrors &Errors) override;
    virtual void Sleep(SFObjectErrors &Errors) override;
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SFModule_ActorFactory(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors), NextActorId(1)
    {
    }
    virtual ~SFModule_ActorFactory() {}

#pragma region ISFActorFactory

    virtual SFActorId CreatePlayer(SFUInt32 PlayerId, SFUInt32 SessionId,
                                   const SFString &PlayerName) override;

    virtual SFActorId CreateNPC(SFUInt32 NPCId, const SFVector3 &SpawnPos,
                                const std::vector<SFVector3> &PatrolPath) override;

    virtual SFActorId CreateMonster(SFUInt32 MonsterId, const SFVector3 &SpawnPos,
                                    float AggroRange) override;

    virtual void DestroyActor(SFActorId ActorId) override;

    virtual ESFActorType GetActorType(SFActorId ActorId) override;
    virtual SFVector3 GetActorPosition(SFActorId ActorId) override;
    virtual void SetActorPosition(SFActorId ActorId, const SFVector3 &Pos) override;
    virtual float GetActorYaw(SFActorId ActorId) override;
    virtual SFUInt32 GetActorHP(SFActorId ActorId) override;
    virtual SFUInt32 GetActorMaxHP(SFActorId ActorId) override;
    virtual SFUInt32 GetActorSessionId(SFActorId ActorId) override;
    virtual SFUInt32 GetActorPlayerId(SFActorId ActorId) override;
    virtual SFActorId FindActorByPlayerId(SFUInt32 PlayerId) override;
    virtual SFSize SerializeActorInfo(SFActorId ActorId, char *OutBuffer, SFSize BufferSize) override;
    virtual void TickAllActors(SFUInt64 DeltaMS) override;

    virtual std::vector<SFActorId> GetAllActorIds() override;
    virtual float GetActorMoveSpeed(SFActorId ActorId) override;
    virtual std::vector<SFVector3> GetNPCPatrolPath(SFActorId ActorId) override;
    virtual SFUInt32 GetNPCPatrolIndex(SFActorId ActorId) override;
    virtual void SetNPCPatrolIndex(SFActorId ActorId, SFUInt32 Index) override;
    virtual SFVector3 GetMonsterSpawnPosition(SFActorId ActorId) override;
    virtual float GetMonsterAggroRange(SFActorId ActorId) override;

#pragma endregion ISFActorFactory

private:
    SFActor *FindActor(SFActorId ActorId);
    void ClearActors();

private:
    SFMap<SFActorId, SFActor *> ActorMap;
    SFUInt32 NextActorId;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_MODULE_ACTOR_FACTORY_H__ */
