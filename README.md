# SkywalkerGameServer

一套完整的服务器代码框架，除系统库之外，全部自己手写，性能不考虑，只是为了学习

## 当前开发状态（2026-03-03）

- 当前阶段：已推进到 `P2 第八批（7.13）`
- 验证方式：保持“只编译不运行”，每轮改动后全量 CMake 编译通过
- 网络侧：协议/路由/会话/黑名单（持久化+热重载）已具备
- AI 侧：策略切换（strict/balanced/relaxed）+ 审计查询/清理已具备
- Replay 侧：录制落盘、版本头、校验和、索引/区间/关键字查询已具备
- 运维侧：Admin 命令分级权限（admin/operator/observer）+ ACL 配置化重载已具备

## 快速导航（建议先看）

- 阶段落地与路线图：`框架插件与模块缺口清单及落地步骤.md`
- 会话交接与继续开发入口：`JobRecord.md`
- 服务器关键配置：`Bin/Server/ServerConfig.skywalkerC`
- 网络与能力回归测试：`SkywalkerGameServer/SkywalkerNetworkTest.cpp`

## 插件 API 文档索引

- [Actor 插件 API](SkywalkerFramework/Plugin/Actor/README.md)
- [AI 插件 API](SkywalkerFramework/Plugin/AI/README.md)
- [Auth 插件 API](SkywalkerFramework/Plugin/Auth/README.md)
- [CommandLine 插件 API](SkywalkerFramework/Plugin/CommandLine/README.md)
- [DB 插件 API](SkywalkerFramework/Plugin/DB/README.md)
- [Level 插件 API](SkywalkerFramework/Plugin/Level/README.md)
- [Network 插件 API](SkywalkerFramework/Plugin/Network/README.md)
- [Profiler 插件 API](SkywalkerFramework/Plugin/Profiler/README.md)
- [Test 插件 API](SkywalkerFramework/Plugin/Test/README.md)
- [Video 插件 API](SkywalkerFramework/Plugin/Video/README.md)

## 当前关键能力入口

- 核心网关：`SkywalkerFramework/Core/Service/SSFGameplayServiceGateway.h`
- 网络服务模块：`SkywalkerFramework/Plugin/Network/Module/SSFModule_NetworkServer.cpp`
- AI 运行时模块：`SkywalkerFramework/Plugin/AI/Module/SSFModule_AIRuntime.cpp`
- Replay 播放模块：`SkywalkerFramework/Plugin/Video/Module/SSFModule_ReplayPlayer.cpp`
- 管理命令模块：`SkywalkerFramework/Plugin/CommandLine/Module/SSFModule_AdminCommand.cpp`

## 当前推荐继续顺序（下次会话）

1. 先阅读 `JobRecord.md` 的“本地继续步骤”。
2. 查看 `框架插件与模块缺口清单及落地步骤.md` 最新小节（7.x）。
3. 修改后仅执行全量编译，确保 `SkywalkerTestRunner` 可生成。
4. 再进行提交与阶段文档同步。

## 目录说明

### Bin

执行目录

### SkywalkerGameClient

与**SkywalkerGameServer**配套的测试客户端

### SkywalkerGameServer

服务器

### SkywalkerScript

专用脚本编码工具

### SkywalkerServerEditor

服务器编辑器

### SkywalkerFramework

服务器框架

### SkywalkerTools

各种独立的工具

## 架构设想

### 关于多线程

分成两部分来设计

#### 单个 `CSkywalkerFramework`

每个 `CSkywalkerFramework`中本身存在多线程，线程只能在 `CSkywalkerFramework`中通信

线程根据功能暂且分

* 网络线程，只用来处理网络消息
* 资源管理线程，管理服务器的资源
* 场景线程，管理场景
* 主线程，负责游戏的逻辑

#### 多个 `CSkywalkerFramework`

可以使用多个 `CSkywalkerFramework`，将每个 `CSkywalkerFramework`当成一个线程，`CSkywalkerFramework`之间可以通信

为什么实现多个 `CSkywalkerFramework`设计

1. 方便一个编辑器管理全部服务器

### 关于 `CSkywalkerFramework`设计

分成三部分组成

#### Core

这中间的代码会生成一个静态链接库

#### SkywalkerFramework

会链接 `Core`生成的静态链接库，然后自身也会生成一个静态链接库

#### Plugin

里面存放了各种功能独立的插件(Plugin)，每个插件都会生成一个动态库，通过配置加载需要的动态库即可

## FAQ记录一些开发过程中遇到的问题

### 宏定义函数报错

* 是不是使用了单行注释，换成多行注释试下

### 枚举未声明

* 是不是它前面那个使用了中文注释，修改一下编码试试

### _CrtlsValidHeapPointer(block)

是不是释放了智能指针
