/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPlugin.h
**作者: shyfan
**日期: 2023/07/30 12:34:07
**功能:
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__

#include <memory>

#include "Include/SSFIPluginManager.h"

namespace Skywalker
{
    namespace ServerFramework
    {
        typedef std::shared_ptr<SSFIPluginManager> SSFSharedPtrPluginManager;

        class SSFCPlugin : public SSFIPlugin
        {
        public:
            SSFCPlugin(SSFSharedPtrPluginManager PluginManager);
            virtual ~SSFCPlugin();

        protected:
            SSFSharedPtrPluginManager PluginManager;
        };
    }
}

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
