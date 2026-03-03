# SSFPlugin_Actor

## 插件定位
Actor 运行时插件，负责实体（玩家/NPC）生命周期管理的承载位。

## 当前模块
- `SSFModule_ActorRuntime`

## 当前状态
- 已接入插件注册与构建流程。
- 处于 P1 阶段骨架能力，后续可继续补充实体创建、销毁、Tick 调度与事件派发。

## API接口说明
- 当前对外 API 以模块生命周期接口为主：`Init`、`Start`、`Tick`、`Stop`、`Destroy`。
- 现阶段未通过核心网关暴露独立业务接口（如实体创建/查询），后续可按 Actor 能力扩展新增。

## 关键代码入口
- `SSFPlugin_Actor.cpp`
- `Module/SSFModule_ActorRuntime.h`
- `Module/SSFModule_ActorRuntime.cpp`
