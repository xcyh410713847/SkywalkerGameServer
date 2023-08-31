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

#include "SSFPlatform.h"

#pragma region Common Macro

/**
 * 定义命名空间
 */
#define SKYWALKER_SF_NAMESPACE Skywalker::ServerFramework

/**
 * 命名空间开始
 */
#define SKYWALKER_SF_NAMESPACE_BEGIN \
    namespace SKYWALKER_SF_NAMESPACE \
    {

/**
 * 命名空间结束
 */
#define SKYWALKER_SF_NAMESPACE_END \
    } // Skywalker::ServerFramework

/**
 * 使用命名空间
 */
#define SKYWALKER_SF_NAMESPACE_USE using namespace SKYWALKER_SF_NAMESPACE;

/**
 * 导出
 */
#if defined(SKYWALKER_PLATFORM_WINDOWS)
#define SKYWALKER_SF_DLL_EXPORT extern "C" __declspec(dllexport)
#define SKYWALKER_SF_API __declspec(dllexport)
#else
#define SKYWALKER_SF_DLL_EXPORT extern "C" __attribute__((visibility("default")))
#define SKYWALKER_SF_API __attribute__((visibility("default")))
#endif

#pragma endregion Common Macro

#pragma region Function Macro

/**
 * 获取类名
 */
#define SKYWALKER_SF_CLASS_NAME(ClassName) typeid(ClassName).name()

/**
 * 断言
 */
#define SKYWALKER_SF_ASSERT(Condition) assert((Condition))

#pragma endregion Function Macro

#pragma region Common Type

/**
 * 普通指针
 */
#define SKYWALKER_SF_PTR(T) T *
#define SKYWALKER_SF_CONST_PTR(T) const T *
#define SKYWALKER_SF_PTR_VALID(Obj) (Obj != nullptr)

/**
 * 共享指针
 */
#define SKYWALKER_SF_SHARED_PTR(T) std::shared_ptr<T>

/**
 * Cast
 */
#define SKYWALKER_SF_POINT_CAST(T) dynamic_cast<SKYWALKER_SF_PTR(T)>

#pragma endregion Common Type

#pragma region Iterator Macro

/**
 * 通用迭代器
 */
#define SKYWALKER_SF_COMMON_ITERATOR(Iterator, Container) \
    for (auto Iterator = Container.begin(); Iterator != Container.end(); ++Iterator)

#pragma endregion Iterator Macro

#pragma region Pointer Macro

SKYWALKER_SF_NAMESPACE_BEGIN

/**
 * 对象
 */
class SSFObject;
#define SKYWALKER_SF_PTR_OBJECT SKYWALKER_SF_PTR(SSFObject)

/**
 * 插件管理器
 */
class SSFOPluginManager;
#define SKYWALKER_SF_PTR_PLUGIN_MANAGER SKYWALKER_SF_PTR(SSFOPluginManager)

/**
 * 插件
 */
class SSFOPlugin;
#define SKYWALKER_SF_PTR_PLUGIN SKYWALKER_SF_PTR(SSFOPlugin)

/**
 * 模块
 */
class SSFOModule;
#define SKYWALKER_SF_PTR_MODULE SKYWALKER_SF_PTR(SSFOModule)

SKYWALKER_SF_NAMESPACE_END

#pragma endregion Pointer Macro

#endif // __SKYWALKER_SERVER_FRAMEWORK_CORE_H__
