/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Test\Module\SSFModule_TestTwo.h
**作者: shyfan
**日期: 2023/08/26 16:22:19
**功能: 测试模块 2
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_TWO_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_TWO_H__

#include "Include/SSFCore.h"

#include "Core/Module/SSFModule.h"

SSF_NAMESPACE_BEGIN

class SSFModule_TestTwo : public SSFModule
{
public:
    SSFModule_TestTwo(SSFModuleContext &InContext, SSFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SSFModule_TestTwo(){};
};

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_TWO_H__
