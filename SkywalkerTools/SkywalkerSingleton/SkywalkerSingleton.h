/*************************************************************************
**文件: SkywalkerTools\SkywalkerSingleton\SkywalkerSingleton.h
**作者: shyfan
**日期: 2023/08/07 17:40:11
**功能: 单例
*************************************************************************/

#ifndef __SKYWALKER_SINGLETON_H__
#define __SKYWALKER_SINGLETON_H__

#define SKYWALKER_SINGLETON_DECLARE(Class) \
public:                                    \
    static Class *GetInstance();           \
    static void DestroyInstance();         \
                                           \
private:                                   \
    static Class *Instance;                \
    Class();                               \
    ~Class();                              \
    Class(const Class &);                  \
    Class &operator=(const Class &);

#define SKYWALKER_SINGLETON_IMPLEMENT(Class) \
    Class *Class::Instance = nullptr;        \
    Class *Class::GetInstance()              \
    {                                        \
        if (Instance == nullptr)             \
        {                                    \
            Instance = new Class();          \
        }                                    \
        return Instance;                     \
    }                                        \
    void Class::DestroyInstance()            \
    {                                        \
        if (Instance != nullptr)             \
        {                                    \
            delete Instance;                 \
            Instance = nullptr;              \
        }                                    \
    }                                        \
    Class::Class() {}                        \
    Class::~Class() {}                       \
    Class::Class(const Class &) {}           \
    Class &Class::operator=(const Class &) { return *this; }

#endif // __SKYWALKER_SINGLETON_H__
