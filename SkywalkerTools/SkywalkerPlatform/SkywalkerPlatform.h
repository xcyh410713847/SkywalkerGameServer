/*************************************************************************
**文件: SkywalkerTools\SkywalkerPlatform\SkywalkerPlatform.h
**作者: shyfan
**日期: 2023/08/04 16:57:26
**功能: 平台
*************************************************************************/

#ifndef __SKYWALKER_PLATFORM_H__
#define __SKYWALKER_PLATFORM_H__

#include "SkywalkerPlatformLinux.h"
#include "SkywalkerPlatformMac.h"
#include "SkywalkerPlatformWin.h"

#if defined(SKYWALKER_PLATFORM_WINDOWS)
#define SKYWALKER_DYNAMIC_LIB_HANDLE HINSTANCE
#define SKYWALKER_DYNAMIC_LIB_LOAD(LibName) LoadLibraryExA(LibName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)
#define SKYWALKER_DYNAMIC_LIB_UNLOAD(Instance) FreeLibrary(Instance)
#define SKYWALKER_DYNAMIC_LIB_GET_SYMBOL(Instance, ProcName) GetProcAddress(Instance, ProcName)
#define SKYWALKER_DYNAMIC_LIB_EXT ".dll"
#else
#define SKYWALKER_DYNAMIC_LIB_HANDLE void *
#define SKYWALKER_DYNAMIC_LIB_LOAD(LibName) dlopen(LibName, RTLD_LAZY | RTLD_GLOBAL)
#define SKYWALKER_DYNAMIC_LIB_UNLOAD(Instance) dlclose(Instance)
#define SKYWALKER_DYNAMIC_LIB_GET_SYMBOL(Instance, ProcName) dlsym(Instance, ProcName)
#define SKYWALKER_DYNAMIC_LIB_EXT ".so"
#endif

#endif // __SKYWALKER_PLATFORM_H__
