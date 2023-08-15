/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFILog.h
**作者: shyfan
**日期: 2023/08/15 17:48:35
**功能: 日志接口
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_I_LOG_H__
#define __SKYWALKER_SERVER_FRAMEWORK_I_LOG_H__

#include "SSFCore.h"

SKYWALKER_SF_NAMESPACE_BEGIN

enum ELogLevel
{
    LogLevel_Debug = 0, // 调试
    LogLevel_Info,      // 信息
    LogLevel_Warning,   // 警告
    LogLevel_Error,     // 错误
    LogLevel_Fatal,     // 致命
};

/**
 * 显示文件名，行号和函数名
 */
#ifdef SKYWALKER_SF_LOG_TRACE
#define SKYWALKER_SF_LOG_FILE_LINE "[" << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << ")] "
#else
#define SKYWALKER_SF_LOG_FILE_LINE ""
#endif

/**
 * 日志模块定义
 */
#define SKYWALKER_SF_LOG_DEFINE(Module, ShowLevel) \
    static const char *SSFLogModuleName = #Module; \
    static const ELogLevel SSFLogModuleShowLevel = ShowLevel;

#define SKYWALKER_SF_LOG_MODULE "[" << SSFLogModuleName << "] "

/**
 * 日志打印
 */
#define SKYWALKER_SF_LOG_PRINT(Level, Content) \
    if (Level >= SSFLogModuleShowLevel)        \
        std::cout << Content << std::endl;

/**
 * 调试
 */
#define SKYWALKER_SF_LOG_DEBUG(Content) \
    SKYWALKER_SF_LOG_PRINT(LogLevel_Debug, "[Debug     ]" << SKYWALKER_SF_LOG_MODULE << SKYWALKER_SF_LOG_FILE_LINE << Content)

/**
 * 信息
 */
#define SKYWALKER_SF_LOG_INFO(Content) \
    SKYWALKER_SF_LOG_PRINT(LogLevel_Info, "[Info      ]" << SKYWALKER_SF_LOG_MODULE << SKYWALKER_SF_LOG_FILE_LINE << Content)

/**
 * 警告
 */
#define SKYWALKER_SF_LOG_WARNING(Content) \
    SKYWALKER_SF_LOG_PRINT(LogLevel_Warning, "[Warning   ]" << SKYWALKER_SF_LOG_MODULE << SKYWALKER_SF_LOG_FILE_LINE << Content)

/**
 * 错误
 */
#define SKYWALKER_SF_LOG_ERROR(Content) \
    SKYWALKER_SF_LOG_PRINT(LogLevel_Error, "[Error     ]" << SKYWALKER_SF_LOG_MODULE << SKYWALKER_SF_LOG_FILE_LINE << Content)

/**
 * 致命
 */
#define SKYWALKER_SF_LOG_FATAL(Content) \
    SKYWALKER_SF_LOG_PRINT(LogLevel_Fatal, "[Fatal     ]" << SKYWALKER_SF_LOG_MODULE << SKYWALKER_SF_LOG_FILE_LINE << Content)

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_I_LOG_H__
