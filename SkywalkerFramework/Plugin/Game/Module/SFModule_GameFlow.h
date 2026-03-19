/*************************************************************************
**文件: SkywalkerFramework\Plugin\Game\Module\SFModule_GameFlow.h
**作者: shyfan
**日期: 2026/03/19
**功能: 游戏流程模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_GAME_FLOW_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_GAME_FLOW_H__

#include "Include/SFCore.h"
#include "Include/SFNetworkInterface.h"
#include "Include/SFActorTypes.h"

#include "Core/Module/SFModule.h"

SF_NAMESPACE_BEGIN

class SFModule_GameFlow : public SSFModule
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
    SFModule_GameFlow(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors),
          NetworkServer(nullptr),
          ActorFactory(nullptr),
          SceneManager(nullptr)
    {
    }
    virtual ~SFModule_GameFlow() {}

private:
    ISFNetworkServer *FindNetworkServer();
    ISFActorFactory *FindActorFactory();
    ISFSceneManager *FindSceneManager();

    void SpawnInitialWorldActors();

    void HandlePlayerEnter(SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen);
    void HandlePlayerLeave(SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen);

private:
    ISFNetworkServer *NetworkServer;
    ISFActorFactory *ActorFactory;
    ISFSceneManager *SceneManager;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_MODULE_GAME_FLOW_H__ */
