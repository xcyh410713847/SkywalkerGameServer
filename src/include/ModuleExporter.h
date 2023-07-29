/*************************************************************************
**文件: ModuleExporter.h
**作者: shyfan
**日期: 2023/07/26 12:12:48
**功能: 模块导出器
*************************************************************************/

#ifndef __SKYWALKER_GS_INCLUDE_MODULE_EXPORTER_H__
#define __SKYWALKER_GS_INCLUDE_MODULE_EXPORTER_H__

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __attribute__((visibility("default")))
#endif

#endif // _SKYWALKER_GS_INCLUDE_MODULE_EXPORTER_H__