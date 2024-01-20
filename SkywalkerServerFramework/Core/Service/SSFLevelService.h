/*************************************************************************
**文件: SkywalkerServerFramework\Core\Service\SSFLevelService.h
**作者: shyfan
**日期: 2024/01/20 15:23:37
**功能: SSFLevelService
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFLevelService_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFLevelService_H__

#include "Core/Service/SSFService.h"

SSF_NAMESPACE_BEGIN

class SSFLevelService : public SSFService
{
    SSF_OBJECT_CLASS(SSFLevelService)

public:
    SSFLevelService(SSFServiceContext &InContext, SSFObjectErrors &InErrors);
    virtual ~SSFLevelService();
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFLevelService_H__
