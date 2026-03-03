/*************************************************************************
**文件: SkywalkerFramework\Include\SFCore.h
**作者: shyfan
**日期: 2023/08/02 17:16:32
**功能: 核心头文件
*************************************************************************/

#ifndef __SKYWALKER_FRAMEWORK_SFCORE_H__
#define __SKYWALKER_FRAMEWORK_SFCORE_H__

#include <iostream>
#include <string>
#include <assert.h>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <list>
#include <map>
#include <regex>
#include <filesystem>

#include "SFPlatform.h"

#pragma region Namespace Macro

/**
 * 定义命名空间
 */
#define SF_NAMESPACE Skywalker::Framework

/**
 * 命名空间开始
 */
#define SF_NAMESPACE_BEGIN \
    namespace SF_NAMESPACE \
    {

/**
 * 命名空间结束
 */
#define SF_NAMESPACE_END \
    } // Skywalker::Framework

/**
 * 使用命名空间
 */
#define SF_NAMESPACE_USING using namespace SF_NAMESPACE;

#pragma endregion Namespace Macro

SF_NAMESPACE_BEGIN

#pragma region Function Macro

/**
 * 获取类名字符串
 */

#define SF_CLASS_NAME(Class, ClassName)                  \
    ClassName = typeid(Class).name();                    \
    std::smatch Match;                                   \
    std::regex RegexObj(R"(class ([\w:]+))");            \
    if (std::regex_search(ClassName, Match, RegexObj) && \
        Match.size() > 1)                                \
    {                                                    \
        ClassName = Match[1].str();                      \
    }

#pragma endregion Function Macro

#pragma region Assert Macro

/**
 * 断言
 */
#define SF_ASSERT(Condition) assert((Condition))

/**
 * 断言 是否继承自 TBase
 */
#define SF_ASSERT_IS_BASE_OF(TBase, T) \
    static_assert(std::is_base_of<TBase, T>{}, "T must be derived from TBase")

#pragma endregion Assert Macro

#pragma region Common Type

/**
 * 裸指针
 */
#define SF_PTR(T) T *
#define SF_CONST_PTR(T) const T *
#define SF_PTR_VALID(Obj) (Obj != nullptr)
#define SF_PTR_INVALID(Obj) (Obj == nullptr)

/**
 * 智能指针
 */
#define SF_UNIQUE_PTR(T) std::unique_ptr<T>
#define SF_MAKE_UNIQUE_PTR(T) std::make_unique<T>();
#define SF_UNIQUE_PTR_CAST(T, Obj) std::unique_ptr<T>(dynamic_cast<T *>(Obj))

#define SF_CONST_UNIQUE_PTR(T) std::unique_ptr<const T>
#define SF_MAKE_CONST_UNIQUE_PTR(T) std::make_unique<const T>();

#define SF_SHARED_PTR(T) std::shared_ptr<T>
#define SF_MAKE_SHARED_PTR(T, ...) std::make_shared<T>(__VA_ARGS__);

#define SF_CONST_SHARED_PTR(T) std::shared_ptr<const T>
#define SF_MAKE_CONST_SHARED_PTR(T) std::make_shared<const T>();

#define SF_WEAK_PTR(T) std::weak_ptr<T>
#define SF_MAKE_WEAK_PTR(T) std::make_weak_ptr<T>();

#define SF_CONST_WEAK_PTR(T) std::weak_ptr<const T>
#define SF_MAKE_CONST_WEAK_PTR(T) std::make_weak_ptr<const T>();

/**
 * 指针类型转换宏
 */
#define SF_PTR_DYNAMIC_CAST(T) dynamic_cast<SF_PTR(T)>
#define SF_PTR_STATIC_CAST(T) static_cast<SF_PTR(T)>

#pragma endregion Common Type

#pragma region Iterator Macro

/**
 * 通用迭代器
 */
#define SF_COMMON_ITERATOR(Iterator, Container) \
    for (auto Iterator = Container.begin(); Iterator != Container.end(); ++Iterator)

#pragma endregion Iterator Macro

#pragma region Plugin Macro

/**
 * 注册库
 */
#define SF_REGISTER_LIBRARY(LibraryName)                                              \
    SFObjectErrors LibraryName##Errors;                                               \
    if (DynamicLibMap.find(LibraryName) != DynamicLibMap.end())                       \
    {                                                                                 \
        SF_ERROR_TRACE(LibraryName##Errors, ESFError::Plugin_Load_Repeated);          \
        return;                                                                       \
    }                                                                                 \
    SFDynamicLib *DynamicLib = new SFDynamicLib(LibraryName);                         \
    if (DynamicLib == nullptr)                                                        \
    {                                                                                 \
        SF_ERROR_TRACE(LibraryName##Errors, ESFError::Plugin_Load_DynamicLibNullptr); \
        return;                                                                       \
    }                                                                                 \
    DynamicLibMap.insert(std::make_pair(LibraryName, DynamicLib));                    \
    if (!DynamicLib->Load())                                                          \
    {                                                                                 \
        SF_ERROR_TRACE(LibraryName##Errors, ESFError::Plugin_Load_Failed);            \
        return;                                                                       \
    }                                                                                 \
    SF_LOG_INFO("Register Library [" << LibraryName << "] Success");

/**
 * 注册插件
 */
#define SF_REGISTER_PLUGIN(PluginManager, PluginClass)                                                              \
    SF_ASSERT_IS_BASE_OF(SFPlugin, PluginClass);                                                                    \
    SFObjectErrors PluginClass##Errors;                                                                             \
    SSFPluginContext PluginClass##Context;                                                                          \
    PluginClass##Context.PluginManager = PluginManager;                                                             \
    SF_PTR(SFPlugin)                                                                                                \
    Plugin = PluginManager->NewObject<PluginClass>(PluginClass##Context, PluginClass##Errors);                      \
    PluginManager->RegisterPlugin(PluginClass##Errors, Plugin);                                                     \
    if (PluginClass##Errors.IsValid())                                                                              \
    {                                                                                                               \
        auto FirstError = PluginClass##Errors.GetFirstError();                                                      \
        SF_LOG_INFO("Register Plugin [" << Plugin->GetObjectClassName() << "] Failed " << FirstError.GetContent()); \
    }                                                                                                               \
    else                                                                                                            \
    {                                                                                                               \
        SF_LOG_INFO("Register Plugin [" << Plugin->GetObjectClassName() << "] Success");                            \
    }

/**
 * 注销插件
 */
#define SF_UNREGISTER_PLUGIN(PluginManager, PluginClass)                                                              \
    SF_ASSERT_IS_BASE_OF(SFPlugin, PluginClass);                                                                      \
    SFObjectErrors PluginClass##Errors;                                                                               \
    SFString PluginClassName;                                                                                         \
    SF_CLASS_NAME(PluginClass, PluginClassName);                                                                      \
    SF_PTR(SFPlugin)                                                                                                  \
    Plugin = PluginManager->GetPlugin(PluginClassName);                                                               \
    PluginManager->UnregisterPlugin(PluginClass##Errors, Plugin);                                                     \
    if (PluginClass##Errors.IsValid())                                                                                \
    {                                                                                                                 \
        auto FirstError = PluginClass##Errors.GetFirstError();                                                        \
        SF_LOG_INFO("Unregister Plugin [" << Plugin->GetObjectClassName() << "] Failed " << FirstError.GetContent()); \
    }                                                                                                                 \
    else                                                                                                              \
    {                                                                                                                 \
        SF_LOG_INFO("Unregister Plugin [" << Plugin->GetObjectClassName() << "] Success");                            \
    }                                                                                                                 \
    Plugin->Release(PluginClass##Errors);

/**
 * 注册模块
 */
#define SF_REGISTER_MODULE(ModuleClass)                                                      \
    SF_ASSERT_IS_BASE_OF(SSFModule, ModuleClass);                                            \
    SFObjectErrors ModuleClass##Errors;                                                      \
    SSFModuleContext ModuleClass##Context;                                                   \
    ModuleClass##Context.Plugin = this;                                                      \
    SF_PTR(SSFModule)                                                                        \
    ModuleClass##Module = NewObject<ModuleClass>(ModuleClass##Context, ModuleClass##Errors); \
    RegisterModule(ModuleClass##Errors, ModuleClass##Module);                                \
    if (ModuleClass##Errors.IsValid())                                                       \
    {                                                                                        \
        SF_ERROR_DESC(ModuleClass##Errors,                                                   \
                      ESFError::Module_Register_Failed,                                      \
                      "Plugin RegisterModule Failed");                                       \
    }                                                                                        \
    else                                                                                     \
    {                                                                                        \
        SF_PTR(SSFModule)                                                                    \
        LoadedModule = GetModule<ModuleClass>();                                             \
        if (LoadedModule != nullptr)                                                         \
        {                                                                                    \
            SF_LOG_INFO("Register Module [" << #ModuleClass << "] Success");                 \
            LoadedModule->Init(ModuleClass##Errors);                                         \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            SF_LOG_DEBUG("Register Module [" << #ModuleClass << "] Skip By Config");         \
            ModuleClass##Module->Release(ModuleClass##Errors);                               \
        }                                                                                    \
    }

/**
 * 注销模块
 */
#define SF_UNREGISTER_MODULE(ModuleClass)                                           \
    SF_ASSERT_IS_BASE_OF(SSFModule, ModuleClass);                                   \
    SF_PTR(SSFModule)                                                               \
    ModuleClass##Module = GetModule<ModuleClass>();                                 \
    SFObjectErrors ModuleClass##Errors;                                             \
    if (ModuleClass##Module == nullptr)                                             \
    {                                                                               \
        SF_LOG_DEBUG("Unregister Module [" << #ModuleClass << "] Skip Not Loaded"); \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        UnregisterModule(ModuleClass##Errors, ModuleClass##Module);                 \
        if (ModuleClass##Errors.IsValid())                                          \
        {                                                                           \
            SF_ERROR_DESC(ModuleClass##Errors,                                      \
                          ESFError::Module_Unregister_Failed,                       \
                          "Plugin UnregisterModule Failed");                        \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            SF_LOG_INFO("Unregister Module [" << #ModuleClass << "] Success");      \
        }                                                                           \
        ModuleClass##Module->Release(ModuleClass##Errors);                          \
    }

/**
 * 插件导出
 */
#ifdef SF_DYNAMIC_PLUGIN
#define SF_PLUGIN_EXPORT(PluginClass)                                        \
    SF_DLL_EXPORT void DllStartPlugin(SF_PTR(SFPluginManager) PluginManager) \
    {                                                                        \
        SF_REGISTER_PLUGIN(PluginManager, PluginClass);                      \
    }                                                                        \
    SF_DLL_EXPORT void DllStopPlugin(SF_PTR(SFPluginManager) PluginManager)  \
    {                                                                        \
        SF_UNREGISTER_PLUGIN(PluginManager, PluginClass);                    \
    }
#else
#define SF_PLUGIN_EXPORT(PluginClass)
#endif // SF_DYNAMIC_PLUGIN

#pragma endregion Plugin Macro

#pragma region typedef

/**
 * 基础类型定义
 */
typedef bool SFBool;
typedef char SFByte;
typedef unsigned char SFUByte;
typedef short SFShort;
typedef unsigned short SFUShort;
typedef int SFInt;
typedef unsigned int SFUInt;
typedef long SFLong;
typedef unsigned long SFULong;
typedef long long SFLongLong;
typedef unsigned long long SFULongLong;

/**
 * 固定宽度整型定义（跨平台推荐）
 */
typedef std::int8_t SFInt8;
typedef std::uint8_t SFUInt8;
typedef std::int16_t SFInt16;
typedef std::uint16_t SFUInt16;
typedef std::int32_t SFInt32;
typedef std::uint32_t SFUInt32;
typedef std::int64_t SFInt64;
typedef std::uint64_t SFUInt64;

/**
 * 与平台位宽相关的整型定义
 */
typedef std::size_t SFSize;
typedef std::ptrdiff_t SFPtrDiff;

typedef float SFFloat;
typedef double SFDouble;
typedef std::string SFString;
typedef std::filesystem::path SFPath;

/**
 * 通用布尔常量
 */
#define TRUE 1
#define FALSE 0

#pragma endregion typedef

#pragma region Advanced typedef

/**
 * 对象唯一标识类型
 */
typedef SFULongLong SFObjectGUID;

/**
 * 无效 ObjectGUID
 */
#define SF_OBJECT_INVALID_GUID 0

/**
 * ObjectGUID 是否有效
 */
#define SF_OBJECT_GUID_VALID(ObjGUID) (ObjGUID != SF_OBJECT_INVALID_GUID)

/**
 * ObjectGUID 是否无效
 */
#define SF_OBJECT_GUID_INVALID(ObjGUID) (ObjGUID == SF_OBJECT_INVALID_GUID)

#pragma endregion Advanced typedef

#pragma region Container

/**
 * 映射容器别名
 */
template <typename Key, typename Value>
using SFMap = std::map<Key, Value>;

#pragma endregion Container

#pragma region Template Macro

/**
 * 模板必须继承自 TBase
 */
#define SF_TEMPLATE_CLASS(TBase, T) \
    template <typename T, typename std::enable_if<std::is_base_of<TBase, T>{}, int>::type = 0>

#pragma endregion Template Macro

SF_NAMESPACE_END

#endif // __SKYWALKER_FRAMEWORK_SFCORE_H__
