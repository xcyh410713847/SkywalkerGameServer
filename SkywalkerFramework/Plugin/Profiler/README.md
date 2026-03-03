# SFPlugin_Profiler

## 插件定位
性能观测与健康检查插件。

## 当前模块
- `SFModule_Metrics`
- `SFModule_HealthCheck`

## 当前状态
- 已接入插件注册与构建流程。
- 处于 P1 观测能力骨架阶段，可继续扩展指标采集与健康探针输出。

## API接口说明
- 健康检查接口（`SFModule_HealthCheck`）：`IsHealthy() const`。
- 指标模块（`SFModule_Metrics`）当前以内部 Tick 采集为主，暂未开放独立查询 API。
- 现阶段未通过核心网关暴露 Profiler 相关对外接口，后续可按运维需求补充。

## 关键代码入口
- `SFPlugin_Profiler.cpp`
- `Module/SFModule_Metrics.h`
- `Module/SFModule_HealthCheck.h`
