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

namespace Skywalker
{
    namespace Errors
    {
        std::string emptyTrace = "";

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
             * @param const T &error	: 错误
             * @param string trace		: 错误堆栈
             */
            void AddError(const T &error, std::string trace)
            {
                errors.push_back(error);
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

// 不带堆栈的错误
#define CPP_ERRORS_WRAP(cppErrors, error) cppErrors.AddError(error, emptyTrace)

// 带堆栈的错误
#define CPP_ERRORS_WRAP_TRACE(cppErrors, error)                                   \
    {                                                                             \
        stringstream ss;                                                          \
        ss << "[" << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << ")] "; \
        cppErrors.AddError(error, ss.str());                                      \
    }
    } // namespace Errors
} // namespace Skywalker

#endif // __SKYWALKER_ERRORS_H__
