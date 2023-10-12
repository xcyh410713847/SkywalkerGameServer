/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFPlatform.h
**作者: shyfan
**日期: 2023/08/04 16:39:49
**功能: 平台
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLATFORM_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLATFORM_H__

#include "SkywalkerPlatform/SkywalkerPlatform.h"

/**
 * 导出
 */
#if defined(SKYWALKER_PLATFORM_WINDOWS)
#define SSF_DLL_EXPORT extern "C" __declspec(dllexport)
#define SSF_API __declspec(dllexport)
#else
#define SSF_DLL_EXPORT extern "C" __attribute__((visibility("default")))
#define SSF_API __attribute__((visibility("default")))
#endif

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLATFORM_H__
