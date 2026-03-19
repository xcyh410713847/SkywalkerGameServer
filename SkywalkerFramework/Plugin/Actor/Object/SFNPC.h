/*************************************************************************
**文件: SkywalkerFramework\Plugin\Actor\Object\SFNPC.h
**作者: shyfan
**日期: 2026/03/19
**功能: NPC实体
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_ACTOR_OBJECT_SFNPC_H__
#define __SKYWALKER_SERVER_FRAMEWORK_ACTOR_OBJECT_SFNPC_H__

#include "SFCreature.h"

#include <vector>

SF_NAMESPACE_BEGIN

class SFNPC : public SFCreature
{
public:
    SFUInt32 NPCId = 0;
    std::vector<SFVector3> PatrolPath;
    SFUInt32 CurrentPatrolIndex = 0;

public:
    SFNPC()
    {
        ActorType = ESFActorType::NPC;
        HP = 100;
        MaxHP = 100;
        MoveSpeed = 2.0f;
    }
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_ACTOR_OBJECT_SFNPC_H__ */
