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
    SSF_OBJECT_CLASS(SSFService)

    SSFService(SSFServiceContext &InContext, SSFObjectErrors &InErrors);
    virtual ~SSFService();
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFService_H__
