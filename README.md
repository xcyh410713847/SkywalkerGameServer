# SkywalkerGameServer

一套完整的游戏服务器框架，除标准库外全部手写，旨在学习服务器开发。

**当前状态**: 可运行的最小游戏服务器 Demo — 登录→进入场景→移动同步→NPC巡逻→Monster追击→多客户端互见→Profiler监控→服务器/客户端命令行。

## 快速开始

### 编译

```bash
cd build
cmake ..
cmake --build . --config Debug
```

### 运行 Demo

**1. 启动服务器**
```bash
cd Bin/Debug
SkywalkerGameServer.exe
```
服务器监听 `127.0.0.1:9527`，自动创建默认场景（SceneId=1），生成 2 个 NPC 和 2 个 Monster。

**2. 启动客户端（玩家1）**
```bash
cd Bin/Debug
SkywalkerGameClient.exe
```
客户端自动连接 → 登录（PlayerId=10001, Token=skywalker_demo_token）→ 进入场景 → 开始自动移动。

**3. 启动第二个客户端（玩家2）**
```bash
cd Bin/Debug
set SF_ENV_ClientConfigPath=../../Client/ClientConfig2.skywalkerC
SkywalkerGameClient.exe
```
使用 PlayerId=10002 登录，进场后可看到玩家1 + NPC + Monster。

### 服务器命令

在服务器控制台输入：
| 命令 | 功能 |
|------|------|
| `status` | 显示在线人数、场景数、Actor数、消息吞吐 |
| `quit` | 优雅关闭服务器 |
| `help` | 显示帮助信息 |

### 客户端命令

在客户端控制台输入：
| 命令 | 功能 |
|------|------|
| `move x y z` | 移动到指定坐标（如 `move 10 0 5`） |
| `manual` | 切换到手动模式（停止自动移动） |
| `auto` | 切换到自动模式（恢复自动移动） |
| `quit` | 断开连接并退出 |
| `help` | 显示帮助信息 |

### Profiler 监控

服务器每 10 秒自动打印统计信息：在线连接数、已认证连接数、场景数/Actor数、消息收发吞吐量。

---

## 架构设计

### 三层架构

```
┌─────────────────────────────────────┐
│  Plugin (动态库 .dll/.so)            │  ← 10个插件：Network/Auth/Actor/Level/Game/AI/CommandLine/Profiler/DB/Video
├─────────────────────────────────────┤
│  SkywalkerFramework (静态库)         │  ← 框架核心：生命周期、插件管理、服务容器
├─────────────────────────────────────┤
│  SkywalkerTools (独立工具库)          │  ← 平台抽象、对象池、错误追踪、事件总线、计时器等
└─────────────────────────────────────┘
```

### 插件全景

| 插件 | 模块 | 职责 |
|------|------|------|
| **Network** | NetworkServer, NetworkClient | 消息协议、Session会话、心跳、消息分发、SendTo/Broadcast |
| **Auth** | Auth | 登录验证（PlayerId+Token），独立插件 |
| **Actor** | ActorFactory | 实体类体系 Actor→Creature→Player/NPC/Monster，工厂模式 |
| **Level** | SceneManager | 场景容器、Actor进出场景、EnterScene/Move处理、全场景广播 |
| **Game** | GameFlow | 游戏流程（登录→创建Player→进场景）、NPC/Monster初始化 |
| **AI** | AIRuntime | NPC巡逻行为、Monster追击AI |
| **CommandLine** | OS | 服务器命令行（status/quit/help） |
| **Profiler** | ServerStats | 定时统计在线人数、消息吞吐 |
| **DB** | — | 骨架（Demo 用内存存储） |
| **Video** | — | 骨架（录像回放，未实现） |

### 实体继承体系

```
SSFObject (框架基类)
  └─ SFActor (场景实体: Position, Rotation, ActorType)
       └─ SFCreature (生物: HP, MoveSpeed)
            ├─ SFPlayer (玩家: PlayerId, SessionId)
            ├─ SFNPC (NPC: NPCId, PatrolPath)
            └─ SFMonster (怪物: MonsterId, AggroRange)
```

### 消息协议

```
┌──────────┬──────────┬─────────────────┐
│ Length    │ MsgID    │ Payload         │
│ 4 bytes  │ 2 bytes  │ N bytes         │
│ uint32   │ uint16   │ 业务数据        │
│ (网络序) │ (网络序) │                 │
└──────────┴──────────┴─────────────────┘
```

### 交互流程

```
Client ──LoginReq──→ [Network] ──dispatch──→ [Auth] 验证
                                                │
                                          LoginSuccess
                                                │
                                                ▼
                                            [Game] 创建 Player
                                                │
                                                └──→ [Level] 加入场景
                                                       ├── 广播 ActorEnterScene
                                                       └── 回复 EnterSceneResp

Client ──MoveReq──→ [Network] ──dispatch──→ [Level] 更新位置 → 广播 MoveBroadcast
```

---

## 项目结构

```
SkywalkerGameServer/
├── Bin/                        # 编译输出 + 运行时配置
│   ├── Server/                # 服务器配置
│   └── Client/                # 客户端配置
├── SkywalkerFramework/         # 核心框架
│   ├── Core/                  # 静态库 SFCore
│   ├── Plugin/                # 10个动态插件
│   └── Include/               # 公共头文件（接口定义、类型宏）
├── SkywalkerGameServer/       # 服务器可执行文件
├── SkywalkerGameClient/       # 测试客户端
├── SkywalkerServerEditor/     # 服务器编辑器（骨架）
├── SkywalkerTools/            # 独立工具库
└── PLAN.md                    # 开发计划文档
```

## 配置文件

| 文件 | 说明 |
|------|------|
| `Bin/Server/ServerPlugin.skywalkerC` | 服务器插件加载列表 |
| `Bin/Server/ServerConfig.skywalkerC` | 服务器配置（IP=127.0.0.1, Port=9527） |
| `Bin/Client/ClientPlugin.skywalkerC` | 客户端插件加载列表 |
| `Bin/Client/ClientConfig.skywalkerC` | 客户端配置（PlayerId=10001） |
| `Bin/Client/ClientConfig2.skywalkerC` | 第二客户端配置（PlayerId=10002） |

## 开发规范

- **全部手写**: 不引入第三方库
- **插件化**: 功能以插件形式实现，插件间禁止编译期依赖
- **命名**: 插件 `SFPlugin_`、模块 `SFModule_`、类 `C` 前缀、枚举 `E` 前缀
- **头文件守卫**: `#ifndef __SKYWALKER_*__`（非 `#pragma once`）
- **错误追踪**: 使用 `SF_ERROR_DESC_TRACE`（禁止 `SF_ERROR_TRACE`）
- **跨平台**: 通过 `SkywalkerPlatform` 抽象系统调用

## 文档索引

### 开发知识库
| 文件 | 说明 |
|------|------|
| [AGENTS.md](AGENTS.md) | 项目级开发知识库（架构、约定、禁忌） |
| [PLAN.md](PLAN.md) | Demo 开发计划（分阶段，含验收标准） |

### 核心框架
| 文件 | 说明 |
|------|------|
| [SkywalkerFramework/README.md](SkywalkerFramework/README.md) | 核心框架目录 |
| [SkywalkerTools/README.md](SkywalkerTools/README.md) | 工具库 |

### 工具库
| 文件 | 说明 |
|------|------|
| [SkywalkerPlatform](SkywalkerTools/SkywalkerPlatform/README.md) | 平台抽象 |
| [SkywalkerPool](SkywalkerTools/SkywalkerPool/README.md) | 对象池 |
| [SkywalkerErrors](SkywalkerTools/SkywalkerErrors/README.md) | 错误追踪 |
| [SkywalkerEvent](SkywalkerTools/SkywalkerEvent/README.md) | 事件总线 |
| [SkywalkerPath](SkywalkerTools/SkywalkerPath/README.md) | 路径解析 |
| [SkywalkerSingleton](SkywalkerTools/SkywalkerSingleton/README.md) | 单例模式 |
| [SkywalkerTimer](SkywalkerTools/SkywalkerTimer/README.md) | 计时器 |
| [SkywalkerDerived](SkywalkerTools/SkywalkerDerived/README.md) | 编译期继承检测 |
| [SkywalkerScript](SkywalkerTools/SkywalkerScript/README.md) | 脚本解析器 |
