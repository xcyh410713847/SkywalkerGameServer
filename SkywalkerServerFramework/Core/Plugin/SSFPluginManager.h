/*************************************************************************
**文件: SSFPluginManager.h
**作者: shyfan
**日期: 2023/07/29 16:01:14
**功能: 插件管理器
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__

#include <map>
#include <string>

#include "SSFIPluginManager.h"

namespace Skywalker
{
    namespace ServerFramework
    {
        class SSFCPluginManager : public SSFIPluginManager
        {
        public:
            SSFCPluginManager();
            virtual ~SSFCPluginManager();

#pragma region SSFIPluginManager
            /**
             * 注册插件
             * @param Plugin 插件
             */
            virtual void RegisterPlugin(SSFPluginError &Errors, SSFIPlugin *Plugin) override;

            /**
             * 注销插件
             * @param Plugin 插件
             */
            virtual void UnregisterPlugin(SSFPluginError &Errors, SSFIPlugin *Plugin) override;

            /**
             * 获取插件
             * @param PluginName 插件名称
             * @return 插件
             */
            virtual SSFIPlugin *GetPlugin(const std::string &PluginName) override;

#pragma endregion SSFIPluginManager

        private:
            typedef std::map<std::string, SSFIPlugin *> TMap_Plugin;
            TMap_Plugin PluginMap;
        };
    }
}

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_MANAGER_H__
