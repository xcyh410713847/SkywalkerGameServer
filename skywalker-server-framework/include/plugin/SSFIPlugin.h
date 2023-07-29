/*************************************************************************
**文件: SSFIPlugin.h
**作者: shyfan
**日期: 2023/07/28 10:58:10
**功能: 插件接口
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_H__
#define __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_H__

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
