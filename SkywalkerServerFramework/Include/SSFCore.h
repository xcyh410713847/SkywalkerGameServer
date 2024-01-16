/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFCore.h
**作者: shyfan
**日期: 2023/08/02 17:16:32
**功能: 核心头文件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_CORE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_CORE_H__

#include <iostream>
#include <string>
#include <assert.h>
#include <memory>
#include <list>
#include <map>
#include <regex>

#include "SSFPlatform.h"

#pragma region Namespace Macro

/**
 * 定义命名空间
 */
#define SSF_NAMESPACE Skywalker::ServerFramework

/**
 * 命名空间开始
 */
#define SSF_NAMESPACE_BEGIN \
    namespace SSF_NAMESPACE \
    {

/**
 * 命名空间结束
 */
#define SSF_NAMESPACE_END \
    } // Skywalker::ServerFramework

/**
 * 使用命名空间
 */
#define SSF_NAMESPACE_USING using namespace SSF_NAMESPACE;

#pragma endregion Namespace Macro

SSF_NAMESPACE_BEGIN

#pragma region Function Macro

/**
 * 获取类名
 */

#define SSF_CLASS_NAME(Class, ClassName)                 \
    ClassName = typeid(Class).name();                    \
    std::smatch Match;                                   \
    std::regex RegexObj(R"(class ([\w:]+))");            \
    if (std::regex_search(ClassName, Match, RegexObj) && \
        Match.size() > 1)                                \
    {                                                    \
        ClassName = Match[1].str();                      \
    }

/**
 * 断言
 */
#define SSF_ASSERT(Condition) assert((Condition))

#pragma endregion Function Macro

#pragma region Common Type

/**
 * 裸指针
 */
#define SSF_PTR(T) T *
#define SSF_CONST_PTR(T) const T *
#define SSF_PTR_VALID(Obj) (Obj != nullptr)
#define SSF_PTR_INVALID(Obj) (Obj == nullptr)

/**
 * 智能指针
 */
#define SSF_UNIQUE_PTR(T) std::unique_ptr<T>
#define SSF_MAKE_UNIQUE_PTR(T) std::make_unique<T>();

#define SSF_CONST_UNIQUE_PTR(T) std::unique_ptr<const T>
#define SSF_MAKE_CONST_UNIQUE_PTR(T) std::make_unique<const T>();

#define SSF_SHARED_PTR(T) std::shared_ptr<T>
#define SSF_MAKE_SHARED_PTR(T, ...) std::make_shared<T>(__VA_ARGS__);

#define SSF_CONST_SHARED_PTR(T) std::shared_ptr<const T>
#define SSF_MAKE_CONST_SHARED_PTR(T) std::make_shared<const T>();

#define SSF_WEAK_PTR(T) std::weak_ptr<T>
#define SSF_MAKE_WEAK_PTR(T) std::make_weak_ptr<T>();

#define SSF_CONST_WEAK_PTR(T) std::weak_ptr<const T>
#define SSF_MAKE_CONST_WEAK_PTR(T) std::make_weak_ptr<const T>();

/**
 * Cast
 */
#define SSF_PTR_DYNAMIC_CAST(T) dynamic_cast<SSF_PTR(T)>
#define SSF_PTR_STATIC_CAST(T) static_cast<SSF_PTR(T)>

#pragma endregion Common Type

#pragma region Iterator Macro

/**
 * 通用迭代器
 */
#define SSF_COMMON_ITERATOR(Iterator, Container) \
    for (auto Iterator = Container.begin(); Iterator != Container.end(); ++Iterator)

#pragma endregion Iterator Macro

#pragma region Pointer Macro

/**
 * 简单对象
 */
class SSFObjectSimple;
#define SSF_PTR_OBJECT_SIMPLE SSF_PTR(SSFObjectSimple)

/**
 * 对象
 */
class SSFObject;
#define SSF_PTR_OBJECT SSF_PTR(SSFObject)

/**
 * 插件管理器
 */
class SSFOPluginManager;
#define SSF_PTR_PLUGIN_MANAGER SSF_PTR(SSFOPluginManager)

/**
 * 插件
 */
class SSFOPlugin;
#define SSF_PTR_PLUGIN SSF_PTR(SSFOPlugin)

/**
 * 模块
 */
class SSFOModule;
#define SSF_PTR_MODULE SSF_PTR(SSFOModule)

#pragma endregion Pointer Macro

#pragma region Plugin Macro

/**
 * 插件导出
 */
#ifdef SKYWALKER_SF_DYNAMIC_PLUGIN
#define SSF_PLUGIN_EXPORT(PluginClass)                                       \
    SSF_DLL_EXPORT void DllStartPlugin(SSF_PTR_PLUGIN_MANAGER PluginManager) \
    {                                                                        \
        SSF_REGISTER_PLUGIN(PluginManager, PluginClass);                     \
    }                                                                        \
    SSF_DLL_EXPORT void DllStopPlugin(SSF_PTR_PLUGIN_MANAGER PluginManager)  \
    {                                                                        \
        SSF_UNREGISTER_PLUGIN(PluginManager, PluginClass);                   \
    }
#else
#define SSF_PLUGIN_EXPORT(PluginClass)
#endif // SKYWALKER_SF_DYNAMIC_PLUGIN

#pragma endregion Plugin Macro

#pragma region typedef

typedef bool SSFBool;
typedef char SSFByte;
typedef unsigned char SSFUByte;
typedef short SSFShort;
typedef unsigned short SSFUShort;
typedef int SSFInt;
typedef unsigned int SSFUInt;
typedef long SSFLong;
typedef unsigned long SSFULong;
typedef long long SSFLongLong;
typedef unsigned long long SSFULongLong;
typedef float SSFFloat;
typedef double SSFDouble;
typedef std::string SSFString;

#define TRUE 1
#define FALSE 0

#pragma endregion typedef

#pragma region Data Structure

template <typename Key, typename Value>
using SSFMap = std::map<Key, Value>;

#pragma endregion Data Structure

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_CORE_H__
