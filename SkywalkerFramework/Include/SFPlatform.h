/*************************************************************************
**文件: SkywalkerFramework\Include\SFPlatform.h
**作者: shyfan
**日期: 2023/08/04 16:39:49
**功能: 平台
*************************************************************************/

#ifndef __SKYWALKER_FRAMEWORK_SFPLATFORM_H__
#define __SKYWALKER_FRAMEWORK_SFPLATFORM_H__

#include "SkywalkerPlatform/SkywalkerPlatform.h"

/**
 * 导出
 */
#if defined(SKYWALKER_PLATFORM_WINDOWS)
#define SF_DLL_EXPORT extern "C" __declspec(dllexport)
#define SF_API __declspec(dllexport)
#else
#define SF_DLL_EXPORT extern "C" __attribute__((visibility("default")))
#define SF_API __attribute__((visibility("default")))
#endif

#endif // __SKYWALKER_FRAMEWORK_SFPLATFORM_H__
