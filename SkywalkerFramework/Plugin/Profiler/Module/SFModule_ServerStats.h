/*************************************************************************
**文件: SkywalkerFramework\Plugin\Profiler\Module\SFModule_ServerStats.h
**作者: shyfan
**日期: 2026/03/19
**功能: 服务器统计监控模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_SERVER_STATS_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_SERVER_STATS_H__

#include "Include/SFCore.h"
#include "Include/SFNetworkInterface.h"
#include "Include/SFActorTypes.h"

#include "Core/Module/SFModule.h"

SF_NAMESPACE_BEGIN

/**
 * 服务器统计监控模块
 *
 * 职责：
 * 1. 定时输出在线人数、场景Actor数、消息吞吐统计
 * 2. 通过 ISFNetworkServer / ISFSceneManager 接口查询数据
 */
class SFModule_ServerStats : public SSFModule
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
    SFModule_ServerStats(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors),
          NetworkServer(nullptr),
          SceneManager(nullptr),
          ReportIntervalMS(10000),
          ElapsedMS(0)
    {
    }
    virtual ~SFModule_ServerStats() {}

private:
    ISFNetworkServer *FindNetworkServer();
    ISFSceneManager *FindSceneManager();

    void PrintStats();

private:
    ISFNetworkServer *NetworkServer;
    ISFSceneManager *SceneManager;

    SFUInt64 ReportIntervalMS;
    SFUInt64 ElapsedMS;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_MODULE_SERVER_STATS_H__ */
