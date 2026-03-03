# SFPlugin_DB

## 插件定位
数据访问与玩家数据仓储插件。

## 当前模块
- `SFModule_DBAccess`
- `SFModule_PlayerRepository`

## 当前能力
- 提供数据库访问层骨架与玩家数据加载/保存承载位。
- 通过核心网关参与登录链路中的玩家数据加载流程。

## API接口说明
- 访问层接口（`SFModule_DBAccess`）：`IsReady() const`。
- 仓储接口（`SFModule_PlayerRepository`）：`LoadPlayer(SFUInt64 PlayerId)`、`SavePlayer(SFUInt64 PlayerId)`。
- 网关对外能力（由 `SSFGameplayServiceGateway` 暴露）：`LoadPlayer(SFUInt64 PlayerId)`。
- 典型调用链：Network 登录处理 -> 网关 `LoadPlayer` -> DB 仓储加载玩家数据。

## 关键代码入口
- `SFPlugin_DB.cpp`
- `Module/SFModule_DBAccess.h`
- `Module/SFModule_DBAccess.cpp`
- `Module/SFModule_PlayerRepository.h`
- `Module/SFModule_PlayerRepository.cpp`
