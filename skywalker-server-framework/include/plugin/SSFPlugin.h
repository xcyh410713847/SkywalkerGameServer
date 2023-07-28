/*************************************************************************
**文件: SSFPlugin.h
**作者: shyfan
**日期: 2023/07/28 10:58:44
**功能: 插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__

#include "SSFIPlugin.h"

namespace Skywalker
{
    namespace ServerFramework
    {
        class SSFCPlugin : public SSFIPlugin
        {
        public:
            SSFCPlugin();
            virtual ~SSFCPlugin();
        };
    }
}

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
