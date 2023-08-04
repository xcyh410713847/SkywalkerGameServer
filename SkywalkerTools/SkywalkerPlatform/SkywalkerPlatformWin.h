/*************************************************************************
**文件: SkywalkerTools\SkywalkerPlatform\SkywalkerPlatformWin.h
**作者: shyfan
**日期: 2023/08/04 17:05:11
**功能: Windows平台
*************************************************************************/

#ifndef __SKYWALKER_PLATFORM_WIN_H__
#define __SKYWALKER_PLATFORM_WIN_H__

#if defined(_WIN32) || defined(_WIN64)

#include <Windows.h>

#define SKYWALKER_PLATFORM_WINDOWS

#define SKYWALKER_PLATFORM_SLEEP(ms) Sleep(ms)

#endif // _WIN32 || _WIN64

#endif // __SKYWALKER_PLATFORM_WIN_H__
