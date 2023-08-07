/*************************************************************************
**文件: SkywalkerServerFramework\Core\SSFModule.h
**作者: shyfan
**日期: 2023/07/30 12:56:46
**功能: 模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__

#include "Include\SSFIModule.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFCModule : public SSFIModule
{
public:
    SSFCModule();
    virtual ~SSFCModule();
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__
