/*************************************************************************
**文件: SkywalkerFramework\Plugin\AI\Module\SSFModule_AIRuntime.h
**作者: shyfan
**日期: 2026/03/02
**功能: AI运行时模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_AIRUNTIME_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_AIRUNTIME_H__

#include "Core/Module/SSFModule.h"

SF_NAMESPACE_BEGIN

class SSFModule_AIRuntime : public SSFModule
{
#pragma region Object

public:
    virtual void Init(SFObjectErrors &Errors) override;
    virtual void Tick(SFObjectErrors &Errors, SFUInt64 DelayMS) override;
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SSFModule_AIRuntime(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SSFModule_AIRuntime() {};

    void SetTickBudgetMS(SFUInt64 InTickBudgetMS);
    SFUInt64 GetTickBudgetMS() const;

private:
    SFUInt64 TickBudgetMS = 2;
    SFUInt64 TickCounter = 0;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_AIRUNTIME_H__
