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
/**
 * SF_UNIQUE_PTR(T)
 * 说明：独占所有权智能指针，同一时刻只能有一个拥有者，不可拷贝、可移动。
 * 场景：对象生命周期明确、仅由单一对象管理资源（如模块内部对象、句柄封装）。
 * 用法：
 *   SF_UNIQUE_PTR(MyType) Obj = SF_MAKE_UNIQUE_PTR(MyType);
 *   Obj->DoSomething();
 * 注意：离开作用域自动释放；如需转移所有权，使用 std::move。
 */
#define SF_UNIQUE_PTR(T) std::unique_ptr<T>
#define SF_MAKE_UNIQUE_PTR(T) std::make_unique<T>();
#define SF_UNIQUE_PTR_CAST(T, Obj) std::unique_ptr<T>(dynamic_cast<T *>(Obj))

/**
 * SF_CONST_UNIQUE_PTR(T)
 * 说明：指向 const 对象的独占指针，可管理生命周期但不能修改对象内容。
 * 场景：只读资源所有权管理（例如加载后不可修改的配置对象）。
 * 用法：
 *   SF_CONST_UNIQUE_PTR(Config) Cfg = SF_MAKE_CONST_UNIQUE_PTR(Config);
 *   // Cfg->SetXxx(); // 编译期禁止
 */
#define SF_CONST_UNIQUE_PTR(T) std::unique_ptr<const T>
#define SF_MAKE_CONST_UNIQUE_PTR(T) std::make_unique<const T>();

/**
 * SF_SHARED_PTR(T)
 * 说明：共享所有权智能指针，内部引用计数，最后一个 shared_ptr 释放时对象销毁。
 * 场景：对象需要在多个模块/系统之间共享（如事件对象、上下文对象）。
 * 用法：
 *   SF_SHARED_PTR(MyType) Obj = SF_MAKE_SHARED_PTR(MyType, Arg1, Arg2);
 *   UseInA(Obj);
 *   UseInB(Obj);
 * 注意：避免互相持有 shared_ptr 形成循环引用；循环关系应配合 weak_ptr。
 */
#define SF_SHARED_PTR(T) std::shared_ptr<T>
#define SF_MAKE_SHARED_PTR(T, ...) std::make_shared<T>(__VA_ARGS__);

/**
 * SF_CONST_SHARED_PTR(T)
 * 说明：指向 const 对象的共享指针，可共享生命周期但不能修改对象内容。
 * 场景：跨模块共享只读数据（如只读配置、静态描述信息）。
 * 用法：
 *   SF_CONST_SHARED_PTR(Config) Cfg = SF_MAKE_CONST_SHARED_PTR(Config);
 *   // 多处读取 Cfg，不可写。
 */
#define SF_CONST_SHARED_PTR(T) std::shared_ptr<const T>
#define SF_MAKE_CONST_SHARED_PTR(T) std::make_shared<const T>();

/**
 * SF_WEAK_PTR(T)
 * 说明：弱引用指针，不拥有对象生命周期，不增加引用计数。
 * 场景：观察者关系、缓存索引、父子节点反向引用，避免 shared_ptr 循环引用。
 * 用法：
 *   SF_WEAK_PTR(MyType) Weak = SharedObj;
 *   if (auto Locked = Weak.lock())
 *   {
 *       Locked->DoSomething();
 *   }
 *   else
 *   {
 *       // 对象已释放
 *   }
 * 注意：使用前必须 lock() 判空，不可直接解引用 weak_ptr。
 */
#define SF_WEAK_PTR(T) std::weak_ptr<T>
#define SF_MAKE_WEAK_PTR(T) std::make_weak_ptr<T>();

/**
 * SF_CONST_WEAK_PTR(T)
 * 说明：指向 const 对象的弱引用，不拥有生命周期，且锁定后只能只读访问。
 * 场景：只读观察者引用，既避免循环引用又限制写操作。
 * 用法：
 *   SF_CONST_WEAK_PTR(Config) WeakCfg = SharedConstCfg;
 *   if (auto LockedCfg = WeakCfg.lock())
 *   {
 *       // 只读访问 LockedCfg
 *   }
 */
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
 * 说明：加载失败时输出警告并跳过，不终止后续插件加载。
 */
#define SF_REGISTER_LIBRARY(LibraryName)                                                              \
    {                                                                                                  \
        SFObjectErrors LibraryName##Errors;                                                            \
        if (DynamicLibMap.find(LibraryName) != DynamicLibMap.end())                                    \
        {                                                                                              \
            SF_LOG_WARNING("Register Library [" << LibraryName << "] Skip: already loaded");              \
            continue;                                                                                  \
        }                                                                                              \
        SFDynamicLib *DynamicLib = new SFDynamicLib(LibraryName);                                      \
        if (DynamicLib == nullptr)                                                                     \
        {                                                                                              \
            SF_LOG_WARNING("Register Library [" << LibraryName << "] Skip: alloc failed");                \
            continue;                                                                                  \
        }                                                                                              \
        if (!DynamicLib->Load())                                                                       \
        {                                                                                              \
            SF_LOG_WARNING("Register Library [" << LibraryName << "] Skip: DLL load failed");             \
            delete DynamicLib;                                                                         \
            continue;                                                                                  \
        }                                                                                              \
        DynamicLibMap.insert(std::make_pair(LibraryName, DynamicLib));                                 \
        SF_LOG_INFO("Register Library [" << LibraryName << "] Success");                               \
    }

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

#pragma region Env Macro

/**
 * 程序工作目录路径
 */
#define SF_ENV_PROGRAM_WORK_DIR "SKYWALKER_PROGRAM_WORK_DIR"

/**
 * 程序配置路径
 */
#define SF_ENV_PROGRAM_CONFIG_DIR "SKYWALKER_PROGRAM_CONFIG_DIR"

/**
 * 插件配置路径
 */
#define SF_ENV_PLUGIN_CONFIG_DIR "SKYWALKER_PLUGIN_CONFIG_DIR"

#pragma endregion Env Macro

SF_NAMESPACE_END

#endif // __SKYWALKER_FRAMEWORK_SFCORE_H__
