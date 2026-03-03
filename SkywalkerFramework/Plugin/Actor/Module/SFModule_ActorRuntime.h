/*************************************************************************
**文件: SkywalkerFramework\Plugin\Actor\Module\SFModule_ActorRuntime.h
**作者: shyfan
**日期: 2026/03/02
**功能: Actor运行时模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_ACTORRUNTIME_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_ACTORRUNTIME_H__

#include "Core/Module/SFModule.h"

SF_NAMESPACE_BEGIN

class SFModule_ActorRuntime : public SSFModule
{
#pragma region Object

public:
    virtual void Init(SFObjectErrors &Errors) override;
    virtual void Start(SFObjectErrors &Errors) override;
    virtual void Tick(SFObjectErrors &Errors, SFUInt64 DelayMS) override;
    virtual void Stop(SFObjectErrors &Errors) override;
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SFModule_ActorRuntime(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SFModule_ActorRuntime() {};
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_ACTORRUNTIME_H__
