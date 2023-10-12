/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\DB\SSFPlugin_DB.h
**作者: shyfan
**日期: 2023/09/28 10:47:30
**功能: DB插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_DB_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_DB_H__

#include "Core/Plugin/SSFPluginManager.h"
#include "Core/Plugin/SSFPlugin.h"

SSF_NAMESPACE_BEGIN

class SSFPlugin_DB : public SSFOPlugin
{
    SSF_OBJECT_CLASS(SSFPlugin_DB)

public:
    SSFPlugin_DB(SSF_PTR_PLUGIN_MANAGER InPluginManager)
        : SSFOPlugin(InPluginManager)
    {
    }
    virtual ~SSFPlugin_DB(){};

    /**
     * 获取插件名称
     * @return 插件名称
     */
    virtual const std::string GetName() override
    {
        return SSF_CLASS_NAME(SSFPlugin_DB);
    }

#pragma region SSFOPlugin

private:
    /**
     * 安装
     */
    virtual void Install(SSFModuleErrors &Errors) override;

    /**
     * 卸载
     */
    virtual void Uninstall(SSFModuleErrors &Errors) override;

#pragma endregion SSFOPlugin
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_DB_H__
