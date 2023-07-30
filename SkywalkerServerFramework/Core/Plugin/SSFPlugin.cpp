/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPlugin.cpp
**作者: shyfan
**日期: 2023/07/30 12:33:58
**功能: 插件
*************************************************************************/

#include "SSFPlugin.h"

using namespace Skywalker::ServerFramework;

SSFCPlugin::SSFCPlugin(SSFSharedPtrPluginManager PluginManager)
    : PluginManager(PluginManager)
{
}

SSFCPlugin::~SSFCPlugin()
{
}
