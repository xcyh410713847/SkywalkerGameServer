/*************************************************************************
**文件: SkywalkerFramework\Include\SFActorTypes.h
**作者: shyfan
**日期: 2026/03/19
**功能: Actor 实体公共类型定义（供跨插件访问）
*************************************************************************/

#ifndef __SKYWALKER_FRAMEWORK_SFACTOR_TYPES_H__
#define __SKYWALKER_FRAMEWORK_SFACTOR_TYPES_H__

#include "SFCore.h"

#include <vector>
#include <cmath>

SF_NAMESPACE_BEGIN

/*------------------------------------------------------------------------
 * Actor 类型枚举
 *----------------------------------------------------------------------*/
enum class ESFActorType : SFUInt8
{
    None        = 0,
    Player      = 1,
    NPC         = 2,
    Monster     = 3,
};

/*------------------------------------------------------------------------
 * 3D 向量（位置/方向）
 *----------------------------------------------------------------------*/
struct SFVector3
{
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;

    SFVector3() = default;
    SFVector3(float InX, float InY, float InZ) : X(InX), Y(InY), Z(InZ) {}

    float DistanceTo(const SFVector3 &Other) const
    {
        float DX = X - Other.X;
        float DY = Y - Other.Y;
        float DZ = Z - Other.Z;
        return std::sqrt(DX * DX + DY * DY + DZ * DZ);
    }

    SFVector3 operator-(const SFVector3 &Other) const
    {
        return SFVector3(X - Other.X, Y - Other.Y, Z - Other.Z);
    }

    SFVector3 operator+(const SFVector3 &Other) const
    {
        return SFVector3(X + Other.X, Y + Other.Y, Z + Other.Z);
    }

    SFVector3 operator*(float Scalar) const
    {
        return SFVector3(X * Scalar, Y * Scalar, Z * Scalar);
    }

    float Length() const
    {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }

    SFVector3 Normalized() const
    {
        float Len = Length();
        if (Len < 0.0001f) return SFVector3(0, 0, 0);
        return SFVector3(X / Len, Y / Len, Z / Len);
    }
};

/*------------------------------------------------------------------------
 * Actor 唯一ID 类型
 *----------------------------------------------------------------------*/
typedef SFUInt32 SFActorId;

/*------------------------------------------------------------------------
 * Actor 序列化信息（用于网络传输 ActorEnterScene 等）
 *
 *  ActorType(u8) + ActorId(u32) + PosX(f32) + PosY(f32) + PosZ(f32)
 *  + Yaw(f32) + HP(u32) + MaxHP(u32)
 *  Total = 1 + 4 + 4*3 + 4 + 4 + 4 = 29 bytes
 *----------------------------------------------------------------------*/
static const SFSize SF_ACTOR_INFO_SIZE = 29;

/*------------------------------------------------------------------------
 * ISFActorFactory — Actor 工厂抽象接口
 *
 * 由 Actor 插件的 SFModule_ActorFactory 实现。
 * 其他插件通过 PluginManager 获取后 dynamic_cast 使用。
 *----------------------------------------------------------------------*/
class ISFActorFactory
{
public:
    virtual ~ISFActorFactory() = default;

    /** 创建玩家 */
    virtual SFActorId CreatePlayer(SFUInt32 PlayerId, SFUInt32 SessionId,
                                    const SFString &PlayerName) = 0;

    /** 创建 NPC */
    virtual SFActorId CreateNPC(SFUInt32 NPCId, const SFVector3 &SpawnPos,
                                 const std::vector<SFVector3> &PatrolPath) = 0;

    /** 创建怪物 */
    virtual SFActorId CreateMonster(SFUInt32 MonsterId, const SFVector3 &SpawnPos,
                                     float AggroRange) = 0;

    /** 销毁 Actor */
    virtual void DestroyActor(SFActorId ActorId) = 0;

    /** 获取 ActorType */
    virtual ESFActorType GetActorType(SFActorId ActorId) = 0;

    /** 获取 Actor 位置 */
    virtual SFVector3 GetActorPosition(SFActorId ActorId) = 0;

    /** 设置 Actor 位置 */
    virtual void SetActorPosition(SFActorId ActorId, const SFVector3 &Pos) = 0;

    /** 获取 Actor 朝向 */
    virtual float GetActorYaw(SFActorId ActorId) = 0;

    /** 获取 Actor HP/MaxHP */
    virtual SFUInt32 GetActorHP(SFActorId ActorId) = 0;
    virtual SFUInt32 GetActorMaxHP(SFActorId ActorId) = 0;

    /** 获取 Actor 的 SessionId (仅 Player 有效) */
    virtual SFUInt32 GetActorSessionId(SFActorId ActorId) = 0;

    /** 获取 Actor 的 PlayerId (仅 Player 有效) */
    virtual SFUInt32 GetActorPlayerId(SFActorId ActorId) = 0;

    /** 通过 PlayerId 找 ActorId */
    virtual SFActorId FindActorByPlayerId(SFUInt32 PlayerId) = 0;

    /** 序列化 Actor 信息到 buffer (返回写入字节数) */
    virtual SFSize SerializeActorInfo(SFActorId ActorId, char *OutBuffer, SFSize BufferSize) = 0;

    /** Tick 所有 Actor */
    virtual void TickAllActors(SFUInt64 DeltaMS) = 0;

    /** 获取所有 ActorId 列表 */
    virtual std::vector<SFActorId> GetAllActorIds() = 0;

    /** 获取 Actor 移动速度 (Creature 子类有效) */
    virtual float GetActorMoveSpeed(SFActorId ActorId) = 0;

    /** NPC 巡逻路径 */
    virtual std::vector<SFVector3> GetNPCPatrolPath(SFActorId ActorId) = 0;
    virtual SFUInt32 GetNPCPatrolIndex(SFActorId ActorId) = 0;
    virtual void SetNPCPatrolIndex(SFActorId ActorId, SFUInt32 Index) = 0;

    /** Monster 出生点与仇恨范围 */
    virtual SFVector3 GetMonsterSpawnPosition(SFActorId ActorId) = 0;
    virtual float GetMonsterAggroRange(SFActorId ActorId) = 0;
};

/*------------------------------------------------------------------------
 * ISFSceneManager — 场景管理抽象接口
 *
 * 由 Level 插件的 SFModule_SceneManager 实现。
 *----------------------------------------------------------------------*/
class ISFSceneManager
{
public:
    virtual ~ISFSceneManager() = default;

    /** 将 Actor 加入场景 */
    virtual bool AddActorToScene(SFUInt32 SceneId, SFActorId ActorId) = 0;

    /** 将 Actor 从场景移除 */
    virtual bool RemoveActorFromScene(SFUInt32 SceneId, SFActorId ActorId) = 0;

    /** 获取场景内所有 ActorId */
    virtual std::vector<SFActorId> GetSceneActors(SFUInt32 SceneId) = 0;

    /** 获取场景内所有玩家的 SessionId */
    virtual std::vector<SFUInt32> GetScenePlayerSessionIds(SFUInt32 SceneId) = 0;

    /** 获取 Actor 所在场景ID (0=不在任何场景) */
    virtual SFUInt32 GetActorSceneId(SFActorId ActorId) = 0;

    /** 获取默认场景ID */
    virtual SFUInt32 GetDefaultSceneId() = 0;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_FRAMEWORK_SFACTOR_TYPES_H__ */
