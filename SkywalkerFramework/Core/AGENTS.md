# SkywalkerFramework/Core - 核心基础库

## 概述
框架核心基础设施，包含插件加载、单例服务管理、事件/定时器系统及对象生命周期。

## 目录结构
- **DynamicLib**: 封装跨平台动态库（DLL/SO）加载逻辑。
- **Object**: 基础对象模型与管理器。
- **Plugin**: 核心 `SFPluginManager`，负责扫描配置并动态加载功能插件。
- **Service**: 
  - `SFServiceManager`: 全局服务容器。
  - `FrameworkService`: 核心基础服务（Event, Timer）。
  - `LevelService`: 场景/世界状态管理服务。
- **Module**: 核心模块基类与生命周期定义。

## 关键位置
- **SFCore.h**: 核心类型定义（`SSFInt`, `SFString`）与指针宏（`SSF_SHARED_PTR`）。
- **SFErrors.h**: 框架级错误码 `ESFError` 与错误处理宏定义。
- **SFPluginManager.h**: 插件动态库扫描与注册入口。
- **SFServiceManager.h**: 所有核心服务的全局访问点。

## 开发规范
- **静态库编译**: 本目录生成 `SFCore` 静态库，由 `SkywalkerFramework` 链接。
- **命名空间**: 代码必须包裹在 `SF_NAMESPACE_BEGIN/END` 宏内。
- **宏注册**: 使用 `SF_REGISTER_PLUGIN` 和 `SF_REGISTER_MODULE` 进行对象注册。

## 禁忌
- **错误宏陷阱**: ⚠️ `SF_ERROR_TRACE` 宏存在已知 Bug，应优先使用 `SF_ERROR_DESC_TRACE`。
- **循环包含**: 严禁在 `Include/SFCore.h` 中包含具体实现类，应使用前置声明。
- **内存管理**: 严禁直接使用 `delete`，必须使用 `SSF_PTR` 系列宏。
- **全局变量**: 禁止在头文件中定义静态全局变量，应通过 `SFServiceManager` 管理。
