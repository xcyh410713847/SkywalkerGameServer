/*************************************************************************
**文件: Plugin.h
**作者: shyfan
**日期: 2023/07/28 10:41:45
**功能: 插件
*************************************************************************/

#ifndef __SKYWALKER_FRAMEWORK_PLUGIN_H__
#define __SKYWALKER_FRAMEWORK_PLUGIN_H__

#include "IPlugin.h"

namespace Skywalker
{
    namespace Framework
    {
        class CPlugin : public IPlugin
        {
        public:
            CPlugin();
            virtual ~CPlugin();
        };
    }
}

#endif // __SKYWALKER_FRAMEWORK_PLUGIN_H__
