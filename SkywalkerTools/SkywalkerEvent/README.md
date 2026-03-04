# SkywalkerEvent

## 功能
- 轻量事件系统，支持按 `MainID + SubID` 组织事件。
- 支持多个执行阶段（Before/During/After）。

## 主要接口
- `CSkywalkerEvent::RegisterEvent(...)`
- `CSkywalkerEvent::UnRegisterEvent(...)`
- `CSkywalkerEvent::TriggerEvent(...)`

## 便捷宏
- `SKYWALKER_REGISTER_EVENT`
- `SKYWALKER_REGISTER_EVENT_ORDER`
- `SKYWALKER_UNREGISTER_EVENT`
- `SKYWALKER_TRIGGER_EVENT`

## 使用示例
```cpp
SKYWALKER_REGISTER_EVENT(1, 100, OnLogin);
MyParam Param{};
SKYWALKER_TRIGGER_EVENT(1, 100, Param);
SKYWALKER_UNREGISTER_EVENT(1, 100, OnLogin);
```

## 注意事项
- 当前实现为静态全局表，适合单进程内事件分发。
- 回调注销为“标记失效”语义，若后续需强一致移除可扩展清理策略。
