/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFErrors.h
**作者: shyfan
**日期: 2023/07/30 13:25:43
**功能: 错误码
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_ERRORS_H__
#define __SKYWALKER_SERVER_FRAMEWORK_ERRORS_H__

#include "SkywalkerErrors\SkywalkerErrors.h"

#include "SSFCore.h"

SKYWALKER_SF_NAMESPACE_BEGIN

// 对象错误
typedef SKYWALKER_ERRORS_NAMESPACE::CSkywalkerErrors<std::string> SSFObjectErrors;

// 插件错误
typedef SKYWALKER_ERRORS_NAMESPACE::CSkywalkerErrors<int> SSFPluginErrors;

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_ERRORS_H__
