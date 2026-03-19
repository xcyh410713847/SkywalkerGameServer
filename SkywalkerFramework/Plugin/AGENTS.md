# SkywalkerFramework/Plugin — 插件枢纽

## 概述
9个动态插件容器，C++20，全部编译为 Shared Library (.dll/.so)。

## 当前插件
| 插件 | 状态 | 模块 | 说明 |
|------|------|------|------|
| **Network** | 活跃 | NetworkServer, NetworkClient | 网络通信、Socket连接管理 |
| **CommandLine** | 活跃 | OS | 命令行与运维管理 |
| **AI** | 骨架 | — | AI 系统 |
| **DB** | 骨架 | — | 数据库 |
| **Game** | 骨架 | — | 游戏逻辑 |
| **Level** | 骨架 | — | 场景管理 |
| **Actor** | 骨架 | — | 角色实体 |
| **Video** | 骨架 | — | 视频/渲染 |
| **Profiler** | 骨架 | — | 性能分析 |

## 目录约定
```
[PluginName]/
├── SFPlugin_[Name].h/.cpp     # 插件入口：SF_PLUGIN_EXPORT + Install/Uninstall
├── Module/
│   └── SFModule_[Name].h/.cpp # 业务逻辑实现
├── Object/                    # 插件内对象（如 Network 的 Socket）
├── CMakeLists.txt             # 通过 SkywalkerServer_Plugin 宏构建
└── README.md
```

## 关键位置
- **入口点**: `SFPlugin_[Name].cpp` → `Install()` 注册模块
- **业务实现**: `Module/SFModule_[Name].cpp` → 实际功能
- **加载配置**: `Bin/Server/ServerPlugin.skywalkerC` → 控制加载顺序
- **网络对象**: `Network/Object/` → ServerSocket/ClientSocket/NetworkSocket

## 开发规范
- **注册**: `SF_PLUGIN_EXPORT` 导出 + `Install()` 中 `SF_REGISTER_MODULE`
- **命名**: 插件类 `SFPlugin_`、模块类 `SFModule_`
- **热重载**: 避免插件内全局静态变量，`Uninstall()` 必须干净卸载
- **构建**: 每个插件 CMakeLists.txt 调用 `SkywalkerServer_Plugin(PlugName DirName)`

## 禁忌
- **手动 LoadLibrary**: 插件加载由框架配置驱动，禁止手动调用
- **插件间链接依赖**: 禁止编译期互相依赖，保持物理隔离
- **裸指针**: 用 `SSF_SHARED_PTR` 等框架智能指针宏

## 新增插件步骤
1. `Plugin/` 下创建目录 `[Name]/`
2. 实现 `SFPlugin_[Name].h/.cpp`，使用 `SF_PLUGIN_EXPORT`
3. `Install()` 中 `SF_REGISTER_MODULE` 注册模块
4. 创建 `CMakeLists.txt`，调用 `SkywalkerServer_Plugin`
5. 在 `Plugin/CMakeLists.txt` 中 `add_subdirectory([Name])`
6. 在 `Bin/Server/ServerPlugin.skywalkerC` 添加插件名
