/*************************************************************************
**文件: SkywalkerFramework\Plugin\Actor\Object\SFCreature.h
**作者: shyfan
**日期: 2026/03/19
**功能: 生物实体基类
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_ACTOR_OBJECT_SFCREATURE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_ACTOR_OBJECT_SFCREATURE_H__

#include "SFActor.h"

SF_NAMESPACE_BEGIN

class SFCreature : public SFActor
{
public:
    SFUInt32 HP = 0;
    SFUInt32 MaxHP = 0;
    float MoveSpeed = 0.0f;
    bool bAlive = true;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_ACTOR_OBJECT_SFCREATURE_H__ */
