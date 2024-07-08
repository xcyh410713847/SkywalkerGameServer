/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Test\Module\SSFModule_TestThree.h
**作者: shyfan
**日期: 2023/08/26 16:22:19
**功能: 测试模块 3
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_THREE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_THREE_H__

#include "Include/SSFCore.h"

#include "Core/Module/SSFModule.h"

SSF_NAMESPACE_BEGIN

class SSFModule_TestThree : public SSFModule
{
public:
    SSFModule_TestThree(SSFModuleContext &InContext, SSFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SSFModule_TestThree(){};
};

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_THREE_H__
