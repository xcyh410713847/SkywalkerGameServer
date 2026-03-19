# SkywalkerGameServer 开发知识库

**Generated:** 2026-03-19 | **Commit:** aee3a33 | **Branch:** master

## 概述
C++20 游戏服务器框架，插件化架构，全部手写（无第三方库），学习用途。参考 NoahGameFrame + UnrealEngine 设计。

## 项目结构
```
SkywalkerGameServer/
├── SkywalkerFramework/         # 核心框架（静态库 + 动态插件）
│   ├── Core/                  # 静态库 SFCore：服务管理、插件加载、对象模型
│   ├── Plugin/                # 9个动态插件：Network/CommandLine/AI/DB/Game/Level/Actor/Video/Profiler
│   └── Include/               # 6个公共头文件
├── SkywalkerGameServer/       # 服务器可执行文件（main入口）
├── SkywalkerGameClient/       # 测试客户端
├── SkywalkerServerEditor/     # 服务器编辑器
├── SkywalkerTools/            # 独立工具库：9个子模块（Platform/Pool/Errors/Event/Path/Singleton/Timer/Script/Derived）
├── Bin/                       # 编译输出 + 运行时配置
└── build/                     # CMake 构建目录
```

## 快速导航
| 任务 | 位置 | 备注 |
|------|------|------|
| 框架核心 | `SkywalkerFramework/Core/AGENTS.md` | 服务管理、插件机制、禁忌 |
| 插件系统 | `SkywalkerFramework/Plugin/AGENTS.md` | 动态加载、注册机制 |
| 工具库 | `SkywalkerTools/AGENTS.md` | 平台抽象、内存池、错误处理 |
| 入口点 | `SkywalkerGameServer/SkywalkerGameServer.cpp` | main → 配置路径 → SKYWALKER_FRAMEWORK_START |
| 插件配置 | `Bin/Server/ServerPlugin.skywalkerC` | 插件加载列表与顺序 |
| 网络模块 | `Plugin/Network/Module/SFModule_NetworkServer.cpp` | 网络服务入口（accept循环） |
| 错误宏定义 | `Include/SFErrors.h` | SF_ERROR_* 系列宏 |
| 核心类型宏 | `Include/SFCore.h` | 指针宏、注册宏、命名空间宏 |

## 启动流程
```
main() → 解析路径 → 环境变量注入(SF_ENV_*) → SKYWALKER_FRAMEWORK_START
  → CSkywalkerFramework::Start()
    → ServiceManager 初始化 (Timer/Event)
    → SFPluginManager::Init()
      → LoadPluginConfig() 读 ServerPlugin.skywalkerC
      → LoadPlugin() 遍历 DynamicLib 加载 SFPlugin_*.dll
      → StartPlugin() 调用 DllStartPlugin → Plugin::Install() → SF_REGISTER_MODULE
    → 主循环 Tick()
  → Stop() → 逆序卸载
```

## 三层架构
```
┌─────────────────────────────────────┐
│  Plugin (动态库 .dll/.so)            │  ← Network, CommandLine, AI, DB 等
├─────────────────────────────────────┤
│  SkywalkerFramework (静态库)         │  ← 生命周期、插件管理、服务容器
├─────────────────────────────────────┤
│  SkywalkerTools (独立工具库)          │  ← Platform, Pool, Errors, Event 等
└─────────────────────────────────────┘
```

## 编译命令
```bash
cd build/SkywalkerGameServer
cmake ..
cmake --build . --config Debug
cmake --build . --target SkywalkerGameServer --config Debug
```

## 命名约定
| 类型 | 约定 | 示例 |
|------|------|------|
| 类 | C前缀 | `CSkywalkerFramework`, `CSkywalkerScriptNode` |
| 插件 | SFPlugin_前缀 | `SFPlugin_Network` |
| 模块 | SFModule_前缀 | `SFModule_NetworkServer` |
| 接口 | I前缀 | `ISSFPlugin` |
| 枚举 | E前缀 | `ESFError`, `ESFRunningState` |
| 文件 | PascalCase | `SkywalkerFramework.h` |
| 命名空间 | `SF_NAMESPACE_BEGIN/END`（框架）、`Skywalker::[ToolName]`（工具库） |
| 头文件守卫 | `#ifndef __SKYWALKER_FILENAME_H__`（非 `#pragma once`） |

## 文件头格式
```cpp
/*************************************************************************
**文件: SkywalkerFramework\Plugin\Test\SFPlugin_Test.h
**作者: shyfan
**日期: 2023/08/21 19:54:39
**功能: 测试插件
*************************************************************************/
```

## 禁忌（全项目级）
- **SF_ERROR_TRACE**: 有已知Bug，**必须**用 `SF_ERROR_DESC_TRACE` 替代
- **裸 delete**: 禁止直接 `delete`，必须使用 `SSF_PTR` 系列宏
- **直接系统头**: 禁止 `#include <windows.h>`，用 `SkywalkerPlatform` 抽象
- **插件间链接依赖**: 禁止插件编译期互相依赖，保持物理隔离
- **头文件全局变量**: 禁止在头文件定义 static 全局变量
- **循环包含**: `Include/SFCore.h` 禁止包含具体实现类
- **手动 LoadLibrary**: 插件加载完全由框架配置驱动

## 已知问题
1. **宏中单行注释** → 导致宏展开错误，改用多行注释 `/* */`
2. **中文注释后枚举** → 编码问题导致"未声明"，检查文件编码
3. **_CrtIsValidHeapPointer** → 错误释放智能指针导致堆损坏
4. **工具库非线程安全** → Pool/Event/Errors 需调用侧同步
5. **CSkywalkerPool::Get()** → 不检查空，调用前必须 `Size() > 0`
6. **CSkywalkerErrors::GetErrors()** → 返回完整拷贝，禁止热路径调用

## 未完成项 (TODO)
- `Core/Object/SFArchive.h` — 对象指针反序列化：需通过 ObjectManager 查找/创建
- `Core/Object/SFJsonArchive.h` — Base64 解码未实现
- `SkywalkerScript.h` — 使用 raw `new[]`，应迁移到 RAII 容器
