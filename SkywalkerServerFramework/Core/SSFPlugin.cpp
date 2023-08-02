/*************************************************************************
**文件: SkywalkerServerFramework\Core\SSFPlugin.cpp
**作者: shyfan
**日期: 2023/07/30 12:57:08
**功能: 插件
*************************************************************************/

#include "SSFPlugin.h"

SKYWALKER_SF_NAMESPACE_USE

SSFCPlugin::SSFCPlugin(SSFSharedPtrPluginManager PluginManager)
    : PluginManager(PluginManager)
{
}

SSFCPlugin::~SSFCPlugin()
{
}
