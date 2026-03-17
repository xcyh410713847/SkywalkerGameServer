# AGENTS.md - SkywalkerTools 工具库

## 概述
提供跨项目复用的 C++ 基础工具库，包含平台抽象、对象池、错误追踪及设计模式。

## 目录结构
- `SkywalkerPlatform/`: 平台识别、动态库加载、系统调用封装（Win/Linux/Mac）。
- `SkywalkerPool/`: 固定容量的对象池，管理对象生命周期。
- `SkywalkerErrors/`: 基于模板的错误容器，支持文件、行号与函数追踪。
- `SkywalkerEvent/`: 支持 Before/During/After 三阶段执行的轻量级事件总线。
- `SkywalkerPath/`: 统一的绝对路径与配置路径解析。
- `SkywalkerSingleton/`: 快速声明与实现单例模式。
- `SkywalkerTimer/`: 高精度帧计时器。
- `SkywalkerScript/`: 配置文件解析器。

## 关键位置
- **跨平台适配**: 参考 `SkywalkerPlatform/SkywalkerPlatform.h` 提供的 OS 无关接口。
- **错误聚合**: 使用 `SkywalkerErrors/SkywalkerErrors.h` 中的 `CSkywalkerErrors<T>`。
- **高精度计时**: 查阅 `SkywalkerTimer/SkywalkerTimer.h` 获取帧 delta 时间。
- **脚本解析**: 查阅 `SkywalkerScript/SkywalkerScript.h` 获取配置解析能力。

## 开发规范
- **命名空间**: 严格遵守 `Skywalker::[ToolName]` 嵌套规则。
- **头文件保护**: 统一格式为 `#ifndef __SKYWALKER_FILENAME_H__`。
- **错误包装**: 业务推荐使用 `SKYWALKER_ERRORS_WRAP_TRACE` 自动捕获上下文。

## 禁忌
- **热路径拷贝**: `CSkywalkerErrors::GetErrors()` 返回 vector 完整拷贝。禁止在高性能逻辑中频繁调用，仅用于最终结果展示。
- **空池获取**: `CSkywalkerPool::Get()` 不会自动扩容或检查空指针。调用前需校验 `Size() > 0`。
- **多线程盲用**: 大部分工具（Pool, Event, Errors）**非线程安全**。必须在调用侧同步或仅在单线程环境下使用。
- **直接系统调用**: 严禁直接包含 `windows.h` 等系统头文件。应优先使用 `SkywalkerPlatform` 抽象层。
