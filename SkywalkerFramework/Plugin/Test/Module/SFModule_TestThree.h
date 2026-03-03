/*************************************************************************
**文件: SkywalkerFramework\Plugin\Test\Module\SFModule_TestThree.h
**作者: shyfan
**日期: 2023/08/26 16:22:19
**功能: 测试模块 3
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_THREE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_THREE_H__

#include "Include/SFCore.h"

#include "Core/Module/SFModule.h"

SF_NAMESPACE_BEGIN

class SFModule_TestThree : public SSFModule
{
public:
    SFModule_TestThree(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SFModule_TestThree() {};
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_THREE_H__
