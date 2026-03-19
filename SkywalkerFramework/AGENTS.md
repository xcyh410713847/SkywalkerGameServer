# SkywalkerFramework

## 概述
游戏服务器核心框架，包含静态库核心（SFCore）和动态插件系统。承上（可执行程序）启下（工具库）。

## 目录结构
```
SkywalkerFramework/
├── Core/              # 静态库 SFCore：对象模型、服务容器、插件管理器
├── Plugin/            # 10个动态插件（Network/CommandLine/AI/DB/Game/Level/Actor/Video/Profiler/Auth）
├── Include/           # 8个公共头文件（SFCore.h/SFErrors.h/SFEvent.h/SFFramework.h/SFILog.h/SFPlatform.h/SFNetworkInterface.h/SFActorTypes.h）
├── SkywalkerFramework.h/.cpp  # CSkywalkerFramework：Start/Tick/Stop 主循环
└── SkywalkerFramework.plantuml # 架构图源文件
```

## 关键位置
| 文件 | 作用 |
|------|------|
| `SkywalkerFramework.h` | SKYWALKER_FRAMEWORK_START 宏、ESFRunningState 枚举、CSkywalkerFramework 类 |
| `Include/SFCore.h` | 核心类型（SSFInt/SFString）、指针宏（SSF_SHARED_PTR）、注册宏（SF_REGISTER_PLUGIN/MODULE）、导出宏（SF_PLUGIN_EXPORT） |
| `Include/SFErrors.h` | ESFError 枚举、SF_ERROR_*/SF_ERROR_DESC_TRACE 宏 |
| `Include/SFFramework.h` | 框架公共 include（main 文件引用此头） |
| `Include/SFNetworkInterface.h` | ISFNetworkServer 接口、消息ID定义、登录类型 |
| `Include/SFActorTypes.h` | SFVector3、ESFActorType、ISFActorFactory、ISFSceneManager 接口 |
| `Core/Plugin/SFPluginManager.h` | 插件动态库扫描与注册入口 |

## 编译输出
- Core → `SFCore.lib` (静态库，所有插件链接)
- Plugin → `SFPlugin_*.dll/.so` (动态库，运行时加载)
- 宏 `SkywalkerServer_Plugin` 统一构建插件目标

## 子文档
- [Core/AGENTS.md](Core/AGENTS.md) — 核心基础库详情
- [Plugin/AGENTS.md](Plugin/AGENTS.md) — 插件系统详情
