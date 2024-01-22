/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\CommandLine\SSFPlugin_CommandLine.h
**作者: shyfan
**日期: 2024/01/16 11:45:30
**功能: 命令行插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_CommandLine_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_CommandLine_H__

#include "Core/Plugin/SSFPlugin.h"

SSF_NAMESPACE_BEGIN

class SSFPlugin_CommandLine : public SSFPlugin
{
    SSF_OBJECT_CLASS(SSFPlugin_CommandLine)

public:
    SSFPlugin_CommandLine(SSFPluginContext &InContext, SSFObjectErrors &InErrors)
        : SSFPlugin(InContext, InErrors)
    {
    }
    virtual ~SSFPlugin_CommandLine(){};

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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_CommandLine_H__
