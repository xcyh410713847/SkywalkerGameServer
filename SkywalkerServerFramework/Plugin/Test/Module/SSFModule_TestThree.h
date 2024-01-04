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

class SSFModule_TestThree : public SSFOModule
{
    SSF_OBJECT_CLASS(SSFModule_TestThree)

public:
    SSFModule_TestThree(SSF_PTR_PLUGIN_MANAGER InPluginManager)
        : SSFOModule(InPluginManager)
    {
    }
    virtual ~SSFModule_TestThree(){};
};

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_THREE_H__
