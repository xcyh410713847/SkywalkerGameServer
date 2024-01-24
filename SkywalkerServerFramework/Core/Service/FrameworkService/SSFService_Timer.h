/*************************************************************************
**文件: SkywalkerServerFramework\Core\Service\FrameworkService\SSFService_Timer.h
**作者: shyfan
**日期: 2024/01/24 19:46:15
**功能: 时间 Service
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFService_Timer_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFService_Timer_H__

#include "SkywalkerTimer/SkywalkerTimer.h"

#include "Core/Service/SSFFrameworkService.h"

SSF_NAMESPACE_BEGIN

typedef SKYWALKER_TIMER_NAMESPACE::SkywalkerTimer SkywalkerSFTimer;
class SSFService_Timer : public SSFFrameworkService, public SkywalkerSFTimer
{
    SSF_OBJECT_CLASS(SSFService_Timer)

public:
    SSFService_Timer(SSFServiceContext &InContext, SSFObjectErrors &InErrors);
    virtual ~SSFService_Timer();

#pragma region Process

public:
    /**
     * 初始化
     */
    virtual bool Init(SSFObjectErrors &InErrors) override;

    /**
     * 启动
     */
    virtual bool Start(SSFObjectErrors &InErrors) override;

    /**
     * Tick
     */
    virtual bool Tick(SSFObjectErrors &InErrors) override;

    /**
     * 停止
     */
    virtual bool Stop(SSFObjectErrors &InErrors) override;

    /**
     * 销毁
     */
    virtual void Destroy(SSFObjectErrors &InErrors) override;

#pragma endregion Process
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFService_Timer_H__
