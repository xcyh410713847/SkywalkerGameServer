/*************************************************************************
**文件: SkywalkerServerFramework\Core\Service\SSFFrameworkService.h
**作者: shyfan
**日期: 2024/01/20 15:18:41
**功能: SSFFrameworkService
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFFrameworkService_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFFrameworkService_H__

#include "Core/Service/SSFService.h"

SSF_NAMESPACE_BEGIN

class SSFFrameworkService : public SSFService
{
    SSF_OBJECT_CLASS(SSFFrameworkService)

public:
    SSFFrameworkService(SSFServiceContext &InContext, SSFObjectErrors &InErrors);
    virtual ~SSFFrameworkService();
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFFrameworkService_H__
