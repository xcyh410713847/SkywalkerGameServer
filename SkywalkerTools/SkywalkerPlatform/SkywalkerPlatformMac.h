/*************************************************************************
**文件: SkywalkerTools\SkywalkerPlatform\SkywalkerPlatformMac.h
**作者: shyfan
**日期: 2023/08/04 17:05:49
**功能: Mac平台
*************************************************************************/

#ifndef __SKYWALKER_PLATFORM_MAC_H__
#define __SKYWALKER_PLATFORM_MAC_H__

#if defined(__APPLE__)

#include <unistd.h>

#define SKYWALKER_PLATFORM_MAC

#define SKYWALKER_PLATFORM_SLEEP(ms) usleep(ms)

#endif // __APPLE__

#endif // __SKYWALKER_PLATFORM_MAC_H__
