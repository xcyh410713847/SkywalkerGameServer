/*************************************************************************
**文件: SkywalkerFramework\Include\SFILog.h
**作者: shyfan
**日期: 2023/08/15 17:48:35
**功能: 日志接口
*************************************************************************/

#ifndef __SKYWALKER_FRAMEWORK_SFILOG_H__
#define __SKYWALKER_FRAMEWORK_SFILOG_H__

#include "SFCore.h"

SF_NAMESPACE_BEGIN

/**
 * 日志等级
 */
enum ESFLogLevel
{
    Framework = 0, // 框架，用于框架内部调试
    Debug,         // 调试，用于开发调试
    Info,          // 信息，用于显示信息
    Warning,       // 警告，用于显示警告
    Error,         // 错误，用于显示错误
    Fatal,         // 致命，用于显示致命错误
};

#define SF_LOG_LEVEL Framework

/**
 * 显示文件名，行号和函数名
 */
#ifdef SF_LOG_TRACE
#define SF_LOG_FILE_LINE "[" << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << ")] "
#else
#define SF_LOG_FILE_LINE ""
#endif

/**
 * 定义日志模块名称与显示等级
 */
#define SF_LOG_DEFINE(Module, ShowLevel)          \
    static const char *SFLogModuleName = #Module; \
    static const ESFLogLevel SFLogModuleShowLevel = ShowLevel;

#define SF_LOG_MODULE "[" << SFLogModuleName << "] "

/**
 * 按全局等级过滤并输出日志
 */
#define SF_LOG_PRINT_EX(Level, Content) \
    if (Level >= SF_LOG_LEVEL)          \
        std::cout << Content << std::endl;

/**
 * 按模块等级过滤并输出日志
 */
#define SF_LOG_PRINT(Level, Content)   \
    if (Level >= SFLogModuleShowLevel) \
    SF_LOG_PRINT_EX(Level, Content)

/**
 * 框架
 */
#define SF_LOG_FRAMEWORK(Content) \
    SF_LOG_PRINT(Framework, "[Framework ]" << SF_LOG_MODULE << SF_LOG_FILE_LINE << Content)

/**
 * 调试
 */
#define SF_LOG_DEBUG(Content) \
    SF_LOG_PRINT(ESFLogLevel::Debug, "[Debug     ]" << SF_LOG_MODULE << SF_LOG_FILE_LINE << Content)

/**
 * 调试（当前对象）
 */
#define SF_LOG_DEBUG_MODULE(Content) \
    SF_LOG_DEBUG_OBJECT(this, Content)

/**
 * 调试（指定对象）
 */
#define SF_LOG_DEBUG_OBJECT(Object, Content) \
    SF_LOG_DEBUG(Object->GetObjectClassName() << " " << " [ObjectGUID " << Object->GetObjectGUID() << "] " << Content)

/**
 * 信息
 */
#define SF_LOG_INFO(Content) \
    SF_LOG_PRINT(ESFLogLevel::Info, "[Info      ]" << SF_LOG_MODULE << SF_LOG_FILE_LINE << Content)

/**
 * 警告
 */
#define SF_LOG_WARNING(Content) \
    SF_LOG_PRINT(ESFLogLevel::Warning, "[Warning   ]" << SF_LOG_MODULE << SF_LOG_FILE_LINE << Content)

/**
 * 错误
 */
#define SF_LOG_ERROR(Content) \
    SF_LOG_PRINT(ESFLogLevel::Error, "[Error     ]" << SF_LOG_MODULE << SF_LOG_FILE_LINE << Content)

/**
 * 致命
 */
#define SF_LOG_FATAL(Content) \
    SF_LOG_PRINT(ESFLogLevel::Fatal, "[Fatal     ]" << SF_LOG_MODULE << SF_LOG_FILE_LINE << Content)

SF_NAMESPACE_END

#endif // __SKYWALKER_FRAMEWORK_SFILOG_H__
