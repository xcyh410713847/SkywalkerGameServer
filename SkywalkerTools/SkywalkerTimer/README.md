# SkywalkerTimer

## 功能
- 提供高精度计时能力，适用于帧循环、性能采样和日志时间戳输出。

## 主要接口
- `Reset()`：重置起点。
- `Tick()`：刷新当前帧耗时。
- `GetDeltaTime()`：获取帧间隔（毫秒）。
- `GetTotalTime()`：获取累计运行时间（秒）。
- `GetCurrTimeStr()`：获取格式化当前时间字符串。

## 使用示例
```cpp
Skywalker::Timer::SkywalkerTimer Timer;
Timer.Reset();
while (Running)
{
    Timer.Tick();
    auto DtMs = Timer.GetDeltaTime();
}
```

## 注意事项
- `GetDeltaTime()` 为整数毫秒，亚毫秒精度会被截断。
- GMT 时间接口与单调时钟接口语义不同，使用时注意区分。
