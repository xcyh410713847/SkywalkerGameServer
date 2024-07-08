/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\DB\SSFPlugin_DB.h
**作者: shyfan
**日期: 2023/09/28 10:47:30
**功能: DB插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_DB_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_DB_H__

#include "Core/Plugin/SSFPlugin.h"

SSF_NAMESPACE_BEGIN

class SSFPlugin_DB : public SSFPlugin
{
public:
    SSFPlugin_DB(SSFPluginContext &InContext, SSFObjectErrors &InErrors)
        : SSFPlugin(InContext, InErrors)
    {
    }
    virtual ~SSFPlugin_DB(){};

#pragma region SSFPlugin

private:
    /**
     * 安装
     */
    virtual void Install(SSFObjectErrors &Errors) override;

    /**
     * 卸载
     */
    virtual void Uninstall(SSFObjectErrors &Errors) override;

#pragma endregion SSFPlugin
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_DB_H__
