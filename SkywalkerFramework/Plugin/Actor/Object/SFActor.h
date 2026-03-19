/*************************************************************************
**文件: SkywalkerFramework\Plugin\Actor\Object\SFActor.h
**作者: shyfan
**日期: 2026/03/19
**功能: 场景实体基类
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_ACTOR_OBJECT_SFACTOR_H__
#define __SKYWALKER_SERVER_FRAMEWORK_ACTOR_OBJECT_SFACTOR_H__

#include "Include/SFCore.h"
#include "Include/SFActorTypes.h"

SF_NAMESPACE_BEGIN

class SFActor
{
public:
    SFActorId ActorId = 0;
    ESFActorType ActorType = ESFActorType::None;
    SFVector3 Position;
    float Yaw = 0.0f;
    SFUInt32 SceneId = 0;
    bool bActive = true;

public:
    virtual void Tick(SFUInt64 DeltaMS) {}
    virtual ~SFActor() = default;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_ACTOR_OBJECT_SFACTOR_H__ */
