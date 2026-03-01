/*************************************************************************
**文件: SkywalkerFramework\Plugin\DB\SSFPlugin_DB.h
**作者: shyfan
**日期: 2023/09/28 10:47:30
**功能: DB插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_DB_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_DB_H__

#include "Core/Plugin/SSFPlugin.h"

SF_NAMESPACE_BEGIN

class SSFPlugin_DB : public SFPlugin
{
public:
    SSFPlugin_DB(SFPluginContext &InContext, SFObjectErrors &InErrors)
        : SFPlugin(InContext, InErrors)
    {
    }
    virtual ~SSFPlugin_DB() {};

#pragma region SSFPlugin

private:
    /**
     * 安装
     */
    virtual void Install(SFObjectErrors &Errors) override;

    /**
     * 卸载
     */
    virtual void Uninstall(SFObjectErrors &Errors) override;

#pragma endregion SSFPlugin
};

SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_DB_H__
