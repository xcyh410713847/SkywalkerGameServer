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

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFModule_TestTwo : public SSFOModule
{
public:
    SSFModule_TestTwo(SSFObjectCreatorContext &Context, SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager)
        : SSFOModule(Context, InPluginManager)
    {
    }
    virtual ~SSFModule_TestTwo(){};

    /**
     * 获取模块名称
     * @return 模块名称
     */
    virtual const std::string GetName() override
    {
        return SKYWALKER_SF_CLASS_NAME(SSFModule_TestTwo);
    };
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_TEST_TWO_H__
