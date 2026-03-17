# SFPlugin_Network

## 插件定位
网络通信插件，负责连接接入、协议编解码、会话管理和消息路由。

## 当前模块
- `SFModule_NetworkServer`
- `SFModule_NetworkClient`

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

## 关键配置
- `IP`、`Port`、`SessionTimeoutMS`
- `MaxMsgPerSecond`、`ViolationThreshold`
- `BlacklistPersistence`、`BlacklistFilePath`
- `BlacklistHotReload`、`BlacklistReloadIntervalMS`

## API接口说明
- 网络客户端公开接口（`SFModule_NetworkClient`）：`Connect`、`Disconnect`、`IsConnected`、`Send`、`SendPacket`。
- 网络服务端模块（`SFModule_NetworkServer`）当前以插件生命周期与内部路由处理为主，未额外开放独立业务 API。

## 关键代码入口
- `SFPlugin_Network.cpp`
- `Module/SFModule_NetworkServer.h`
- `Module/SFModule_NetworkServer.cpp`
- `Protocol/SFNetworkCodec.h`
- `Session/SFNetworkSessionManager.h`
