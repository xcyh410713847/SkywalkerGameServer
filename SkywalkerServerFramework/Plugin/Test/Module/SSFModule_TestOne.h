/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Test\Module\SSFModule_TestOne.h
**作者: shyfan
**日期: 2023/08/26 16:22:19
**功能: 测试模块 1
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_ONE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_ONE_H__

#include "Include/SSFCore.h"

#include "Core/Module/SSFModule.h"

SSF_NAMESPACE_BEGIN

class SSFModule_TestOne : public SSFOModule
{
    SSF_OBJECT_CLASS(SSFModule_TestOne)

public:
    SSFModule_TestOne(SSF_PTR_PLUGIN_MANAGER InPluginManager)
        : SSFOModule(InPluginManager)
    {
    }
    virtual ~SSFModule_TestOne(){};
};

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_ONE_H__
