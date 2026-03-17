# SkywalkerFramework

## 概述
游戏服务器核心框架，包含静态库核心和动态插件系统。

## 目录结构
```
SkywalkerFramework/
├── Core/           # 静态库：服务管理、插件加载、事件定时器
├── Plugin/         # 动态插件
└── Include/        # 公共头文件
```

## 关键位置
- **SFCore.h**: 核心类型定义与指针宏
- **SFPluginManager.h**: 插件动态库扫描与注册入口
- **SFGameplayServiceGateway.h**: 游戏业务服务网关

## 编译输出
- Core → SFCore.lib (静态库)
- Plugin → SFPlugin_*.dll/.so (动态库)

## 相关文档
- [Core/AGENTS.md](Core/AGENTS.md) - 核心基础库
- [Plugin/AGENTS.md](Plugin/AGENTS.md) - 插件系统