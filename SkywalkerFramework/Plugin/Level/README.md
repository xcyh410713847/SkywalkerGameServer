# SSFPlugin_Level

## 插件定位
世界/房间管理插件，负责在线场景承载。

## 当前模块
- `SSFModule_WorldManager`

## 当前能力
- 世界创建与基础管理。
- 玩家进出世界与玩家计数。
- 通过核心网关参与登录后进世界、断线离场流程。

## API接口说明
- 世界管理接口（`SSFModule_WorldManager`）：`CreateWorld`、`EnterWorld`、`LeaveWorld`、`GetWorldPlayerCount`。
- 网关对外能力（由 `SSFGameplayServiceGateway` 暴露）：`EnterWorld`、`LeaveWorld`。
- 典型调用链：Network 登录成功 -> 网关 `EnterWorld`；连接断开 -> 网关 `LeaveWorld`。

## 关键代码入口
- `SSFPlugin_Level.cpp`
- `Module/SSFModule_WorldManager.h`
- `Module/SSFModule_WorldManager.cpp`
