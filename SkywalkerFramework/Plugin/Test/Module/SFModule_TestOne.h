/*************************************************************************
**文件: SkywalkerFramework\Plugin\Test\Module\SFModule_TestOne.h
**作者: shyfan
**日期: 2023/08/26 16:22:19
**功能: 测试模块 1
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_ONE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_ONE_H__

#include "Include/SFCore.h"

#include "Core/Module/SFModule.h"

SF_NAMESPACE_BEGIN

class SFModule_TestOne : public SSFModule
{
public:
    SFModule_TestOne(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SFModule_TestOne() {};
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_ONE_H__
