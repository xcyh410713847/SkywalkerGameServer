# SkywalkerPlatform

## 功能
- 统一跨平台基础能力封装（Windows / Linux / Mac）。
- 屏蔽动态库加载、平台名、路径分隔符、环境变量设置等差异。

## 主要内容
- 平台识别宏：`SKYWALKER_PLATFORM_WINDOWS/LINUX/MAC`
- 动态库宏：
  - `SKYWALKER_DYNAMIC_LIB_LOAD`
  - `SKYWALKER_DYNAMIC_LIB_UNLOAD`
  - `SKYWALKER_DYNAMIC_LIB_GET_SYMBOL`
- 通用能力：
  - `SKYWALKER_PLATFORM_NAME`
  - `SKYWALKER_PATH_SEPARATOR`
  - `SKYWALKER_THREAD_LOCAL`
  - `SkywalkerSetEnv(...)`
  - `SkywalkerInetPton(...)`

## 使用示例
```cpp
SkywalkerSetEnv(SF_ENV_PROGRAM_CONFIG_DIR, "Bin/Server/ServerConfig.skywalkerC");
```

## 注意事项
- `SKYWALKER_PLATFORM_SLEEP(ms)` 在 Linux/Mac 下底层是 `usleep`（微秒）；调用方应确认单位一致性。
