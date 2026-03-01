/*************************************************************************
**文件: SkywalkerFramework\Core\Service\FrameworkService\SSFService_Timer.h
**作者: shyfan
**日期: 2024/01/24 19:46:15
**功能: 时间 Service
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFService_Timer_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFService_Timer_H__

#include "SkywalkerTimer/SkywalkerTimer.h"

#include "Core/Service/FrameworkService/SSFFrameworkService.h"

SF_NAMESPACE_BEGIN

typedef SKYWALKER_TIMER_NAMESPACE::SkywalkerTimer SkywalkerSFTimer;
class SSFService_Timer : public SSFFrameworkService, public SkywalkerSFTimer
{
public:
    SSFService_Timer(SSFServiceContext &InContext, SFObjectErrors &InErrors);
    virtual ~SSFService_Timer();

#pragma region Process

public:
    /**
     * 初始化
     */
    virtual bool Init(SFObjectErrors &InErrors) override;

    /**
     * 启动
     */
    virtual bool Start(SFObjectErrors &InErrors) override;

    /**
     * Tick
     */
    virtual bool Tick(SFObjectErrors &InErrors) override;

    /**
     * 停止
     */
    virtual bool Stop(SFObjectErrors &InErrors) override;

    /**
     * 销毁
     */
    virtual void Destroy(SFObjectErrors &InErrors) override;

#pragma endregion Process
};

SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFService_Timer_H__
