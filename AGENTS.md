# SkywalkerGameServer 开发知识库

## 概述
C++20 游戏服务器框架，基于插件化架构设计，用于学习目的。

## 项目结构
```
SkywalkerGameServer/
├── SkywalkerFramework/         # 核心框架
│   ├── Core/                  # 静态库：服务管理、插件加载
│   ├── Plugin/                # 动态插件：Network/CommandLine等
│   └── Include/               # 公共头文件
├── SkywalkerGameServer/       # 服务器可执行文件
├── SkywalkerGameClient/        # 测试客户端
├── SkywalkerTools/            # 工具库：平台抽象/对象池/错误追踪
├── Bin/                      # 编译输出目录
└── build/                    # CMake 构建目录
```

## 快速导航
| 任务 | 位置 | 备注 |
|------|------|------|
| 框架核心 | `SkywalkerFramework/Core/AGENTS.md` | 服务管理、插件机制、已知Bug |
| 插件系统 | `SkywalkerFramework/Plugin/AGENTS.md` | 动态加载配置 |
| 工具库 | `SkywalkerTools/AGENTS.md` | 平台抽象、内存池、错误处理 |
| 入口点 | `SkywalkerGameServer/SkywalkerGameServer.cpp` | main函数 |
| 插件配置 | `Bin/Server/ServerPlugin.skywalkerC` | 插件加载列表 |
| 网络模块 | `SkywalkerFramework/Plugin/Network/Module/SFModule_NetworkServer.cpp` | 网络服务入口 |

## 编译命令
```bash
cd build/SkywalkerGameServer
cmake ..
cmake --build . --config Debug
cmake --build . --target SkywalkerGameServer --config Debug
```

## 核心规范
- **插件-模块设计** - 功能以插件形式实现
- **跨平台** - 从设计之初考虑 Windows/Linux/iOS
- **无网关设计** - 插件间直接调用，无统一网关

## 已知问题
1. **宏错误**: 检查是否使用单行注释 → 改用多行注释
2. **枚举未声明**: 检查前一行是否有中文注释 → 尝试修改编码
3. **_CrtIsValidHeapPointer**: 错误释放了智能指针
4. **SF_ERROR_TRACE宏Bug**: 传递了错误参数，应使用 `SF_ERROR_DESC_TRACE`

## 关键文件头格式
```cpp
/*************************************************************************
**文件: SkywalkerFramework\Plugin\Test\SFPlugin_Test.h
**作者: shyfan
**日期: 2023/08/21 19:54:39
**功能: 测试插件
*************************************************************************/
```

## 命名约定
| 类型 | 约定 | 示例 |
|------|------|------|
| 类 | C前缀 | `CSkywalkerFramework` |
| 插件 | SFPlugin_前缀 | `SFPlugin_Network` |
| 模块 | SFModule_前缀 | `SFModule_NetworkServer` |
| 接口 | I前缀 | `ISSFPlugin` |
| 枚举 | E前缀 | `ESkywalkerServerState` |
| 文件 | PascalCase | `SkywalkerFramework.h` |
