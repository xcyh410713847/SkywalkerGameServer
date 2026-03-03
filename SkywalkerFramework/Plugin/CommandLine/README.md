# SSFPlugin_CommandLine

## 插件定位
管理命令与运维操作插件，提供服务器在线命令入口。

## 当前模块
- `SSFModule_OS`
- `SSFModule_AdminCommand`

## 当前能力
- 角色权限：`admin` / `operator` / `observer`。
- ACL 配置化：支持从 `ServerConfig.skywalkerC` 读取并通过 `reload_acl` 热重载。
- 命令能力：
	- AI：`show_ai_stats`、`show_ai_strategies`、`show_ai_audit`、`show_ai_audit_stats`、`set_ai_strategy`、`clear_ai_audit`
	- Replay：`show_replay_stats`、`show_replay_query_stats`、`show_replay_event`、`show_replay_events`、`find_replay_events`
	- ACL 统计：`show_admin_acl_stats`
- 统计能力：执行成功/拒绝/失败计数。

## 关键配置
- `AdminACL_Admin`
- `AdminACL_Operator`
- `AdminACL_Observer`

## API接口说明
- 对外执行入口（`SSFModule_AdminCommand`）：`ExecuteCommand(const SFString &CommandLine)`。
- ACL 相关内部接口：`ReloadACLFromConfig`、`HasPermission`、`ParseRoleAndCommand`（用于权限判断与角色解析）。
- AI 命令接口：`show_ai_stats`、`show_ai_strategies`、`show_ai_audit`、`show_ai_audit_stats`、`set_ai_strategy`、`clear_ai_audit`。
- Replay 命令接口：`show_replay_stats`、`show_replay_query_stats`、`show_replay_event`、`show_replay_events`、`find_replay_events`。
- ACL 统计与运维命令：`show_admin_acl_stats`、`reload_acl`。

## 关键代码入口
- `SSFPlugin_CommandLine.cpp`
- `Module/SSFModule_AdminCommand.h`
- `Module/SSFModule_AdminCommand.cpp`
