/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SFService.h
**作者: shyfan
**日期: 2024/01/16 21:10:23
**功能: SSFService
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFService_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFService_H__

#include "Include/SFCore.h"

#include "Core/Object/SFObject.h"

SF_NAMESPACE_BEGIN

struct SSFServiceContext : public SSFObjectContext
{
};

class SSFService : public SSFObject
{
public:
    SSFService(SSFServiceContext &InContext, SFObjectErrors &InErrors);
    virtual ~SSFService();

#pragma region Process

public:
    /**
     * 初始化
     */
    virtual bool Init(SFObjectErrors &InErrors) = 0;

    /**
     * 启动
     */
    virtual bool Start(SFObjectErrors &InErrors) = 0;

    /**
     * Tick
     */
    virtual bool Tick(SFObjectErrors &InErrors) = 0;

    /**
     * 停止
     */
    virtual bool Stop(SFObjectErrors &InErrors) = 0;

    /**
     * 销毁
     */
    virtual void Destroy(SFObjectErrors &InErrors) = 0;

#pragma endregion Process
};

SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFService_H__
