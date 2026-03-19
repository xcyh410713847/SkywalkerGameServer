# SkywalkerFramework/Plugin — 插件枢纽

## 概述
10个动态插件容器，C++20，全部编译为 Shared Library (.dll/.so)。

## 当前插件
| 插件 | 状态 | 模块 | 说明 |
|------|------|------|------|
| **Network** | 活跃 | NetworkServer, NetworkClient | 消息协议、Session会话、收发缓冲区、心跳、消息分发、SendTo/Broadcast API |
| **Auth** | 活跃 | Auth | 登录验证（PlayerId+Token），独立插件，未来可拆为登录服务器 |
| **Actor** | 活跃 | ActorFactory | 实体类体系（Actor→Creature→Player/NPC/Monster）、工厂创建/销毁 |
| **Level** | 活跃 | SceneManager | 场景容器、Actor 注册进出场景、EnterScene/Move 消息处理、全场景广播 |
| **Game** | 活跃 | GameFlow | 游戏流程控制（登录→创建Player→进场景）、NPC/Monster 初始化生成 |
| **AI** | 活跃 | AIRuntime | NPC 巡逻行为、Monster 追击 AI（范围检测 + 朝玩家移动） |
| **CommandLine** | 活跃 | OS | 非阻塞 stdin 轮询、status/quit/help 服务器命令 |
| **Profiler** | 活跃 | ServerStats | 每10秒打印统计：在线人数、场景数/Actor数、消息吞吐 |
| **DB** | 骨架 | — | 数据库（Demo 用内存存储，未实现持久化） |
| **Video** | 骨架 | — | 录像回放（Demo 不涉及） |

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
- **网络对象**: `Network/Object/` → SFNetworkBuffer, SFMessageCodec, SFMessageDispatcher, SFSession
- **实体对象**: `Actor/Object/` → SFActor, SFCreature, SFPlayer, SFNPC, SFMonster
- **场景对象**: `Level/Object/` → SFScene
- **共享接口**: `Include/SFNetworkInterface.h` (ISFNetworkServer), `Include/SFActorTypes.h` (ISFActorFactory, ISFSceneManager)

## 跨插件通信
插件禁止编译期互相依赖。交互通过以下方式：
1. **消息分发**: Network 的 MessageDispatcher 注册 Handler 回调（各插件在 Init() 阶段注册）
2. **共享接口**: `Include/` 中定义抽象接口（ISFNetworkServer, ISFActorFactory, ISFSceneManager）
3. **运行时查找**: `PluginManager->GetPlugin("...")->GetModule("...")` → `dynamic_cast<IInterface*>`
4. 参考实现: `SFModule_Auth.cpp::FindNetworkServer()`

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
