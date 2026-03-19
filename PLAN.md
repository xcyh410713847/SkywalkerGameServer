# SkywalkerGameServer — 可测试 Demo 开发计划

**目标**: 将现有框架打造成一个可运行、可测试的最小游戏服务器 Demo。  
**最终交付**: 服务器启动 → 客户端连接 → 登录验证 → 进入场景 → 玩家移动同步 → 场景中有NPC巡逻和怪物 → 多客户端互相可见。  
**约束**: 不引入第三方库，保持项目"全部手写"的学习定位。

---

## 一、插件职责划分

参照 UnrealEngine 设计思想，功能按职责分散到独立插件，插件间通过消息/接口交互，禁止编译期链接依赖。

### 插件全景图

```
┌─────────────────────────────────────────────────────────┐
│                    业务插件层                              │
│  ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌────────────┐  │
│  │  Auth    │ │  Game    │ │   AI     │ │ CommandLine│  │
│  │ 登录验证 │ │ 游戏流程 │ │ AI行为   │ │ 运维命令   │  │
│  └────┬─────┘ └────┬─────┘ └────┬─────┘ └────────────┘  │
│       │            │            │                        │
│  ┌────┴─────┐ ┌────┴─────┐ ┌───┴──────┐                │
│  │ (新建)   │ │ (新建)   │ │ (已有骨架)│                │
│  └──────────┘ └──────────┘ └──────────┘                 │
├─────────────────────────────────────────────────────────┤
│                    世界实体层                              │
│  ┌──────────┐ ┌──────────┐                              │
│  │  Level   │ │  Actor   │                              │
│  │ 场景容器 │ │ 实体体系 │                              │
│  └────┬─────┘ └────┬─────┘                              │
│  ┌────┴─────┐ ┌────┴─────┐                              │
│  │ (已有骨架)│ │ (已有骨架)│                              │
│  └──────────┘ └──────────┘                              │
├─────────────────────────────────────────────────────────┤
│                    基础设施层                              │
│  ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌──────────┐   │
│  │ Network  │ │   DB     │ │ Profiler │ │  Video   │   │
│  │ 网络传输 │ │ 数据存储 │ │ 性能监控 │ │ 录像回放 │   │
│  └──────────┘ └──────────┘ └──────────┘ └──────────┘   │
│  ┌──────────┐ ┌──────────┐ ┌──────────┐                │
│  │ (已实现) │ │ (已有骨架)│ │ (已有骨架)│                │
│  └──────────┘ └──────────┘ └──────────┘                │
└─────────────────────────────────────────────────────────┘
```

### 各插件职责详述

| 插件 | 定位 | Demo 职责 | 状态 |
|------|------|-----------|------|
| **Network** | 纯网络传输层 | 消息协议（帧编解码）、收发缓冲区、心跳、Session（Socket↔连接会话）、消息分发器、SendTo/Broadcast API | 已有TCP骨架，需扩展 |
| **Auth** | 登录/鉴权服务（独立，未来可拆为登录服务器） | 登录验证（PlayerId+Token）、登录成功后通知 Game 创建玩家 | **新建插件** |
| **Actor** | 场景实体基类体系（类似 UE AActor） | Actor 基类（位置/朝向/状态）→ Creature(生物基类，含HP/移动速度) → Player / NPC / Monster | 已有骨架，需实现类体系 |
| **Level** | 场景/世界容器 | Scene 管理、Actor 注册进出场景、场景内 AOI 广播（Demo 全场景广播）、移动同步 | 已有骨架，需实现模块 |
| **Game** | 游戏业务流程控制 | 游戏流程调度（登录→创建Player→进入场景）、消息Handler注册、玩家下线清理 | 已有骨架目录，需实现 |
| **AI** | AI 行为系统 | NPC 巡逻行为、Monster 简单AI（idle/追击范围检测） | 已有骨架，需实现模块 |
| **DB** | 数据访问层 | Demo 用内存 Map 存储玩家/NPC/怪物数据，接口预留持久化 | 已有骨架，需实现模块 |
| **CommandLine** | 运维管理 | 信号处理（已有）、status 命令 | 已有 SFModule_OS |
| **Profiler** | 性能监控 | Demo 可选：在线人数、消息吞吐统计日志 | 已有骨架，P4 阶段 |
| **Video** | 录像回放 | Demo 不涉及 | 保持骨架 |

### 实体类继承体系（Actor 插件）

```
SSFObject (框架基类)
  └─ SFActor (场景实体基类)
       │  - Position (x, y, z)
       │  - Rotation (yaw)
       │  - ActorType 枚举
       │  - SceneId
       │  - bActive
       │
       └─ SFCreature (生物基类)
            │  - HP / MaxHP
            │  - MoveSpeed
            │  - bAlive
            │
            ├─ SFPlayer (玩家)
            │    - PlayerId
            │    - SessionId
            │    - PlayerName
            │
            ├─ SFNPC (NPC)
            │    - NPCId
            │    - NPCType
            │    - PatrolPath (巡逻路径点)
            │
            └─ SFMonster (怪物)
                 - MonsterId
                 - MonsterType
                 - SpawnPosition (出生点)
                 - AggroRange (仇恨范围)
```

### 插件间交互流程（登录→进入场景→移动同步）

```
Client ──LoginReq──→ [Network] ──dispatch──→ [Auth] 验证Token
                                                │
                                          LoginSuccess
                                                │
                                                ▼
                                            [Game] 创建 SFPlayer
                                                │
                                                ├──→ [DB] 查询玩家数据(内存)
                                                │
                                                ├──→ [Actor] 注册 Player 实例
                                                │
                                                └──→ [Level] 加入默认场景
                                                       │
                                                       ├── 广播 PlayerEnterScene
                                                       │   给场景内所有人
                                                       │
                                                       └── 回复 EnterSceneResp
                                                           (含场景内所有Actor列表)

Client ──MoveReq──→ [Network] ──dispatch──→ [Level] 更新Player位置
                                                │
                                                └── 广播 MoveBroadcast
                                                    给同场景其他玩家
```

---

## 二、现状评估

### 已完成 ✅

| 组件 | 状态 | 说明 |
|------|------|------|
| 框架主循环 | **可用** | Start/Tick/Stop 生命周期完整 |
| 插件管理器 | **可用** | 配置解析 → DLL加载 → DllStartPlugin → 模块生命周期 |
| Timer 服务 | **可用** | 帧间隔计算、总时间统计 |
| 动态库加载 | **可用** | 跨平台 LoadLibrary/dlopen 封装 |
| TCP 服务端 | **可用** | socket/bind/listen/accept、非阻塞、客户端连接管理 |
| TCP 客户端 | **可用** | connect/send、非阻塞、断线重连 |
| 信号处理 | **可用** | Ctrl+C 优雅关服 |
| 配置系统 | **可用** | SkywalkerScript 解析器、环境变量注入 |
| 编译系统 | **可用** | CMake C++20、已有成功编译产物 |

### 未完成 ❌

| 组件 | 缺失内容 |
|------|----------|
| 消息协议 | 无消息帧定义，收到的字节直接丢弃 |
| 消息分发 | 无消息路由、无 Handler 注册机制 |
| 服务端发送 | 无服务器向客户端发送的高层 API |
| 会话管理 | 仅有 Socket→对象 映射，无 Session 概念 |
| 实体体系 | 无 Actor/Creature/Player/NPC/Monster 类 |
| 场景管理 | 无 Scene 容器、无 Actor 注册/广播 |
| 登录系统 | 无登录验证 |
| 游戏流程 | 无业务调度 |
| AI 系统 | 无 NPC/怪物行为 |
| 骨架插件容错 | 配置引用了未实现的模块，运行时可能报错 |

### 需处理的配置问题

`ServerPlugin.skywalkerC` 引用了未实现的模块。  
**处理策略**: 框架增加容错机制 — 配置的模块找不到时 warn 日志而非 error 终止。

---

## 三、消息协议设计

采用 **长度前缀 + 消息ID + 二进制负载** 方案（贴近真实游戏服务器协议）。

### 消息帧格式

```
┌──────────┬──────────┬─────────────────┐
│ Length    │ MsgID    │ Payload         │
│ 4 bytes  │ 2 bytes  │ N bytes         │
│ uint32   │ uint16   │ 业务数据        │
│ (网络序) │ (网络序) │                 │
└──────────┴──────────┴─────────────────┘

Length = sizeof(MsgID) + sizeof(Payload) = 2 + N
总包长 = 4 + Length
```

### 消息ID 分配

| 范围 | 用途 | 处理插件 |
|------|------|----------|
| 0x0001 - 0x00FF | 系统消息（心跳） | Network |
| 0x0100 - 0x01FF | 登录/鉴权 | Auth |
| 0x0200 - 0x02FF | 场景/世界 | Level |
| 0x0300 - 0x03FF | 角色/移动 | Level (广播) |
| 0x0400 - 0x04FF | 游戏业务 | Game |
| 0x0500 - 0x05FF | AI/生物 | AI |

### Demo 所需消息列表

| MsgID | 名称 | 方向 | Payload | 处理插件 |
|-------|------|------|---------|----------|
| 0x0001 | Heartbeat | 双向 | 无 | Network |
| 0x0101 | LoginReq | C→S | PlayerId(u32) + Token(string) | Auth |
| 0x0102 | LoginResp | S→C | Result(u8) + SessionId(u32) | Auth |
| 0x0201 | EnterSceneReq | C→S | SceneId(u32) | Level |
| 0x0202 | EnterSceneResp | S→C | Result(u8) + ActorCount(u16) + [ActorInfo...] | Level |
| 0x0203 | ActorEnterScene | S→C | ActorType(u8) + ActorId(u32) + Position(xyz) | Level |
| 0x0204 | ActorLeaveScene | S→C | ActorId(u32) | Level |
| 0x0301 | MoveReq | C→S | Position(x,y,z float) | Level |
| 0x0302 | MoveBroadcast | S→C | ActorId(u32) + Position(x,y,z float) | Level |
| 0x0501 | CreatureSpawn | S→C | ActorType(u8) + CreatureInfo | AI (触发) |

---

## 四、分阶段开发计划

### Phase 0: 编译验证与容错（预计 0.5 天）

**目标**: 确保项目可编译运行，骨架插件不影响启动。  
**涉及插件**: Core（框架）

- [x] P0-1: 验证当前代码可编译通过
- [x] P0-2: 修改 `SFPluginManager` — 模块注册找不到时输出 warn 而非 error 终止
- [x] P0-3: 运行服务器确认启动不报错、监听端口正常
- [x] P0-4: 运行客户端确认连接服务器成功

**验收**: 服务器启动 → 客户端连接 → 日志 "New ClientSocket" → Ctrl+C 关闭。

---

### Phase 1: 消息协议层（预计 2-3 天）

**目标**: 实现消息帧的编解码、收发缓冲区、消息分发机制。  
**涉及插件**: **Network**（全部在此插件内完成）

#### 1.1 收发缓冲区

- [x] P1-1: `Network/Object/SFNetworkBuffer.h` — 线性收发缓冲区
- [x] P1-2: 改造 `SSFObject_ClientSocket` — 添加 RecvBuffer/SendBuffer

#### 1.2 消息帧编解码

- [x] P1-3: `Network/Object/SFMessageCodec.h/.cpp` — 编解码器
- [x] P1-4: `SSFObject_ClientSocket::Tick` 集成 Codec

#### 1.3 消息分发

- [x] P1-5: `Network/Object/SFMessageDispatcher.h/.cpp` — 消息分发器
  - `RegisterHandler(MsgID, Callback)` 供其他插件的模块注册
  - `Dispatch(Session, MsgID, Payload, Len)` 分发消息到对应 Handler
- [x] P1-6: `SFModule_NetworkServer` 集成分发器

#### 1.4 会话管理（Session 属于 Network）

- [x] P1-7: `Network/Object/SFSession.h/.cpp` — 会话对象
  - SessionId(u32)、Socket、登录状态、PlayerId（登录后绑定）、最后活跃时间
- [x] P1-8: `SFModule_NetworkServer` 管理 SessionMap
  - 连接建立 → 创建 Session（未登录态）
  - 登录成功 → Session 标记已登录 + 绑定 PlayerId
  - 断开 → 清理 Session → 通知相关插件

#### 1.5 发送 API 与心跳

- [x] P1-9: `SFModule_NetworkServer` 添加发送接口
  - `SendTo(SessionId, MsgID, Payload, Len)`
  - `Broadcast(MsgID, Payload, Len)`
  - `BroadcastToScene(SceneId, MsgID, Payload, Len)` — 预留接口
- [x] P1-10: 心跳机制（服务端超时检测、客户端定时发送）

**验收**: 客户端连接 → 心跳正常 → 可注册自定义 Handler 收到消息 → Echo 测试通过。

---

### Phase 2: 登录系统（预计 1 天）

**目标**: 独立的登录验证流程。  
**涉及插件**: **Auth**（新建）、Network（Session 状态变更）

- [x] P2-1: 新建 `SFPlugin_Auth`，注册 `SFModule_Auth`
- [x] P2-2: `SFModule_Auth` 注册 LoginReq(0x0101) Handler
  - 验证 PlayerId + Token（Demo: 硬编码白名单或配置文件）
  - 成功 → 更新 Session 为已登录态 + 绑定 PlayerId → 回复 LoginResp(成功)
  - 失败 → 回复 LoginResp(失败) → 断开连接
- [x] P2-3: 客户端连接后自动发送 LoginReq
- [x] P2-4: 添加 `Bin/Server/ServerPlugin.skywalkerC` 中 Auth 插件配置
- [x] P2-5: 未登录时发送非系统消息 → 在 Network 分发器层拦截拒绝

**验收**: 客户端连接 → 自动登录 → 正确 Token 成功 → 错误 Token 被拒绝断开。

---

### Phase 3: 实体体系 + 场景管理（预计 3-4 天）

**目标**: 建立 Actor/Creature 类体系，场景管理，玩家移动同步。  
**涉及插件**: **Actor**、**Level**、**Game**

#### 3.1 Actor 插件 — 实体类体系

- [x] P3-1: 实现 `SFPlugin_Actor`，Install() 注册 `SFModule_ActorFactory`
- [x] P3-2: 定义 Actor 基类 `Actor/Object/SFActor.h`
  - Position(x,y,z)、Rotation(yaw)、ActorType 枚举、SceneId、bActive
  - 虚函数: `Tick(DeltaMS)`、`OnEnterScene()`、`OnLeaveScene()`
- [x] P3-3: 定义 Creature 生物基类 `Actor/Object/SFCreature.h`
  - 继承 SFActor
  - HP/MaxHP、MoveSpeed、bAlive
  - 虚函数: `OnDeath()`、`OnMove()`
- [x] P3-4: 定义 Player `Actor/Object/SFPlayer.h`
  - 继承 SFCreature
  - PlayerId、SessionId、PlayerName
- [x] P3-5: 定义 NPC `Actor/Object/SFNPC.h`
  - 继承 SFCreature
  - NPCId、NPCType、PatrolPath(路径点数组)
- [x] P3-6: 定义 Monster `Actor/Object/SFMonster.h`
  - 继承 SFCreature
  - MonsterId、MonsterType、SpawnPosition、AggroRange
- [x] P3-7: `SFModule_ActorFactory` — Actor 工厂
  - `CreatePlayer(PlayerId)` → SFPlayer*
  - `CreateNPC(NPCConfig)` → SFNPC*
  - `CreateMonster(MonsterConfig)` → SFMonster*
  - `DestroyActor(ActorId)`

#### 3.2 Level 插件 — 场景管理

- [x] P3-8: 实现 `SFPlugin_Level`，Install() 注册 `SFModule_SceneManager`
- [x] P3-9: 定义 `Level/Object/SFScene.h` — 场景对象
  - SceneId、SceneName
  - ActorMap (ActorId → SFActor*)
  - `AddActor()` / `RemoveActor()` / `GetAllActors()`
  - `BroadcastToScene(MsgID, Payload, Len)` — 通过 Network 的 SendTo 发给场景内所有玩家
  - `Tick(DeltaMS)` — Tick 场景内所有 Actor
- [x] P3-10: `SFModule_SceneManager` — 场景管理模块
  - 管理 SceneMap（Demo: 启动时创建默认场景 SceneId=1）
  - 注册 EnterSceneReq(0x0201) Handler → 将 Player 加入场景 → 广播 ActorEnterScene → 回复完整 Actor 列表
  - 注册 MoveReq(0x0301) Handler → 更新 Actor 位置 → 广播 MoveBroadcast
  - 监听玩家断线 → 移除 Actor → 广播 ActorLeaveScene

#### 3.3 Game 插件 — 业务流程

- [x] P3-11: 实现 `SFPlugin_Game`，Install() 注册 `SFModule_GameFlow`
- [x] P3-12: `SFModule_GameFlow` — 游戏流程控制
  - 监听 Auth 登录成功事件 → 通过 ActorFactory 创建 SFPlayer → 通知 Level 将 Player 加入默认场景
  - 监听玩家断线 → 通过 ActorFactory 销毁 Player
  - 负责 NPC/Monster 初始化配置（从 ServerConfig 或硬编码读取出生点数据）
  - `Init()` 阶段创建默认 NPC 和 Monster 并加入场景

#### 3.4 客户端场景交互

- [x] P3-13: 客户端登录成功后自动发送 EnterSceneReq
- [x] P3-14: 客户端收到 EnterSceneResp 打印场景内所有 Actor
- [x] P3-15: 客户端定时发送 MoveReq（模拟移动）
- [x] P3-16: 客户端收到 MoveBroadcast / ActorEnterScene / ActorLeaveScene 打印日志

**验收**:
1. 服务器启动 → 默认场景创建 → NPC 和 Monster 已在场景中
2. 客户端A 登录 → 进入场景 → 收到场景内 Actor 列表（含 NPC/Monster）
3. 客户端B 登录 → 进入场景 → 收到 A + NPC + Monster 信息
4. A 移动 → B 收到 MoveBroadcast
5. B 断开 → A 收到 ActorLeaveScene

---

### Phase 4: AI 行为（预计 1-2 天）

**目标**: NPC 巡逻、Monster 简单行为，展示 Actor 体系的多态性。  
**涉及插件**: **AI**

- [x] P4-1: 实现 `SFPlugin_AI`，Install() 注册 `SFModule_AIRuntime`
- [x] P4-2: `SFModule_AIRuntime` — AI 调度模块
  - Tick 中遍历场景内所有 NPC/Monster
  - 对 NPC: 按 PatrolPath 巡逻（到达路径点后移向下一个），每次移动广播 MoveBroadcast
  - 对 Monster: idle 状态，检测范围内是否有 Player，有则朝 Player 方向移动（追击），广播 MoveBroadcast
- [ ] P4-3: AI Tick 预算控制（跳过：Demo 规模不需要预算控制）

**验收**: NPC 在场景中来回巡逻 → 客户端收到 NPC 位置变化 → Monster 在玩家靠近时追击移动。

---

### Phase 5: 打磨与测试（预计 1-2 天）

**目标**: 健壮性、可观测性、文档。  
**涉及插件**: 全部

- [ ] P5-1: 错误处理完善（消息过大防御、未登录拦截、重复登录防御）
- [ ] P5-2: Profiler 插件实现最小监控（在线人数/场景人数/消息吞吐日志）
- [ ] P5-3: `SFModule_OS` 添加 status 命令
- [ ] P5-4: 客户端命令行模式（`move x y z`、`quit`）
- [ ] P5-5: 多客户端压力测试（10+ 并发）
- [ ] P5-6: 更新 README.md / AGENTS.md

**验收**: 10 个客户端同时在线，玩家移动 + NPC 巡逻 + Monster 追击 全部同步正常，Ctrl+C 优雅关闭。

---

## 五、文件变更预估

### 新增文件

| 文件路径 | Phase | 插件 | 说明 |
|----------|-------|------|------|
| `Network/Object/SFNetworkBuffer.h` | P1 | Network | 收发缓冲区 |
| `Network/Object/SFMessageCodec.h/.cpp` | P1 | Network | 消息帧编解码 |
| `Network/Object/SFMessageDispatcher.h/.cpp` | P1 | Network | 消息分发器 |
| `Network/Object/SFSession.h/.cpp` | P1 | Network | 会话对象 |
| `Plugin/Auth/SFPlugin_Auth.h/.cpp` | P2 | **Auth(新建)** | 登录插件入口 |
| `Plugin/Auth/Module/SFModule_Auth.h/.cpp` | P2 | Auth | 登录验证模块 |
| `Plugin/Auth/CMakeLists.txt` | P2 | Auth | 构建配置 |
| `Actor/Object/SFActor.h/.cpp` | P3 | Actor | Actor 基类 |
| `Actor/Object/SFCreature.h/.cpp` | P3 | Actor | 生物基类 |
| `Actor/Object/SFPlayer.h/.cpp` | P3 | Actor | 玩家 |
| `Actor/Object/SFNPC.h/.cpp` | P3 | Actor | NPC |
| `Actor/Object/SFMonster.h/.cpp` | P3 | Actor | 怪物 |
| `Actor/Module/SFModule_ActorFactory.h/.cpp` | P3 | Actor | Actor 工厂 |
| `Level/Object/SFScene.h/.cpp` | P3 | Level | 场景对象 |
| `Level/Module/SFModule_SceneManager.h/.cpp` | P3 | Level | 场景管理模块 |
| `Game/Module/SFModule_GameFlow.h/.cpp` | P3 | Game | 游戏流程控制 |
| `AI/Module/SFModule_AIRuntime.h/.cpp` | P4 | AI | AI 行为调度 |

### 修改文件

| 文件 | Phase | 改动 |
|------|-------|------|
| `Core/Plugin/SFPluginManager.cpp` | P0 | 模块缺失时 warn 而非 error |
| `Network/Object/SFObject_ClientSocket.h/.cpp` | P1 | 收发缓冲区、Codec 集成 |
| `Network/Module/SFModule_NetworkServer.h/.cpp` | P1 | 分发器、Session、发送 API |
| `Network/Module/SFModule_NetworkClient.h/.cpp` | P1-P5 | 接收处理、登录、输入循环 |
| `Actor/SFPlugin_Actor.cpp` | P3 | Install() 注册 ActorFactory |
| `Level/SFPlugin_Level.cpp` | P3 | Install() 注册 SceneManager |
| `Game/SFPlugin_Game.cpp` (已有骨架目录) | P3 | Install() 注册 GameFlow |
| `AI/SFPlugin_AI.cpp` | P4 | Install() 注册 AIRuntime |
| `Bin/Server/ServerPlugin.skywalkerC` | P2-P3 | 添加 Auth/Game 配置，更新模块列表 |
| `Plugin/CMakeLists.txt` | P2 | 添加 Auth 子目录 |

---

## 六、里程碑总结

| Phase | 交付物 | 涉及插件 | 预计耗时 |
|-------|--------|----------|----------|
| **P0** | 编译通过、容错启动 | Core | 0.5 天 |
| **P1** | 消息协议、Session、心跳、Echo | Network | 2-3 天 |
| **P2** | 独立登录验证流程 | Auth(新建), Network | 1 天 |
| **P3** | Actor类体系、场景管理、移动同步 | Actor, Level, Game | 3-4 天 |
| **P4** | NPC 巡逻、Monster 追击 | AI | 1-2 天 |
| **P5** | 健壮性、监控、文档、压测 | Profiler, CommandLine, 全部 | 1-2 天 |
| **总计** | 可测试的最小游戏服务器 Demo | **7个插件** | **9-13 天** |

---

## 七、技术决策记录

| 决策 | 选项 | 选择 | 理由 |
|------|------|------|------|
| 消息协议 | JSON / 二进制 | 长度前缀+MsgID+二进制 | 贴近真实游戏协议 |
| 骨架插件 | 删除/容错/空模块 | 框架容错（warn不中断） | 保留扩展空间 |
| 数据存储 | 文件/SQLite/内存 | 纯内存 | Demo 不需持久化 |
| 场景同步 | 帧同步/状态同步 | 状态同步 | 实现简单 |
| 登录插件 | 放在Game/独立 | **独立 Auth 插件** | 未来可拆为登录服务器 |
| Session 归属 | Network/Game | **Network** | Session 是传输层概念 |
| 实体体系 | 扁平/继承 | Actor→Creature→Player/NPC/Monster | 参照 UE，展示多态 |
| NPC/Monster | 不做/做 | **做** | 展示 Actor 体系完整性 |

---

## 八、风险与注意

1. **插件间通信**: 插件禁止编译期依赖。跨插件交互通过 Network 消息分发器回调实现，或通过 Framework 的 Service 层传递
2. **线程安全**: 单线程 Tick 驱动，Demo 阶段保持单线程
3. **非阻塞 I/O**: 无 select/epoll，Demo 10-20 连接可接受
4. **SF_ERROR_TRACE Bug**: 全程使用 `SF_ERROR_DESC_TRACE`
5. **Actor 生命周期**: Actor 创建/销毁统一通过 ActorFactory，禁止其他模块直接 new/delete
6. **消息 Handler 注册时机**: 各插件在 `Module::Init()` 阶段注册 Handler，确保在 `Start()` 前就绪
7. **Game 插件目录**: 已有空的子目录结构（Account/Character/Scene 等），新模块放在 Module/ 下即可
