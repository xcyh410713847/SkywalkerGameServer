/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFPluginDefinition.h
**作者: shyfan
**日期: 2023/07/30 12:37:24
**功能: 插件定义
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_DEFINITION_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_DEFINITION_H__

namespace Skywalker
{
    namespace ServerFramework
    {
        enum EPluginError
        {
            PluginError_Success = 0,             // 成功
            PluginError_Unknow,                  // 未知错误
            PluginError_Register_Plugin_nullptr, // 注册插件为空
        };
    }
}

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_DEFINITION_H__
