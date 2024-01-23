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

#include "SkywalkerDerived/SkywalkerDerived.h"

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

#pragma endregion Function Macro

#pragma region Assert Macro

/**
 * 断言
 */
#define SSF_ASSERT(Condition) assert((Condition))

/**
 * 断言 是否继承自 TBase
 */
#define SSF_ASSERT_IS_BASE_OF(TBase, T) \
    static_assert(std::is_base_of<TBase, T>{}, "T must be derived from TBase")

#pragma endregion Assert Macro

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

#pragma region Plugin Macro

/**
 * 注册库
 */
#define SSF_REGISTER_LIBRARY(LibraryName)                                                     \
    SSFObjectErrors LibraryName##Errors;                                                      \
    if (DynamicLibMap.find(LibraryName) != DynamicLibMap.end())                               \
    {                                                                                         \
        SSF_ERROR_TRACE(LibraryName##Errors, SkywalkerSFError_Plugin_Load_Repeated);          \
        return;                                                                               \
    }                                                                                         \
    SSFDynamicLib *DynamicLib = new SSFDynamicLib(LibraryName);                               \
    if (DynamicLib == nullptr)                                                                \
    {                                                                                         \
        SSF_ERROR_TRACE(LibraryName##Errors, SkywalkerSFError_Plugin_Load_DynamicLibNullptr); \
        return;                                                                               \
    }                                                                                         \
    DynamicLibMap.insert(std::make_pair(LibraryName, DynamicLib));                            \
    if (!DynamicLib->Load())                                                                  \
    {                                                                                         \
        SSF_ERROR_TRACE(LibraryName##Errors, SkywalkerSFError_Plugin_Load_Failed);            \
        return;                                                                               \
    }                                                                                         \
    SSF_LOG_INFO("Register Library [" << LibraryName << "] Success");

/**
 * 注册插件
 */
#define SSF_REGISTER_PLUGIN(PluginManager, PluginClass)                                                              \
    SSF_ASSERT_IS_BASE_OF(SSFPlugin, PluginClass);                                                                   \
    SSFObjectErrors PluginClass##Errors;                                                                             \
    SSFPluginContext PluginClass##Context;                                                                           \
    PluginClass##Context.SSFramework = PluginManager->GetFramework();                                                \
    PluginClass##Context.PluginManager = PluginManager;                                                              \
    SSF_PTR(SSFPlugin)                                                                                               \
    Plugin = new PluginClass(PluginClass##Context, PluginClass##Errors);                                             \
    PluginManager->RegisterPlugin(PluginClass##Errors, Plugin);                                                      \
    if (PluginClass##Errors.IsValid())                                                                               \
    {                                                                                                                \
        auto FirstError = PluginClass##Errors.GetFirstError();                                                       \
        SSF_LOG_INFO("Register Plugin [" << Plugin->GetObjectClassName() << "] Failed " << FirstError.GetContent()); \
    }                                                                                                                \
    else                                                                                                             \
    {                                                                                                                \
        SSF_LOG_INFO("Register Plugin [" << Plugin->GetObjectClassName() << "] Success");                            \
    }

/**
 * 注销插件
 */
#define SSF_UNREGISTER_PLUGIN(PluginManager, PluginClass)                                                              \
    SSF_ASSERT_IS_BASE_OF(SSFPlugin, PluginClass);                                                                     \
    SSFObjectErrors PluginClass##Errors;                                                                               \
    SSFString PluginClassName;                                                                                         \
    SSF_CLASS_NAME(PluginClass, PluginClassName);                                                                      \
    SSF_PTR(SSFPlugin)                                                                                                 \
    Plugin = PluginManager->GetPlugin(PluginClassName);                                                                \
    PluginManager->UnregisterPlugin(PluginClass##Errors, Plugin);                                                      \
    if (PluginClass##Errors.IsValid())                                                                                 \
    {                                                                                                                  \
        auto FirstError = PluginClass##Errors.GetFirstError();                                                         \
        SSF_LOG_INFO("Unregister Plugin [" << Plugin->GetObjectClassName() << "] Failed " << FirstError.GetContent()); \
    }                                                                                                                  \
    else                                                                                                               \
    {                                                                                                                  \
        SSF_LOG_INFO("Unregister Plugin [" << Plugin->GetObjectClassName() << "] Success");                            \
    }                                                                                                                  \
    Plugin->Release(PluginClass##Errors);

/**
 * 注册模块
 */
#define SSF_REGISTER_MODULE(ModuleClass)                                              \
    SSF_ASSERT_IS_BASE_OF(SSFModule, ModuleClass);                                    \
    SSFObjectErrors ModuleClass##Errors;                                              \
    SSFModuleContext ModuleClass##Context;                                            \
    ModuleClass##Context.SSFramework = GetFramework();                                \
    ModuleClass##Context.Plugin = this;                                               \
    SSF_PTR(SSFModule)                                                                \
    ModuleClass##Module = new ModuleClass(ModuleClass##Context, ModuleClass##Errors); \
    RegisterModule(ModuleClass##Errors, ModuleClass##Module);                         \
    if (ModuleClass##Errors.IsValid())                                                \
    {                                                                                 \
        SSF_ERROR_DESC(ModuleClass##Errors,                                           \
                       SkywalkerSFError_Module_Register_Failed,                       \
                       "Plugin RegisterModule Failed");                               \
    }                                                                                 \
    SSF_LOG_INFO("Register Module [" << #ModuleClass << "] Success");                 \
    ModuleClass##Module->Init(ModuleClass##Errors);

/**
 * 注销模块
 */
#define SSF_UNREGISTER_MODULE(ModuleClass)                                  \
    SSF_ASSERT_IS_BASE_OF(SSFModule, ModuleClass);                          \
    SSF_PTR(SSFModule)                                                      \
    ModuleClass##Module = GetModule<ModuleClass>();                         \
    SSFObjectErrors ModuleClass##Errors;                                    \
    UnregisterModule(ModuleClass##Errors, ModuleClass##Module);             \
    if (ModuleClass##Errors.IsValid())                                      \
    {                                                                       \
        SSF_ERROR_DESC(ModuleClass##Errors,                                 \
                       SkywalkerSFError_Module_Unregister_Failed,           \
                       "Plugin UnregisterModule Failed");                   \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        SSF_LOG_INFO("Unregister Module [" << #ModuleClass << "] Success"); \
    }                                                                       \
    ModuleClass##Module->Release(ModuleClass##Errors);

/**
 * 插件导出
 */
#ifdef SKYWALKER_SF_DYNAMIC_PLUGIN
#define SSF_PLUGIN_EXPORT(PluginClass)                                          \
    SSF_DLL_EXPORT void DllStartPlugin(SSF_PTR(SSFPluginManager) PluginManager) \
    {                                                                           \
        SSF_REGISTER_PLUGIN(PluginManager, PluginClass);                        \
    }                                                                           \
    SSF_DLL_EXPORT void DllStopPlugin(SSF_PTR(SSFPluginManager) PluginManager)  \
    {                                                                           \
        SSF_UNREGISTER_PLUGIN(PluginManager, PluginClass);                      \
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

#pragma region Advanced typedef

/**
 * ObjectGUID
 */
typedef SSFULongLong SSFObjectGUID;
#define SSF_OBJECT_INVALID_GUID 0
#define SSF_OBJECT_GUID_VALID(ObjGUID) (ObjGUID != SSF_OBJECT_INVALID_GUID)
#define SSF_OBJECT_GUID_INVALID(ObjGUID) (ObjGUID == SSF_OBJECT_INVALID_GUID)

#pragma endregion Advanced typedef

#pragma region Container

template <typename Key, typename Value>
using SSFMap = std::map<Key, Value>;

#pragma endregion Container

#pragma region Template Macro

/**
 * 模板必须继承自 TBase
 */
#define SSF_TEMPLATE_CLASS(TBase, T) \
    template <typename T, typename std::enable_if<std::is_base_of<TBase, T>{}, int>::type = 0>

#pragma endregion Template Macro

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_CORE_H__
