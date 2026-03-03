# SSFPlugin_Video

## 插件定位
回放插件，负责对局录制、文件落盘、回放加载与查询。

## 当前模块
- `SSFModule_ReplayRecorder`
- `SSFModule_ReplayPlayer`

## 当前能力
- 录制：按会话输出 `Session_<id>.replay`。
- 格式：`Magic/Version/SessionId/EventCount/Checksum/EventsBegin`。
- 校验：回放时进行版本与校验和验证。
- 查询：支持按索引、按区间、按关键字检索事件。
- 统计：输出回放查询统计（最近关键字、命中数等）。

## 关键配置
- `ReplayDirectory`（回放文件目录）

## API接口说明
- 录制接口（`SSFModule_ReplayRecorder`）：`StartRecord`、`RecordEvent`、`StopRecord`、`IsRecording`、`SetReplayDirectory`、`BuildStats`。
- 播放接口（`SSFModule_ReplayPlayer`）：`StartReplay`、`StopReplay`、`IsReplaying`、`SetReplayDirectory`、`GetLoadedEventCount`、`BuildStats`。
- 查询接口（`SSFModule_ReplayPlayer`）：`GetEventByIndex`、`GetEventsRange`、`FindEventsByKeyword`。
- 网关对外能力（由 `SSFGameplayServiceGateway` 暴露）：`StartReplayRecord`、`StopReplayRecord`、`StartReplayPlay`、`StopReplayPlay`、`GetReplayRecordStats`、`GetReplayPlayStats`、`GetReplayEventByIndex`、`GetReplayEventsRange`、`FindReplayEventsByKeyword`。

## 关键代码入口
- `SSFPlugin_Video.cpp`
- `Module/SSFModule_ReplayRecorder.h`
- `Module/SSFModule_ReplayRecorder.cpp`
- `Module/SSFModule_ReplayPlayer.h`
- `Module/SSFModule_ReplayPlayer.cpp`
