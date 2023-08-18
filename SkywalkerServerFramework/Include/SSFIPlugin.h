/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFIPlugin.h
**作者: shyfan
**日期: 2023/07/30 12:37:01
**功能:
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_H__
#define __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_H__

#include "SSFCore.h"
#include "SSFIModule.h"

SKYWALKER_SF_NAMESPACE_BEGIN

struct SSFIPlugin
{
    /**
     * 获取插件名称
     * @return 插件名称
     */
    virtual const std::string GetName() = 0;
};

/**
 *SSFIPlugin 指针，共享指针继承有问题，所以用普通
 */
#define SSF_PTR_PLUGIN SSFIPlugin *

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_I_PLUGIN_H__
