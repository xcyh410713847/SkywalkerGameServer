/*************************************************************************
**文件: SkywalkerFramework\Core\Service\FrameworkService\SFService_Event.h
**作者: shyfan
**日期: 2024/01/22 16:03:19
**功能: SSFService_Event
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFService_Event_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFService_Event_H__

#include "Core/Service/FrameworkService/SFFrameworkService.h"

SF_NAMESPACE_BEGIN

class SSFService_Event : public SSFFrameworkService
{
public:
    SSFService_Event(SSFServiceContext &InContext, SFObjectErrors &InErrors);
    virtual ~SSFService_Event();

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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFService_Event_H__
