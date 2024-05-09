/*************************************************************************
**文件: SkywalkerTools\SkywalkerPlatform\SkywalkerPlatformLinux.h
**作者: shyfan
**日期: 2023/08/04 17:05:29
**功能: Linux平台
*************************************************************************/

#ifndef __SKYWALKER_PLATFORM_LINUX_H__
#define __SKYWALKER_PLATFORM_LINUX_H__

#if defined(__linux__)

#include <unistd.h>
#include <dlfcn.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define SKYWALKER_PLATFORM_LINUX

#define SKYWALKER_PLATFORM_SLEEP(ms) usleep(ms)

#endif // __linux__

#endif // __SKYWALKER_PLATFORM_LINUX_H__
