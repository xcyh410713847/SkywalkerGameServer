/*************************************************************************
**文件: SkywalkerFramework\Plugin\Level\Module\SFModule_SceneManager.h
**作者: shyfan
**日期: 2026/03/19
**功能: 场景管理模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_SCENE_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_SCENE_MANAGER_H__

#include "Include/SFCore.h"
#include "Include/SFActorTypes.h"
#include "Include/SFNetworkInterface.h"

#include "Core/Module/SFModule.h"

#include "../Object/SFScene.h"

SF_NAMESPACE_BEGIN

class SFModule_SceneManager : public SSFModule, public ISFSceneManager
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
    SFModule_SceneManager(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors),
          NetworkServer(nullptr),
          ActorFactory(nullptr),
          DefaultSceneId(1)
    {
    }
    virtual ~SFModule_SceneManager() {}

#pragma region ISFSceneManager

    virtual bool AddActorToScene(SFUInt32 SceneId, SFActorId ActorId) override;
    virtual bool RemoveActorFromScene(SFUInt32 SceneId, SFActorId ActorId) override;
    virtual std::vector<SFActorId> GetSceneActors(SFUInt32 SceneId) override;
    virtual std::vector<SFUInt32> GetScenePlayerSessionIds(SFUInt32 SceneId) override;
    virtual SFUInt32 GetActorSceneId(SFActorId ActorId) override;
    virtual SFUInt32 GetDefaultSceneId() override { return DefaultSceneId; }

#pragma endregion ISFSceneManager

private:
    ISFNetworkServer *FindNetworkServer();
    ISFActorFactory *FindActorFactory();
    SFScene *FindScene(SFUInt32 SceneId);
    void CleanupScenes();

    void HandleEnterSceneReq(SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen);
    void HandleMoveReq(SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen);

private:
    ISFNetworkServer *NetworkServer;
    ISFActorFactory *ActorFactory;

    SFMap<SFUInt32, SFScene *> SceneMap;
    SFMap<SFActorId, SFUInt32> ActorSceneMap;
    SFUInt32 DefaultSceneId;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_MODULE_SCENE_MANAGER_H__ */
