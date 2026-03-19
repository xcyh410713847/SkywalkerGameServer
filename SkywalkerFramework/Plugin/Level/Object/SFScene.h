/*************************************************************************
**文件: SkywalkerFramework\Plugin\Level\Object\SFScene.h
**作者: shyfan
**日期: 2026/03/19
**功能: 场景对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_LEVEL_OBJECT_SFSCENE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_LEVEL_OBJECT_SFSCENE_H__

#include "Include/SFCore.h"
#include "Include/SFActorTypes.h"

#include <vector>

SF_NAMESPACE_BEGIN

class SFScene
{
public:
    SFUInt32 SceneId = 0;
    SFString SceneName;

public:
    void AddActor(SFActorId ActorId)
    {
        if (!HasActor(ActorId))
        {
            ActorIds.push_back(ActorId);
        }
    }

    void RemoveActor(SFActorId ActorId)
    {
        for (auto Iter = ActorIds.begin(); Iter != ActorIds.end(); ++Iter)
        {
            if (*Iter == ActorId)
            {
                ActorIds.erase(Iter);
                return;
            }
        }
    }

    bool HasActor(SFActorId ActorId) const
    {
        for (SFActorId Id : ActorIds)
        {
            if (Id == ActorId)
            {
                return true;
            }
        }
        return false;
    }

    const std::vector<SFActorId> &GetActorIds() const
    {
        return ActorIds;
    }

private:
    std::vector<SFActorId> ActorIds;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_LEVEL_OBJECT_SFSCENE_H__ */
