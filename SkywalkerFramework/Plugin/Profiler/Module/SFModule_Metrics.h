/*************************************************************************
**文件: SkywalkerFramework\Plugin\Profiler\Module\SFModule_Metrics.h
**作者: shyfan
**日期: 2026/03/02
**功能: 指标采集模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_METRICS_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_METRICS_H__

#include "Core/Module/SFModule.h"

SF_NAMESPACE_BEGIN

class SFModule_Metrics : public SSFModule
{
#pragma region Object

public:
    virtual void Init(SFObjectErrors &Errors) override;
    virtual void Tick(SFObjectErrors &Errors, SFUInt64 DelayMS) override;
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SFModule_Metrics(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SFModule_Metrics() {};

private:
    SFUInt64 TickCount = 0;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_METRICS_H__
