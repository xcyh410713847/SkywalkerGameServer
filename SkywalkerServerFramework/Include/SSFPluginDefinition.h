/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFPluginDefinition.h
**作者: shyfan
**日期: 2023/07/30 12:37:24
**功能: 插件定义
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_DEFINITION_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_DEFINITION_H__

#include "SSFCore.h"

SKYWALKER_SF_NAMESPACE_BEGIN

enum EPluginError
{
    PluginError_Success = 0,                   // 成功
    PluginError_Unknow,                        // 未知错误
    PluginError_Register_Plugin_nullptr,       // 注册插件为空
    PluginError_LoadPlugin_Repeat,             // 插件重复加载
    PluginError_LoadPlugin_DynamicLib_nullptr, // 动态库为空
    PluginError_LoadPlugin_Failed,             // 插件加载失败
    PluginError_LoadPlugin_Entry_nullptr,      // 插件入口为空
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_DEFINITION_H__
