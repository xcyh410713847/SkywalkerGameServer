# SSFPlugin_Network

## 插件定位
网络通信插件，负责连接接入、协议编解码、会话管理和消息路由。

## 当前模块
- `SSFModule_NetworkServer`
- `SSFModule_NetworkClient`

## 子目录说明
- `Protocol/`：协议头与登录载荷编解码
- `Router/`：消息路由
- `Session/`：会话与黑名单持久化
- `Object/`：网络对象（ServerSocket/ClientSocket）

## 当前能力
- 登录/心跳/玩家输入基础链路。
- 会话超时与异常清理。
- 限流与违规统计。
- 黑名单持久化与热重载。
- 通过核心网关调用 Auth/DB/Level/AI/Replay 等跨插件能力。

## 关键配置
- `IP`、`Port`、`SessionTimeoutMS`
- `MaxMsgPerSecond`、`ViolationThreshold`
- `BlacklistPersistence`、`BlacklistFilePath`
- `BlacklistHotReload`、`BlacklistReloadIntervalMS`

## API接口说明
- 核心网关调用（由 Network 发起跨插件协作）：`ValidateToken`、`LoadPlayer`、`EnterWorld`、`LeaveWorld`。
- Replay 网关调用：`StartReplayRecord`、`StopReplayRecord`、`StartReplayPlay`、`StopReplayPlay`、`GetReplayRecordStats`、`GetReplayPlayStats`、`GetReplayEventByIndex`、`GetReplayEventsRange`、`FindReplayEventsByKeyword`。
- AI 网关调用：`SetAIStrategy`、`GetAIStats`、`GetAIStrategies`、`GetAIAudit`、`ClearAIAudit`。
- 网络客户端公开接口（`SSFModule_NetworkClient`）：`Connect`、`Disconnect`、`IsConnected`、`Send`、`SendPacket`。
- 网络服务端模块（`SSFModule_NetworkServer`）当前以插件生命周期与内部路由处理为主，未额外开放独立业务 API。

## 关键代码入口
- `SSFPlugin_Network.cpp`
- `Module/SSFModule_NetworkServer.h`
- `Module/SSFModule_NetworkServer.cpp`
- `Protocol/SSFNetworkCodec.h`
- `Session/SSFNetworkSessionManager.h`
