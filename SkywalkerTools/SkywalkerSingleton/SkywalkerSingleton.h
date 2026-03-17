/*************************************************************************
**文件: SkywalkerTools\SkywalkerSingleton\SkywalkerSingleton.h
**作者: shyfan
**日期: 2023/08/07 17:40:11
**功能: 单例
*************************************************************************/

#ifndef __SKYWALKER_SINGLETON_H__
#define __SKYWALKER_SINGLETON_H__

#include <memory>
#include <mutex>

/**
 * 声明单例接口宏
 * 要求：在类定义中使用
 */
#define SKYWALKER_SINGLETON_DECLARE(Class) \
private:                                   \
    static std::shared_ptr<Class> Instance; \
    static std::once_flag InitFlag;        \
                                             \
public:                                    \
    static std::shared_ptr<Class> GetInstance(); \
    static void DestroyInstance();         \
                                             \
protected:                                 \
    Class() = default;                     \
    virtual ~Class() = default;            \
    Class(const Class &) = delete;         \
    Class &operator=(const Class &) = delete; \
    \
    friend class std::shared_ptr<Class>;

/**
 * 定义单例实现宏
 * 要求：在一个 .cpp 中展开一次
 * 说明：使用 std::call_once 实现线程安全
 * 注意：使用 new 而非 make_shared，以支持 protected 构造函数
 */
#define SKYWALKER_SINGLETON_IMPLEMENT(Class) \
    std::shared_ptr<Class> Class::Instance = nullptr; \
    std::once_flag Class::InitFlag; \
    \
    std::shared_ptr<Class> Class::GetInstance() \
    { \
        std::call_once(InitFlag, []() { \
            Instance = std::shared_ptr<Class>(new Class()); \
        }); \
        return Instance; \
    } \
    \
    void Class::DestroyInstance() \
    { \
        Instance = nullptr; \
    }

#endif // __SKYWALKER_SINGLETON_H__
