﻿/*************************************************************************
**文件: SkywalkerTools\SkywalkerErrors\SkywalkerErrors.h
**作者: shyfan
**日期: 2023/07/30 12:39:30
**功能: 错误
*************************************************************************/

#ifndef __SKYWALKER_ERRORS_H__
#define __SKYWALKER_ERRORS_H__

#include <vector>
#include <string>
#include <sstream>

#define SKYWALKER_ERRORS_NAMESPACE Skywalker::Errors
#define SKYWALKER_ERRORS_NAMESPACE_BEGIN \
    namespace SKYWALKER_ERRORS_NAMESPACE \
    {
#define SKYWALKER_ERRORS_NAMESPACE_END } // namespace SKYWALKER_ERRORS_NAMESPACE

SKYWALKER_ERRORS_NAMESPACE_BEGIN

template <typename T>
class CSkywalkerErrors
{
    std::vector<T> errors;               // 错误列表
    std::vector<std::string> errorTrace; // 错误堆栈

public:
    CSkywalkerErrors(){};
    ~CSkywalkerErrors()
    {
        errors.clear();
        errorTrace.clear();
    }

    /** 添加错误
     * @param const T &Error	: 错误
     * @param string trace		: 错误堆栈
     */
    void AddError(const T &Error, std::string trace)
    {
        errors.push_back(Error);
        errorTrace.push_back(trace);
    }

    /** 获取错误列表
     * @return const vector<T>	: 错误列表
     */
    const std::vector<T> GetErrors() const
    {
        return errors;
    };

    /** 获取错误堆栈
     * @return const vector<string>	: 错误堆栈
     */
    const std::vector<std::string> GetErrorTrace() const
    {
        return errorTrace;
    }

    /** 获取首个错误
     * @return const T	: 首个错误
     */
    const T GetFirstError() const
    {
        if (errors.size() > 0)
        {
            return errors[0];
        }
        else
        {
            T t;
            return t;
        }
    }

    /** 是否有效
     * @return bool	: true 有效, false 无效
     */
    bool IsValid() const
    {
        return !errors.empty();
    }
};

/**
 * 不带堆栈的错误
 */
#define SKYWALKER_ERRORS_WRAP(CppErrors, Error) CppErrors.AddError(Error, "")

/**
 * 带堆栈的错误
 */
#define SKYWALKER_ERRORS_WRAP_TRACE(CppErrors, Error)                             \
    {                                                                             \
        std::stringstream ss;                                                     \
        ss << "[" << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << ")] "; \
        CppErrors.AddError(Error, ss.str());                                      \
    }

SKYWALKER_ERRORS_NAMESPACE_END

#endif // __SKYWALKER_ERRORS_H__
