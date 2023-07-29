/*************************************************************************
**文件: SSFIPluginManager.h
**作者: shyfan
**日期: 2023/07/29 15:20:03
**功能: 插件管理器接口
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_MANAGER_H__

#include <string>

#include "SSFIPlugin.h"

namespace Skywalker
{
    namespace ServerFramework
    {
        struct SSFIPluginManager
        {
            /**
             * 注册插件
             * @param Plugin 插件
             */
            virtual void RegisterPlugin(SSFIPlugin *Plugin) = 0;

            /**
             * 注销插件
             * @param Plugin 插件
             */
            virtual void UnregisterPlugin(SSFIPlugin *Plugin) = 0;

            /**
             * 获取插件
             * @param PluginName 插件名称
             * @return 插件
             */
            virtual SSFIPlugin *GetPlugin(const std::string &PluginName) = 0;
        };
    }
}

#endif // __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_MANAGER_H__
