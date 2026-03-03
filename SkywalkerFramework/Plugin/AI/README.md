# SSFPlugin_AI

## 插件定位
AI 运行时插件，负责 AI 策略调度、预算控制与策略审计能力。

## 当前模块
- `SSFModule_AIRuntime`

## 当前能力
- Tick 预算控制：支持 `AITickBudgetMS`。
- 策略切换：`strict` / `balanced` / `relaxed`。
- 审计能力：策略切换审计查询与清理。
- 网关接入：通过 `SSFGameplayServiceGateway` 暴露 AI 统计、策略列表、审计相关回调。

## 关键配置
- `Bin/Server/ServerConfig.skywalkerC`
	- `AITickBudgetMS`
	- `AIStrategy`

## API接口说明
- 运行时接口（`SSFModule_AIRuntime`）：`SetTickBudgetMS`、`GetTickBudgetMS`、`GetBudgetExceededCount`。
- 策略接口：`SetStrategy`、`GetStrategy`、`BuildStrategies`。
- 统计与审计接口：`BuildStats`、`BuildAudit`、`ClearAudit`。
- 网关对外能力（由 `SSFGameplayServiceGateway` 暴露）：`SetAIStrategy`、`GetAIStats`、`GetAIStrategies`、`GetAIAudit`、`ClearAIAudit`。

## 关键代码入口
- `SSFPlugin_AI.cpp`
- `Module/SSFModule_AIRuntime.h`
- `Module/SSFModule_AIRuntime.cpp`
