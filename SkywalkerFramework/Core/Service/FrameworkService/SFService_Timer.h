/*************************************************************************
**文件: SkywalkerFramework\Core\Service\FrameworkService\SFService_Timer.h
**作者: shyfan
**日期: 2024/01/24 19:46:15
**功能: 时间 Service
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFService_Timer_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFService_Timer_H__

#include "SkywalkerTimer/SkywalkerTimer.h"

#include "Core/Service/FrameworkService/SFFrameworkService.h"

SF_NAMESPACE_BEGIN

typedef SKYWALKER_TIMER_NAMESPACE::SkywalkerTimer SkywalkerSFTimer;
/**
 * 时间服务
 * 说明：在服务生命周期中维护帧时间与总运行时间统计。
 */
class SSFService_Timer : public SSFFrameworkService, public SkywalkerSFTimer
{
public:
    /** 构造函数 */
    SSFService_Timer(SSFServiceContext &InContext, SFObjectErrors &InErrors);
    /** 析构函数 */
    virtual ~SSFService_Timer();

#pragma region Process

public:
    /**
     * 初始化
     * @return true 成功；false 失败
     */
    virtual bool Init(SFObjectErrors &InErrors) override;

    /**
     * 启动
     * @return true 成功；false 失败
     */
    virtual bool Start(SFObjectErrors &InErrors) override;

    /**
     * 每帧更新时间
     * @return true 继续运行；false 结束
     */
    virtual bool Tick(SFObjectErrors &InErrors) override;

    /**
     * 停止
     * @return true 成功；false 失败
     */
    virtual bool Stop(SFObjectErrors &InErrors) override;

    /**
     * 销毁
     * 说明：释放时间服务相关资源。
     */
    virtual void Destroy(SFObjectErrors &InErrors) override;

#pragma endregion Process
};

SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFService_Timer_H__
