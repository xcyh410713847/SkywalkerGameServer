/*************************************************************************
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

/** 错误工具命名空间宏 */
#define SKYWALKER_ERRORS_NAMESPACE Skywalker::Errors
#define SKYWALKER_ERRORS_NAMESPACE_BEGIN \
    namespace SKYWALKER_ERRORS_NAMESPACE \
    {
#define SKYWALKER_ERRORS_NAMESPACE_END } // namespace SKYWALKER_ERRORS_NAMESPACE

SKYWALKER_ERRORS_NAMESPACE_BEGIN

/**
 * 通用错误容器
 * T 为错误类型（通常是 enum 或错误对象）
 */
template <typename T>
class CSkywalkerErrors
{
    std::vector<T> errors;               // 错误列表
    std::vector<std::string> errorTrace; // 错误堆栈

public:
    CSkywalkerErrors() {};
    ~CSkywalkerErrors()
    {
        errors.clear();
        errorTrace.clear();
    }

    /**
     * 添加错误
     * @param Error 错误对象
     * @param trace 错误追踪信息（可为空）
     */
    void AddError(const T &Error, const std::string &trace)
    {
        errors.push_back(Error);
        errorTrace.push_back(trace);
    }

    /**
     * 获取错误列表（const引用）
     * @return 错误列表引用
     */
    const std::vector<T> &GetErrors() const
    {
        return errors;
    };

    /**
     * 获取错误追踪列表（const引用）
     * @return 错误追踪列表引用
     */
    const std::vector<std::string> &GetErrorTrace() const
    {
        return errorTrace;
    }

    /**
     * 获取首个错误
     * 当无错误时返回 T{} 默认值
     */
    const T GetFirstError() const
    {
        if (!errors.empty())
        {
            return errors[0];
        }
        return T{};
    }

    /**
     * 是否存在错误
     * @return true 表示当前容器已有错误记录
     */
    bool IsValid() const
    {
        return !errors.empty();
    }
};

/**
 * 不带追踪信息的错误包装
 */
#define SKYWALKER_ERRORS_WRAP(CppErrors, Error) CppErrors.AddError(Error, "")

/**
 * 带文件/行号/函数信息的错误包装
 */
#define SKYWALKER_ERRORS_WRAP_TRACE(CppErrors, Error)                             \
    {                                                                             \
        std::stringstream ss;                                                     \
        ss << "[" << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << ")] "; \
        CppErrors.AddError(Error, ss.str());                                      \
    }

SKYWALKER_ERRORS_NAMESPACE_END

#endif // __SKYWALKER_ERRORS_H__
