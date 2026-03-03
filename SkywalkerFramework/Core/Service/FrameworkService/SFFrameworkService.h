/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SFFrameworkService.h
**作者: shyfan
**日期: 2024/01/22 12:29:38
**功能: SSFFrameworkService
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFFrameworkService_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFFrameworkService_H__

#include "Core/Service/SFService.h"

SF_NAMESPACE_BEGIN

class SSFFrameworkService : public SSFService
{
public:
    SSFFrameworkService(SSFServiceContext &InContext, SFObjectErrors &InErrors);
    virtual ~SSFFrameworkService();

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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFFrameworkService_H__
