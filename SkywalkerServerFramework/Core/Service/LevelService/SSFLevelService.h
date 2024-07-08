/*************************************************************************
**文件: SkywalkerServerFramework\Core\Service\SSFLevelService.h
**作者: shyfan
**日期: 2024/01/22 12:28:06
**功能: SSFLevelService
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFLevelService_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFLevelService_H__

#include "Core/Service/SSFService.h"

SSF_NAMESPACE_BEGIN

class SSFLevelService : public SSFService
{
public:
    SSFLevelService(SSFServiceContext &InContext, SSFObjectErrors &InErrors);
    virtual ~SSFLevelService();

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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFLevelService_H__
