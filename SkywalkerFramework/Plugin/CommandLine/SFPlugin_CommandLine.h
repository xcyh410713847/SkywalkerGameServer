/*************************************************************************
**文件: SkywalkerFramework\Plugin\CommandLine\SFPlugin_CommandLine.h
**作者: shyfan
**日期: 2024/01/16 11:45:30
**功能: 命令行插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_CommandLine_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_CommandLine_H__

#include "Core/Plugin/SFPlugin.h"

SF_NAMESPACE_BEGIN

class SFPlugin_CommandLine : public SFPlugin
{
public:
    SFPlugin_CommandLine(SFPluginContext &InContext, SFObjectErrors &InErrors)
        : SFPlugin(InContext, InErrors)
    {
    }
    virtual ~SFPlugin_CommandLine() {};

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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_CommandLine_H__
