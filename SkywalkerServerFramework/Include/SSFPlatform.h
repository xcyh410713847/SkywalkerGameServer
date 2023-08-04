/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFPlatform.h
**作者: shyfan
**日期: 2023/08/04 16:39:49
**功能: 平台
*************************************************************************/

// 平台定义
#if defined(_WIN32) || defined(_WIN64)
#define SKYWALKER_SF_PLATFORM_WINDOWS
#endif

// 如果是在Linux平台
#if defined(__linux__)
#define SKYWALKER_SF_PLATFORM_LINUX
#endif

// 如果是在Mac平台
#if defined(__APPLE__)
#define SKYWALKER_SF_PLATFORM_MAC
#endif

// 平台头文件
#ifdef SKYWALKER_SF_PLATFORM_WINDOWS
#include <windows.h>
#elif SKYWALKER_SF_PLATFORM_LINUX
#include <unistd.h>
#elif SKYWALKER_SF_PLATFORM_MAC
#include <unistd.h>
#endif

// 平台函数
#ifdef SKYWALKER_SF_PLATFORM_WINDOWS
#define SKYWALKER_SF_SLEEP(ms) Sleep(ms)
#elif SKYWALKER_SF_PLATFORM_LINUX
#define SKYWALKER_SF_SLEEP(ms) usleep(ms * 1000)
#elif SKYWALKER_SF_PLATFORM_LINUX
#define SKYWALKER_SF_SLEEP(ms) usleep(ms * 1000)
#endif
