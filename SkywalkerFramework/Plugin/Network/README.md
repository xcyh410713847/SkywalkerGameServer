# SFPlugin_Network

## 插件定位
网络通信插件，负责连接接入、协议编解码和消息路由。

## 当前模块
- `SFModule_NetworkServer`
- `SFModule_NetworkClient`

## 子目录说明
- `Protocol/`：协议头与登录载荷编解码
- `Object/`：网络对象（ServerSocket/ClientSocket）

## 当前能力
- 基础网络通信（接受连接、收发数据包）。
- 协议编解码（登录载荷、心跳、玩家输入）。

## 关键配置
- `IP`、`Port`

## API接口说明
- 网络客户端公开接口（`SFModule_NetworkClient`）：`Connect`、`Disconnect`、`IsConnected`、`Send`、`SendPacket`。
- 网络服务端模块（`SFModule_NetworkServer`）当前以插件生命周期与内部路由处理为主。

## 关键代码入口
- `SFPlugin_Network.cpp`
- `Module/SFModule_NetworkServer.h`
- `Module/SFModule_NetworkServer.cpp`
- `Protocol/SFNetworkCodec.h`
