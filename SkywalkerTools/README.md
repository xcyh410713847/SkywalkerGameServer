# SkywalkerTools

`SkywalkerTools` 提供了一组“功能单一、低耦合、可跨项目复用”的基础工具。

## 设计目标

- 单一职责：每个目录聚焦一类问题。
- 低耦合：工具之间尽量独立，按需引入。
- 可迁移：尽量只依赖 C++ 标准库与少量平台头。

## 目录说明

### SkywalkerDerived

- 作用：编译期继承关系检测。
- 关键能力：`SKYWALKER_IS_DERIVED(DerivedType, BaseType)`。
- 场景：宏约束、模板约束、静态检查。

### SkywalkerErrors

- 作用：错误收集与追踪。
- 关键类型：`CSkywalkerErrors<T>`。
- 关键宏：
	- `SKYWALKER_ERRORS_WRAP`：仅记录错误。
	- `SKYWALKER_ERRORS_WRAP_TRACE`：记录错误 + 文件行号 + 函数。

### SkywalkerEvent

- 作用：轻量事件总线（静态注册/触发）。
- 关键能力：
	- 事件 ID 组合与拆分（MainID/SubID）。
	- 多阶段执行顺序（Before/During/After）。

### SkywalkerPath

- 作用：路径解析与配置路径标准化（可独立复用）。
- 关键函数：
	- `SkywalkerAbsolutePath`
	- `SkywalkerResolveConfigPath`
- 推荐场景：可执行入口、配置加载、脚本路径解析。

### SkywalkerPlatform

- 作用：平台差异统一封装。
- 内容包括：
	- 平台识别宏（Windows/Linux/Mac）。
	- 动态库加载宏。
	- 路径分隔符、线程局部变量、休眠等基础能力。
	- `SkywalkerSetEnv`、`SkywalkerInetPton` 统一接口。

### SkywalkerPool

- 作用：简单对象池。
- 关键类型：`CSkywalkerPool<T>`。
- 注意：`Get()` 前需确保池中已有对象。

### SkywalkerSingleton

- 作用：单例声明/实现宏。
- 关键宏：
	- `SKYWALKER_SINGLETON_DECLARE`
	- `SKYWALKER_SINGLETON_IMPLEMENT`

### SkywalkerTimer

- 作用：高精度帧计时与时间字符串格式化。
- 关键能力：
	- `Reset()` / `Tick()`
	- `GetDeltaTime()` / `GetTotalTime()`

### SkywalkerTest

- 作用：工具层测试与断言支持。
- 说明：用于验证 `SkywalkerTools` 中各工具的基本行为。

## 使用建议

- 仅引入所需目录头文件，避免把全部工具耦合进业务模块。
- 对跨项目复用场景，优先选用 `SkywalkerPath` + `SkywalkerPlatform` + `SkywalkerErrors` 组合。
- 对生命周期敏感场景，使用 `SkywalkerPool` 与 `SkywalkerSingleton` 时需明确所有权。
