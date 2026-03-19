# SkywalkerFramework/Core — 核心基础库

## 概述
框架核心基础设施：对象模型、动态库加载、插件管理器、服务容器、模块生命周期。编译为静态库 `SFCore`。

## 目录结构
- **DynamicLib/**: 跨平台动态库封装（`SFDynamicLib` → LoadLibrary/dlopen 抽象）
- **Object/**: 基础对象模型（`SFObject`）、对象管理器（`SFObjectManager`）、序列化（`SFArchive`/`SFJsonArchive`/`SFMemoryArchive`）
- **Plugin/**: `SFPluginManager`（扫描配置→加载DLL→调用DllStartPlugin）、`SFPlugin` 基类
- **Service/**: 
  - `SFServiceManager` — 全局服务容器
  - `FrameworkService/` — Timer + Event 核心服务
  - `LevelService/` — 场景/世界状态管理
- **Module/**: `SFModule` 基类，定义模块生命周期（Init→Awake→Start→Tick→Stop→Sleep→Destroy）

## 关键文件
| 文件 | 作用 |
|------|------|
| `Plugin/SFPluginManager.cpp` | LoadPluginConfig → LoadPlugin → StartPlugin → StopPlugin 完整流程 |
| `Plugin/SFPlugin.cpp` | Install/Uninstall 分发到子模块，管理 ModuleMap |
| `DynamicLib/SFDynamicLib.cpp` | Load/Unload/GetSymbol 平台实现 |
| `Object/SFObjectManager.h` | AddObject/RemoveObject/FindObject 模板 |
| `Service/SFServiceManager.h` | 所有核心服务的全局访问点 |

## 插件加载详细流程
```
SFPluginManager::Init()
  → LoadPluginConfig(): 用 SkywalkerScript 解析 ServerPlugin.skywalkerC
  → LoadPlugin(): 遍历 PluginNameMap，SFDynamicLib::Load() 各 .dll/.so
  → StartPlugin(): GetSymbol("DllStartPlugin") → 调用 → Plugin::Install() → SF_REGISTER_MODULE
  → 依次调用 Plugin::Init/Awake/Start
  → Tick 阶段：Plugin::Tick → Module::Tick
  → StopPlugin(): GetSymbol("DllStopPlugin") → Plugin::Uninstall
```

## 开发规范
- 命名空间：代码必须包裹在 `SF_NAMESPACE_BEGIN/END` 宏内
- 宏注册：`SF_REGISTER_PLUGIN`（插件）、`SF_REGISTER_MODULE`（模块）
- 对象创建：通过 `SFObjectManager`，GUID 由框架分配

## 禁忌
- **SF_ERROR_TRACE**: 有Bug，用 `SF_ERROR_DESC_TRACE`
- **循环包含**: `Include/SFCore.h` 禁止包含具体实现类，用前置声明
- **裸 delete**: 必须用 `SSF_PTR` 系列宏
- **头文件静态全局变量**: 禁止，通过 `SFServiceManager` 管理

## TODO
- `Object/SFArchive.h`: 对象指针反序列化需接入 ObjectManager
- `Object/SFJsonArchive.h`: Base64 解码未实现
