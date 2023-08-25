/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Private\SSFModule_TCP.h
**作者: shyfan
**日期: 2023/08/25 15:31:50
**功能: TCP链接模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_TCP_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_TCP_H__

#include "Include/SSFCore.h"

#include "Core/Module/SSFModule.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFOModule_TCP : public SSFOModule
{
public:
    SSFOModule_TCP(SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager)
        : SSFOModule(InPluginManager)
    {
    }
    virtual ~SSFOModule_TCP(){};

    /**
     * 获取模块名称
     * @return 模块名称
     */
    virtual const std::string GetName() override
    {
        return SKYWALKER_SF_CLASS_NAME(SSFOModule_TCP);
    };
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_TCP_H__
