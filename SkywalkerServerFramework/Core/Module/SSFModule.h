/*************************************************************************
**文件: SkywalkerServerFramework\Core\Module\SSFModule.h
**作者: shyfan
**日期: 2023/08/07 23:52:04
**功能: 模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__

#include "Include/SSFCore.h"

#include "Core/Object/SSFObject.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFOModule : public SSFObject
{
#pragma region Object
#pragma endregion Object

public:
    SSFOModule(SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager);
    virtual ~SSFOModule();

    /**
     * 获取模块名称
     * @return 模块名称
     */
    virtual const std::string GetName()
    {
        return SKYWALKER_SF_CLASS_NAME(SSFOModule);
    };

protected:
    SKYWALKER_SF_PTR_PLUGIN_MANAGER PluginManager;
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__
