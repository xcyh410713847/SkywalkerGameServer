/*************************************************************************
**文件: SkywalkerFramework\Plugin\AI\Module\SFModule_AIRuntime.h
**作者: shyfan
**日期: 2026/03/19
**功能: AI运行时模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_AI_RUNTIME_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_AI_RUNTIME_H__

#include "Include/SFCore.h"
#include "Include/SFActorTypes.h"
#include "Include/SFNetworkInterface.h"

#include "Core/Module/SFModule.h"

SF_NAMESPACE_BEGIN

/**
 * AI 运行时模块
 *
 * 职责：
 * 1. NPC 巡逻：沿巡逻路径循环移动，到达当前路点后切换下一个
 * 2. Monster 追击：检测仇恨范围内最近的 Player，向其移动；无目标时返回出生点
 * 3. 位置变化时广播 MoveBroadcast 给场景内玩家
 */
class SFModule_AIRuntime : public SSFModule
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
    SFModule_AIRuntime(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors),
          NetworkServer(nullptr),
          ActorFactory(nullptr),
          SceneManager(nullptr)
    {
    }
    virtual ~SFModule_AIRuntime() {}

private:
    ISFNetworkServer *FindNetworkServer();
    ISFActorFactory *FindActorFactory();
    ISFSceneManager *FindSceneManager();

    void TickNPCPatrol(SFActorId ActorId, float DeltaSec);
    void TickMonsterChase(SFActorId ActorId, float DeltaSec);
    void BroadcastActorMove(SFActorId ActorId, const SFVector3 &Pos);

private:
    ISFNetworkServer *NetworkServer;
    ISFActorFactory *ActorFactory;
    ISFSceneManager *SceneManager;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_MODULE_AI_RUNTIME_H__ */
