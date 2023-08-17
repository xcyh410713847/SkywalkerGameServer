/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFIModule.h
**作者: shyfan
**日期: 2023/07/30 12:36:44
**功能: 模块接口
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_I_MODULE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_I_MODULE_H__

#include "SSFCore.h"

SKYWALKER_SF_NAMESPACE_BEGIN

struct SSFIModule
{
    /**
     * 获取模块名称
     * @return 模块名称
     */
    virtual const std::string GetName() = 0;
};
/**
 * SSFIModule 指针，共享指针继承有问题，所以用普通
 */
#define SSF_PTR_MODULE SKYWALKER_SF_PTR(SSFIModule)

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_I_MODULE_H__
