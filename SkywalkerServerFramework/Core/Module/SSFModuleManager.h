/*************************************************************************
**文件: SkywalkerServerFramework\Core\Module\SSFModuleManager.h
**作者: shyfan
**日期: 2023/08/25 12:12:02
**功能: 模块管理器
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_MANAGER_H__

#include "Include/SSFCore.h"

#include "Core/Object/SSFObject.h"
#include "Core/Module/SSFModule.h"
#include "Core/Map/SSFMap.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFOModuleManager : public SSFObject
{
public:
    SSFOModuleManager() : SSFObject() {}
    virtual ~SSFOModuleManager() {}

    /**
     * 注册模块
     * @param Module 模块
     */
    virtual void RegisterModule(SSFModuleErrors &Errors, SKYWALKER_SF_PTR_MODULE Module);

    /**
     * 注销模块
     * @param Module 模块
     */
    virtual void UnregisterModule(SSFModuleErrors &Errors, SKYWALKER_SF_PTR_MODULE Module);

    /**
     * 获取模块
     */
    SKYWALKER_SF_PTR_MODULE GetModule(const std::string &ModuleName);

    /**
     * 获取模块
     * @return 模块
     */
    template <typename T>
    SKYWALKER_SF_PTR(T)
    GetModule()
    {
        SKYWALKER_SF_PTR_MODULE Module = GetModule(SKYWALKER_SF_CLASS_NAME(T));
        auto Iter = ModuleMap.find(SKYWALKER_SF_CLASS_NAME(T));
        if (Module == nullptr)
        {
            return nullptr;
        }

        return SKYWALKER_SF_POINT_CAST(T)(Module);
    }

protected:
    typedef std::map<std::string, SKYWALKER_SF_PTR_MODULE> TMap_Module;
    TMap_Module ModuleMap;
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_MANAGER_H__
