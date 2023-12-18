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

SSF_NAMESPACE_BEGIN

class SSFOModuleManager : public SSFObject
{
public:
    SSFOModuleManager() : SSFObject() {}
    virtual ~SSFOModuleManager() {}

    /**
     * 注册模块
     * @param Module 模块
     */
    virtual void RegisterModule(SSFModuleErrors &Errors, SSF_PTR_MODULE Module);

    /**
     * 注销模块
     * @param Module 模块
     */
    virtual void UnregisterModule(SSFModuleErrors &Errors, SSF_PTR_MODULE Module);

    /**
     * 获取模块
     */
    SSF_PTR_MODULE GetModule(const std::string &ModuleName);

    /**
     * 获取模块
     * @return 模块
     */
    template <typename T>
    SSF_PTR(T)
    GetModule()
    {
        SSF_PTR_MODULE Module = GetModule(SSF_CLASS_NAME(T));
        if (Module == nullptr)
        {
            return nullptr;
        }

        return SSF_PTR_DYNAMIC_CAST(T)(Module);
    }

protected:
    typedef std::map<std::string, SSF_PTR_MODULE> TMap_Module;
    TMap_Module ModuleMap;
};

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_MANAGER_H__
