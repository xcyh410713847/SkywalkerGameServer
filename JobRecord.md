# SkywalkerGameServer 会话交接记录（持续更新）

## 1) 当前总体状态（2026-03-03）

- 分支：`master`（本地持续领先远端）
- 开发阶段：P0/P1 已打底，P2 已推进到第八批（文档 7.13）
- 工作原则：只编译不运行
- 质量门禁：每轮改动后执行全量 CMake 编译，确保 `SkywalkerTestRunner` 生成成功

## 2) 当前能力矩阵（可直接续作）

### 网络（Network）
- 已有统一协议头、消息路由、登录载荷、会话生命周期
- 已有限流、异常统计、黑名单持久化、黑名单热重载

### AI（AI Runtime）
- 支持策略：`strict` / `balanced` / `relaxed`
- 支持预算统计：`TickBudgetMS`、超预算计数
- 支持审计：策略切换审计查询与清理

### 回放（Replay）
- 录制：落盘 `Session_<id>.replay`
- 格式：`Magic/Version/SessionId/EventCount/Checksum/EventsBegin`
- 播放：版本校验 + 校验和校验
- 查询：按索引、按区间、按关键字

### 命令与权限（AdminCommand）
- 角色：`admin` / `operator` / `observer`
- 支持 `role=<role> <command>` 语法
- ACL 可配置并支持 `reload_acl`
- 支持执行统计：success/denied/failure

## 3) 关键代码入口（下次优先看）

- 核心网关：`SkywalkerFramework/Core/Service/SSFGameplayServiceGateway.h`
- 管理命令：`SkywalkerFramework/Plugin/CommandLine/Module/SSFModule_AdminCommand.cpp`
- AI 运行时：`SkywalkerFramework/Plugin/AI/Module/SSFModule_AIRuntime.cpp`
- Replay 播放：`SkywalkerFramework/Plugin/Video/Module/SSFModule_ReplayPlayer.cpp`
- 网络服务：`SkywalkerFramework/Plugin/Network/Module/SSFModule_NetworkServer.cpp`
- 回归测试：`SkywalkerGameServer/SkywalkerNetworkTest.cpp`

## 4) 配置关键项（ServerConfig）

位置：`Bin/Server/ServerConfig.skywalkerC`

- 网络：`IP`、`Port`、`SessionTimeoutMS`、`MaxMsgPerSecond`
- 黑名单：`BlacklistPersistence`、`BlacklistFilePath`、`BlacklistHotReload`
- AI：`AITickBudgetMS`、`AIStrategy`
- Replay：`ReplayDirectory`
- ACL：`AdminACL_Admin`、`AdminACL_Operator`、`AdminACL_Observer`

## 5) 常用命令（约定：仅编译）

- 全量编译：使用 VS Code CMake Tools 的 Build（默认目标）
- 不执行服务端、不执行客户端（遵循当前流程约束）

## 6) 本地继续步骤（下次会话照此执行）

1. 打开 `框架插件与模块缺口清单及落地步骤.md`，定位最新 7.x 小节。
2. 阅读 `SkywalkerGameServer/SkywalkerNetworkTest.cpp`，确认新增能力测试覆盖点。
3. 在对应模块做增量改动（优先网关 -> 模块 -> 命令 -> 测试）。
4. 只执行全量编译验证，修复编译问题后再更新文档。
5. 提交时使用中文阶段化提交信息（保持历史可追溯）。

## 7) 仍可继续推进的方向（建议）

- Replay：查询性能统计结构化（命中率/耗时桶）
- AI：审计持久化（文件落盘 + 重载）
- ACL：更细粒度命令组与错误码统一
- 测试：扩展更多边界与异常路径（空配置、无回调、极值参数）

## 8) 最近关键问题与修复（2026-03-03）

### 关服流程卡住（日志停在 PluginManager/Plugin Release）
- 现象：服务端关闭时日志停在 `SFPluginManager Release` 或 `SSFPlugin_Video Release`，`SkywalkerFramework::Stop` 末尾日志不打印。
- 根因：`SFPluginManager::Release` 生命周期顺序不当，导致对象释放与动态库卸载时序冲突；同时需要避免在 `Release` 中过早触发 `delete this` 后继续访问成员。
- 修复：
	- 在插件管理器中增加 `StopPlugin()`，通过每个动态库导出函数 `DllStopPlugin` 先完成插件反注册。
	- `Release` 调整为：先 `StopPlugin`，再清理插件映射，再清理 `DynamicLibMap` 卸载动态库，最后调用 `SSFObjectManager::Release`。
	- 关键原则：`SSFObjectManager::Release` 可能触发 `delete this`，必须放在最后。
- 关联提交：`9989ebe`（`fix: 修复插件管理器释放顺序导致的关服卡住问题`）。

### 动态库封装跨平台收敛
- 现象：动态库卸载返回值在 Windows/Linux 语义不同，业务层写平台分支会扩散。
- 修复：将卸载成功判定下沉到平台头文件宏 `SKYWALKER_DYNAMIC_LIB_UNLOAD_SUCCESS(Result)`，业务层统一调用。
- 关联提交：`b10e2c7`（`refactor: 优化SFDynamicLib并下沉跨平台卸载语义到平台层`）。
