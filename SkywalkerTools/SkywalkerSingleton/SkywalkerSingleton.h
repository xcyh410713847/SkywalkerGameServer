/*************************************************************************
**文件: SkywalkerTools\SkywalkerSingleton\SkywalkerSingleton.h
**作者: shyfan
**日期: 2023/08/07 17:40:11
**功能: 单例
*************************************************************************/

#ifndef __SKYWALKER_SINGLETON_H__
#define __SKYWALKER_SINGLETON_H__

/**
 * 声明
 */
#define SKYWALKER_SINGLETON_DECLARE(Class) \
private:                                   \
    static Class *Instance;                \
                                           \
public:                                    \
    static Class *GetInstance();           \
    static void DestroyInstance();         \
                                           \
protected:                                 \
    Class() = default;                     \
    virtual ~Class() = default;            \
    Class(const Class &) = delete;         \
    Class &operator=(const Class &) = delete;

/**
 * 定义
 */
#define SKYWALKER_SINGLETON_IMPLEMENT(Class) \
    Class *Class::Instance = nullptr;        \
                                             \
    Class *Class::GetInstance()              \
    {                                        \
        if (Instance == nullptr)             \
        {                                    \
            Instance = new Class();          \
        }                                    \
        return Instance;                     \
    }                                        \
                                             \
    void Class::DestroyInstance()            \
    {                                        \
        if (Instance != nullptr)             \
        {                                    \
            delete Instance;                 \
            Instance = nullptr;              \
        }                                    \
    }

#endif // __SKYWALKER_SINGLETON_H__
