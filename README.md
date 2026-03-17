# SkywalkerGameServer

一套完整的游戏服务器框架，除标准库外全部手写，旨在学习服务器开发。

## 架构设计

### 三层架构

```
┌─────────────────────────────────┐
│          Plugin (动态库)          │  ← Network、AI、DB 等插件
├─────────────────────────────────┤
│     SkywalkerFramework (静态库)   │  ← 框架核心
├─────────────────────────────────┤
│          SFCore (静态库)          │  ← 底层基础设施
└─────────────────────────────────┘
```

### 多线程设计

**单框架内：**
- 网络线程：处理网络消息
- 资源管理线程：管理服务器资源
- 场景线程：管理游戏场景
- 主线程：处理游戏逻辑

**多框架：**
- 多个 `CSkywalkerFramework` 实例可协同工作
- 便于用编辑器统一管理

## 项目结构

```
SkywalkerGameServer/
├── Bin/                      # 可执行文件输出目录
├── build/                    # CMake 构建目录
├── SkywalkerGameServer/       # 服务器主程序
├── SkywalkerGameClient/      # 测试客户端
├── SkywalkerFramework/       # 核心框架
│   ├── Core/                 # 静态库：服务管理、插件加载
│   ├── Plugin/               # 动态插件
│   └── Include/              # 公共头文件
├── SkywalkerTools/           # 工具库
├── SkywalkerScript/          # 脚本编码工具
├── SkywalkerServerEditor/    # 服务器编辑器
└── SkywalkerTest/            # 测试运行器
```

## 编译与测试

```bash
# 编译
cd build/SkywalkerGameServer
cmake ..
cmake --build . --config Debug

# 运行测试
cmake --build . --target SkywalkerTestRunner --config Debug
./Bin/Debug/SkywalkerTestRunner.exe
```

## 开发规范

- **所有代码必须包含单元测试** - 无测试不提交
- **插件-模块设计** - 功能以插件形式实现
- **跨平台** - 从设计之初考虑 Windows/Linux/iOS

## 文档索引

### 核心框架
| 文件 | 说明 |
|------|------|
| [SkywalkerFramework/README.md](SkywalkerFramework/README.md) | 核心框架目录，包含 Core/Plugin/Include |
| [SkywalkerTools/README.md](SkywalkerTools/README.md) | 工具库，提供平台抽象、对象池、错误追踪等 |

### 插件
| 文件 | 说明 |
|------|------|
| [Network 插件](SkywalkerFramework/Plugin/Network/README.md) | 网络通信：连接接入、协议编解码、会话管理、消息路由 |
| [AI 插件](SkywalkerFramework/Plugin/AI/README.md) | AI 运行时：策略调度、预算控制、策略审计 |
| [DB 插件](SkywalkerFramework/Plugin/DB/README.md) | 数据访问：玩家数据仓储、数据库访问层 |
| [Level 插件](SkywalkerFramework/Plugin/Level/README.md) | 场景管理 |
| [Actor 插件](SkywalkerFramework/Plugin/Actor/README.md) | 角色实体 |
| [Auth 插件](SkywalkerFramework/Plugin/Auth/README.md) | 认证授权 |
| [CommandLine 插件](SkywalkerFramework/Plugin/CommandLine/README.md) | 命令行与 Admin 管理 |
| [Profiler 插件](SkywalkerFramework/Plugin/Profiler/README.md) | 性能分析 |
| [Test 插件](SkywalkerFramework/Plugin/Test/README.md) | 测试支持 |
| [Video 插件](SkywalkerFramework/Plugin/Video/README.md) | Replay 回放 |

### 工具库
| 文件 | 说明 |
|------|------|
| [SkywalkerPlatform](SkywalkerTools/SkywalkerPlatform/README.md) | 平台抽象：跨平台宏、动态库加载 |
| [SkywalkerPool](SkywalkerTools/SkywalkerPool/README.md) | 对象池 |
| [SkywalkerErrors](SkywalkerTools/SkywalkerErrors/README.md) | 错误追踪与容器 |
| [SkywalkerEvent](SkywalkerTools/SkywalkerEvent/README.md) | 事件总线 |
| [SkywalkerPath](SkywalkerTools/SkywalkerPath/README.md) | 路径解析 |
| [SkywalkerSingleton](SkywalkerTools/SkywalkerSingleton/README.md) | 单例模式 |
| [SkywalkerTimer](SkywalkerTools/SkywalkerTimer/README.md) | 计时器 |
| [SkywalkerTest](SkywalkerTools/SkywalkerTest/README.md) | 单元测试框架 |
| [SkywalkerDerived](SkywalkerTools/SkywalkerDerived/README.md) | 编译期继承检测 |
