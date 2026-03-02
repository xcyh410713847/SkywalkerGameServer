/*************************************************************************
**文件: SkywalkerFramework\Plugin\AI\Module\SSFModule_AIRuntime.h
**作者: shyfan
**日期: 2026/03/02
**功能: AI运行时模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_AIRUNTIME_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_AIRUNTIME_H__

#include "Core/Module/SSFModule.h"

#include <vector>
#include <unordered_set>

SF_NAMESPACE_BEGIN

class SSFModule_AIRuntime : public SSFModule
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
    SSFModule_AIRuntime(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SSFModule_AIRuntime() {};

    void SetTickBudgetMS(SFUInt64 InTickBudgetMS);
    SFUInt64 GetTickBudgetMS() const;
    SFUInt64 GetBudgetExceededCount() const;
    bool SetStrategy(const SFString &InStrategyName);
    const SFString &GetStrategy() const;
    SFString BuildStats() const;
    SFString BuildStrategies() const;

private:
    SFUInt64 GetEffectiveBudgetMS() const;

private:
    SFUInt64 TickBudgetMS = 2;
    SFUInt64 TickCounter = 0;
    SFUInt64 BudgetExceededCount = 0;
    SFString StrategyName = "strict";
    std::unordered_set<SFString> SupportedStrategies = {"strict", "balanced", "relaxed"};
    std::vector<SFString> StrategyOrder = {"strict", "balanced", "relaxed"};
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_AIRUNTIME_H__
