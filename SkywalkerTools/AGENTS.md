# SkywalkerTools — 工具库

## 概述
跨项目复用的 C++ 基础工具库，9个独立子模块，仅依赖标准库和少量平台头。

## 子模块
| 模块 | 作用 | 关键类型/宏 |
|------|------|-------------|
| **SkywalkerPlatform** | 平台识别、动态库加载、系统调用封装 | `SkywalkerSetEnv`, `SkywalkerInetPton` |
| **SkywalkerPool** | 固定容量对象池 | `CSkywalkerPool<T>` |
| **SkywalkerErrors** | 错误收集+文件行号追踪 | `CSkywalkerErrors<T>`, `SKYWALKER_ERRORS_WRAP_TRACE` |
| **SkywalkerEvent** | Before/During/After 三阶段事件总线 | `ESkywalkerEventOrder` |
| **SkywalkerPath** | 绝对路径+配置路径解析 | `SkywalkerAbsolutePath`, `SkywalkerResolveConfigPath` |
| **SkywalkerSingleton** | 单例声明/实现宏 | `SKYWALKER_SINGLETON_DECLARE/IMPLEMENT` |
| **SkywalkerTimer** | 高精度帧计时器 | `Reset/Tick/GetDeltaTime/GetTotalTime` |
| **SkywalkerScript** | 层级 key=value 配置解析器 | `CSkywalkerScriptParse`, `CSkywalkerScriptNode` |
| **SkywalkerDerived** | 编译期继承关系检测 | `SKYWALKER_IS_DERIVED(Derived, Base)` |

## 关键位置
- **跨平台适配**: `SkywalkerPlatform/SkywalkerPlatform.h`（平台子头：Win/Linux/Mac）
- **错误聚合**: `SkywalkerErrors/SkywalkerErrors.h`
- **脚本解析**: `SkywalkerScript/SkywalkerScript.h`（插件配置加载依赖此模块）
- **路径工具**: `SkywalkerPath/SkywalkerPath.h`（所有 main 入口使用）

## 开发规范
- **命名空间**: `Skywalker::[ToolName]`，各模块有自己的 `NAMESPACE_BEGIN/END` 宏
- **头文件守卫**: `#ifndef __SKYWALKER_FILENAME_H__`
- **错误包装**: 业务推荐 `SKYWALKER_ERRORS_WRAP_TRACE` 自动捕获上下文

## 禁忌
- **热路径调用 GetErrors()**: 返回 vector 完整拷贝，仅用于最终结果展示
- **空池 Get()**: `CSkywalkerPool::Get()` 不自动扩容不检查空，调用前 `Size() > 0`
- **多线程盲用**: Pool/Event/Errors **非线程安全**，必须调用侧同步
- **直接系统调用**: 禁止 `#include <windows.h>` 等，用 `SkywalkerPlatform` 抽象

## 已知技术债
- `SkywalkerScript.h`: 使用 raw `new[]`/手动 `delete`，非 RAII，有内存安全风险
