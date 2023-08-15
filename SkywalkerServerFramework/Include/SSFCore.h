/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFCore.h
**作者: shyfan
**日期: 2023/08/02 17:16:32
**功能: 核心头文件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_CORE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_CORE_H__

#include <iostream>

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
#define SKYWALKER_SF_NAMESPACE_USE using namespace Skywalker::ServerFramework;

/**
 * 导出
 */
#define SKYWALKER_SF_API extern "C" __declspec(dllexport)

#pragma endregion Common Macro

#pragma region Function Macro

/**
 * 释放对象
 */
#define SKYWALKER_SF_RELEASE(Obj) \
    if (Obj)                      \
    {                             \
        Obj->Release();           \
        Obj = nullptr;            \
    }

/**
 * 获取类名
 */
#define SKYWALKER_SF_CLASS_NAME(ClassName) (#ClassName)

#pragma endregion Function Macro

#pragma region Common Type

/**
 * 共享指针
 */
template <typename T>
using SSFSharedPtr = std::shared_ptr<T>;

#pragma endregion Common Type

#pragma region Iterator Macro

/**
 * 通用迭代器
 */
#define SKYWALKER_SF_COMMON_ITERATOR(Iterator, Container) \
    for (auto Iterator = Container.begin(); Iterator != Container.end(); ++Iterator)

#pragma endregion Iterator Macro

#endif // __SKYWALKER_SERVER_FRAMEWORK_CORE_H__
