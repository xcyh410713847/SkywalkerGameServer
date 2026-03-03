/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SFLevelService.h
**作者: shyfan
**日期: 2024/01/22 12:28:06
**功能: SSFLevelService
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFLevelService_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFLevelService_H__

#include "Core/Service/SFService.h"

SF_NAMESPACE_BEGIN

class SSFLevelService : public SSFService
{
public:
    SSFLevelService(SSFServiceContext &InContext, SFObjectErrors &InErrors);
    virtual ~SSFLevelService();

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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFLevelService_H__
