/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFILog.h
**作者: shyfan
**日期: 2023/08/15 17:48:35
**功能: 日志接口
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_I_LOG_H__
#define __SKYWALKER_SERVER_FRAMEWORK_I_LOG_H__

#include "SSFCore.h"

SSF_NAMESPACE_BEGIN

enum ELogLevel
{
    LogLevel_Framework = 0, // 框架，用于框架内部调试
    LogLevel_Debug,         // 调试，用于开发调试
    LogLevel_Info,          // 信息，用于显示信息
    LogLevel_Warning,       // 警告，用于显示警告
    LogLevel_Error,         // 错误，用于显示错误
    LogLevel_Fatal,         // 致命，用于显示致命错误
};

#define SSF_LOG_LEVEL LogLevel_Framework

/**
 * 显示文件名，行号和函数名
 */
#ifdef SSF_LOG_TRACE
#define SSF_LOG_FILE_LINE "[" << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << ")] "
#else
#define SSF_LOG_FILE_LINE ""
#endif

/**
 * 日志模块定义
 */
#define SSF_LOG_DEFINE(Module, ShowLevel)          \
    static const char *SSFLogModuleName = #Module; \
    static const ELogLevel SSFLogModuleShowLevel = ShowLevel;

#define SSF_LOG_MODULE "[" << SSFLogModuleName << "] "

/**
 * 日志打印
 */
#define SSF_LOG_PRINT_EX(Level, Content) \
    if (Level >= SSF_LOG_LEVEL)          \
        std::cout << Content << std::endl;

/**
 * 日志打印
 */
#define SSF_LOG_PRINT(Level, Content)   \
    if (Level >= SSFLogModuleShowLevel) \
    SSF_LOG_PRINT_EX(Level, Content)

/**
 * 框架
 */
#define SSF_LOG_FRAMEWORK(Content) \
    SSF_LOG_PRINT(LogLevel_Framework, "[Framework ]" << SSF_LOG_MODULE << SSF_LOG_FILE_LINE << Content)

/**
 * 调试
 */
#define SSF_LOG_DEBUG(Content) \
    SSF_LOG_PRINT(LogLevel_Debug, "[Debug     ]" << SSF_LOG_MODULE << SSF_LOG_FILE_LINE << Content)

/**
 * 调试
 */
#define SSF_LOG_DEBUG_MODULE(Content) \
    SSF_LOG_DEBUG_OBJECT(this, Content)

/**
 * 调试
 */
#define SSF_LOG_DEBUG_OBJECT(Object, Content) \
    SSF_LOG_DEBUG(Object->GetObjectClassName() << " " << " [ObjectGUID " << Object->GetObjectGUID() << "] " << Content)

/**
 * 信息
 */
#define SSF_LOG_INFO(Content) \
    SSF_LOG_PRINT(LogLevel_Info, "[Info      ]" << SSF_LOG_MODULE << SSF_LOG_FILE_LINE << Content)

/**
 * 警告
 */
#define SSF_LOG_WARNING(Content) \
    SSF_LOG_PRINT(LogLevel_Warning, "[Warning   ]" << SSF_LOG_MODULE << SSF_LOG_FILE_LINE << Content)

/**
 * 错误
 */
#define SSF_LOG_ERROR(Content) \
    SSF_LOG_PRINT(LogLevel_Error, "[Error     ]" << SSF_LOG_MODULE << SSF_LOG_FILE_LINE << Content)

/**
 * 致命
 */
#define SSF_LOG_FATAL(Content) \
    SSF_LOG_PRINT(LogLevel_Fatal, "[Fatal     ]" << SSF_LOG_MODULE << SSF_LOG_FILE_LINE << Content)

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_I_LOG_H__
