/*************************************************************************
**文件: SkywalkerFramework\Plugin\Actor\Object\SFPlayer.h
**作者: shyfan
**日期: 2026/03/19
**功能: 玩家实体
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_ACTOR_OBJECT_SFPLAYER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_ACTOR_OBJECT_SFPLAYER_H__

#include "SFCreature.h"

SF_NAMESPACE_BEGIN

class SFPlayer : public SFCreature
{
public:
    SFUInt32 PlayerId = 0;
    SFUInt32 SessionId = 0;
    SFString PlayerName;

public:
    SFPlayer()
    {
        ActorType = ESFActorType::Player;
        HP = 100;
        MaxHP = 100;
        MoveSpeed = 5.0f;
    }
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_ACTOR_OBJECT_SFPLAYER_H__ */
