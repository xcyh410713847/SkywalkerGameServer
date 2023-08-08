/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPlugin.h
**作者: shyfan
**日期: 2023/08/07 23:53:05
**功能: 插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__

#include <memory>

#include "Include/SSFIPluginManager.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFCPlugin : public SSFIPlugin
{
public:
    SSFCPlugin();
    virtual ~SSFCPlugin();

protected:
    SSFSharedPtr_IPluginManager PluginManager;
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
