/*************************************************************************
**文件: SkywalkerServerFramework\Core\Service\FrameworkService\SSFService_Event.h
**作者: shyfan
**日期: 2024/01/22 16:03:19
**功能: SSFService_Event
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFService_Event_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFService_Event_H__

#include "Core/Service/FrameworkService/SSFFrameworkService.h"

SSF_NAMESPACE_BEGIN

class SSFService_Event : public SSFFrameworkService
{
public:
    SSFService_Event(SSFServiceContext &InContext, SSFObjectErrors &InErrors);
    virtual ~SSFService_Event();

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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFService_Event_H__
