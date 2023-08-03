/*************************************************************************
**文件: SkywalkerGameServer\Include\SGSModuleExporter.h
**作者: shyfan
**日期: 2023/08/03 11:20:39
**功能: 模块导出
*************************************************************************/

#ifndef __SKYWALKER_GS_INCLUDE_MODULE_EXPORTER_H__
#define __SKYWALKER_GS_INCLUDE_MODULE_EXPORTER_H__

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __attribute__((visibility("default")))
#endif

#endif // _SKYWALKER_GS_INCLUDE_MODULE_EXPORTER_H__
