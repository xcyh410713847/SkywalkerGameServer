/*************************************************************************
**文件: SkywalkerFramework\Plugin\Game\SFPlugin_Game.h
**作者: shyfan
**日期: 2023/09/28 10:14:54
**功能: Game插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Game_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Game_H__

#include "Core/Plugin/SFPlugin.h"

SF_NAMESPACE_BEGIN

class SFPlugin_Game : public SFPlugin
{
public:
    SFPlugin_Game(SFPluginContext &InContext, SFObjectErrors &InErrors)
        : SFPlugin(InContext, InErrors)
    {
    }
    virtual ~SFPlugin_Game() {};

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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Game_H__
