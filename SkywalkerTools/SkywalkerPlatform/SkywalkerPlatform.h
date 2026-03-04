/*************************************************************************
**File: SkywalkerTools\SkywalkerPlatform\SkywalkerPlatform.h
**Author: shyfan
**Date: 2023/08/04 16:57:26
**Desc: Platform
*************************************************************************/

#ifndef __SKYWALKER_PLATFORM_H__
#define __SKYWALKER_PLATFORM_H__

/** 平台识别与平台基础能力 */
#include "SkywalkerPlatformLinux.h"
#include "SkywalkerPlatformMac.h"
#include "SkywalkerPlatformWin.h"

#if defined(SKYWALKER_PLATFORM_WINDOWS)
/** 动态库相关（Windows） */
#define SKYWALKER_DYNAMIC_LIB_HANDLE HINSTANCE
#define SKYWALKER_DYNAMIC_LIB_LOAD(LibName) LoadLibraryExA(LibName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)
#define SKYWALKER_DYNAMIC_LIB_UNLOAD(Instance) FreeLibrary(Instance)
#define SKYWALKER_DYNAMIC_LIB_GET_SYMBOL(Instance, ProcName) GetProcAddress(Instance, ProcName)
#define SKYWALKER_DYNAMIC_LIB_UNLOAD_SUCCESS(Result) ((Result) != 0)
#define SKYWALKER_DYNAMIC_LIB_EXT ".dll"
#else
/** 动态库相关（Linux/Mac） */
#define SKYWALKER_DYNAMIC_LIB_HANDLE void *
#define SKYWALKER_DYNAMIC_LIB_LOAD(LibName) dlopen(LibName, RTLD_LAZY | RTLD_GLOBAL)
#define SKYWALKER_DYNAMIC_LIB_UNLOAD(Instance) dlclose(Instance)
#define SKYWALKER_DYNAMIC_LIB_GET_SYMBOL(Instance, ProcName) dlsym(Instance, ProcName)
#define SKYWALKER_DYNAMIC_LIB_UNLOAD_SUCCESS(Result) ((Result) == 0)
#define SKYWALKER_DYNAMIC_LIB_EXT ".so"
#endif

/** 平台名称宏 */
#if defined(_WIN32) || defined(_WIN64)
#define SKYWALKER_PLATFORM_NAME "Windows"
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if defined(TARGET_OS_IPHONE)
#define SKYWALKER_PLATFORM_NAME "iOS"
#else
#define SKYWALKER_PLATFORM_NAME "Mac"
#endif
#elif defined(__linux__)
#define SKYWALKER_PLATFORM_NAME "Linux"
#else
#define SKYWALKER_PLATFORM_NAME "Unknown"
#endif

/** 线程局部存储宏 */
#if defined(SKYWALKER_PLATFORM_WINDOWS)
#define SKYWALKER_THREAD_LOCAL __declspec(thread)
#else
#define SKYWALKER_THREAD_LOCAL __thread
#endif

/** 路径分隔符宏 */
#if defined(SKYWALKER_PLATFORM_WINDOWS)
#define SKYWALKER_PATH_SEPARATOR '\\'
#else
#define SKYWALKER_PATH_SEPARATOR '/'
#endif

/** 禁止拷贝宏 */
#define SKYWALKER_DISALLOW_COPY(ClassName) \
    ClassName(const ClassName &) = delete; \
    ClassName &operator=(const ClassName &) = delete;

#if defined(SKYWALKER_PLATFORM_WINDOWS)
#include <windows.h>
#include <string>
#include <ws2tcpip.h>
/**
 * 设置环境变量（Windows）
 */
inline void SkywalkerSetEnv(const char *name, const char *value)
{
    std::string envVar = std::string(name) + "=" + value;
    _putenv(envVar.c_str());
}
/**
 * IP 字符串转地址（平台统一包装）
 */
inline int SkywalkerInetPton(int af, const char *src, void *dst) { return inet_pton(af, src, dst); }
#else
#include <cstdlib>
#include <arpa/inet.h>
/**
 * 设置环境变量（Linux/Mac）
 */
inline void SkywalkerSetEnv(const char *name, const char *value) { setenv(name, value, 1); }
/**
 * IP 字符串转地址（平台统一包装）
 */
inline int SkywalkerInetPton(int af, const char *src, void *dst) { return inet_pton(af, src, dst); }
#endif

#endif // __SKYWALKER_PLATFORM_H__
