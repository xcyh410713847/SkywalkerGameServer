/*************************************************************************
**文件: SkywalkerFramework\Plugin\Profiler\Module\SSFModule_HealthCheck.h
**作者: shyfan
**日期: 2026/03/02
**功能: 健康检查模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_HEALTHCHECK_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_HEALTHCHECK_H__

#include "Core/Module/SSFModule.h"

SF_NAMESPACE_BEGIN

class SSFModule_HealthCheck : public SSFModule
{
#pragma region Object

public:
    virtual void Init(SFObjectErrors &Errors) override;
    virtual void Tick(SFObjectErrors &Errors, SFUInt64 DelayMS) override;
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SSFModule_HealthCheck(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SSFModule_HealthCheck() {};

    bool IsHealthy() const;

private:
    SFBool bHealthy = TRUE;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_HEALTHCHECK_H__
