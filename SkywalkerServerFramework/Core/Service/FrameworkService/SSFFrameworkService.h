/*************************************************************************
**文件: SkywalkerServerFramework\Core\Service\SSFFrameworkService.h
**作者: shyfan
**日期: 2024/01/22 12:29:38
**功能: SSFFrameworkService
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFFrameworkService_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFFrameworkService_H__

#include "Core/Service/SSFService.h"

SSF_NAMESPACE_BEGIN

class SSFFrameworkService : public SSFService
{
public:
    SSFFrameworkService(SSFServiceContext &InContext, SSFObjectErrors &InErrors);
    virtual ~SSFFrameworkService();

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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFFrameworkService_H__
