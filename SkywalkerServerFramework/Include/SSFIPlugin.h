/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFIPlugin.h
**作者: shyfan
**日期: 2023/07/30 12:37:01
**功能:
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_H__
#define __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_H__

#include <string>

namespace Skywalker
{
    namespace ServerFramework
    {
        struct SSFIPlugin
        {
            virtual const std::string &GetPluginName() = 0;
        };
    }
}

#endif // __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_H__
