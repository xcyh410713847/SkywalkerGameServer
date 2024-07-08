/*************************************************************************
**文件: SkywalkerServerFramework\Core\Service\SSFService.h
**作者: shyfan
**日期: 2024/01/16 21:10:23
**功能: SSFService
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFService_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFService_H__

#include "Include/SSFCore.h"

#include "Core/Object/SSFObject.h"

SSF_NAMESPACE_BEGIN

struct SSFServiceContext : public SSFObjectContext
{
};

class SSFService : public SSFObject
{
public:
    SSFService(SSFServiceContext &InContext, SSFObjectErrors &InErrors);
    virtual ~SSFService();

#pragma region Process

public:
    /**
     * 初始化
     */
    virtual bool Init(SSFObjectErrors &InErrors) = 0;

    /**
     * 启动
     */
    virtual bool Start(SSFObjectErrors &InErrors) = 0;

    /**
     * Tick
     */
    virtual bool Tick(SSFObjectErrors &InErrors) = 0;

    /**
     * 停止
     */
    virtual bool Stop(SSFObjectErrors &InErrors) = 0;

    /**
     * 销毁
     */
    virtual void Destroy(SSFObjectErrors &InErrors) = 0;

#pragma endregion Process
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFService_H__
