/*************************************************************************
**文件: SkywalkerFramework\Plugin\Actor\Object\SFMonster.h
**作者: shyfan
**日期: 2026/03/19
**功能: 怪物实体
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_ACTOR_OBJECT_SFMONSTER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_ACTOR_OBJECT_SFMONSTER_H__

#include "SFCreature.h"

SF_NAMESPACE_BEGIN

class SFMonster : public SFCreature
{
public:
    SFUInt32 MonsterId = 0;
    SFVector3 SpawnPosition;
    float AggroRange = 0.0f;

public:
    SFMonster()
    {
        ActorType = ESFActorType::Monster;
        HP = 200;
        MaxHP = 200;
        MoveSpeed = 3.0f;
    }
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_ACTOR_OBJECT_SFMONSTER_H__ */
