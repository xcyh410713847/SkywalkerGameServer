/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Test\Module\SSFModule_PublicTest.h
**作者: shyfan
**日期: 2023/08/26 15:38:23
**功能: 测试模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_PUBLIC_TEST_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_PUBLIC_TEST_H__

#include "Include/SSFCore.h"

#include "Core/Module/SSFModule.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFModule_PublicTest : public SSFOModule
{
public:
    SSFModule_PublicTest(SSFObjectCreatorContext &Context, SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager)
        : SSFOModule(Context, InPluginManager)
    {
    }
    virtual ~SSFModule_PublicTest(){};

    /**
     * 获取模块名称
     * @return 模块名称
     */
    virtual const std::string GetName() override
    {
        return SKYWALKER_SF_CLASS_NAME(SSFModule_PublicTest);
    };
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_PUBLIC_TEST_H__
