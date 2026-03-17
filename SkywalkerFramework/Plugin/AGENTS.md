# AGENTS.md - 插件枢纽 (Plugin Hub)

## 概述
包含核心功能插件的容器，采用 C++20 编写，全部以动态库 (Shared Library) 形式构建。

## 目录结构
- `[PluginName]/`：插件根目录（如 Network, CommandLine）
- `[PluginName]/Module/`：内部业务逻辑模块
- `[PluginName]/SFPlugin_[Name].h/.cpp`：插件入口与模块注册点
- `[PluginName]/README.md`：各插件专属 API 与能力说明

## 当前插件
- **Network**: 网络通信、基础socket连接
- **CommandLine**: 命令行与运维管理

## 关键位置
- **入口点**：`SFPlugin_[Name].cpp`。通过 `Install()` 挂载模块。
- **业务实现**：`Module/SFModule_[Name].cpp`。实际的功能代码都在这。
- **加载配置**：`Bin/Server/ServerPlugin.skywalkerC`。控制插件加载顺序。
- **网络核心**：`Network/Object/` 处理网络对象。

## 开发规范
- **注册机制**：必须使用 `SF_PLUGIN_EXPORT` 导出插件，并在 `Install()` 中通过 `SF_REGISTER_MODULE` 注册模块。
- **命名规则**：前缀严格遵循 `SFPlugin_`（插件类）或 `SFModule_`（模块类）。
- **热重载友好**：避免在插件内使用全局静态变量，确保 `Uninstall()` 能干净卸载。

## 禁忌
- **手动加载**：禁止手动调用 `LoadLibrary`。插件加载应完全交给框架配置。
- **硬编码依赖**：不要让插件在链接阶段互相依赖，保持插件间的物理隔离。
- **裸指针管理**：优先使用 `SSF_SHARED_PTR` 等框架宏定义的智能指针，防止内存泄漏。
