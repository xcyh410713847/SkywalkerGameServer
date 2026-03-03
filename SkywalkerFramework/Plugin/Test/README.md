# SSFPlugin_Test

## 插件定位
框架测试与示例插件，用于承载功能验证与插件开发示例。

## 当前模块
- `SSFModule_TestOne`
- `SSFModule_TestTwo`
- `SSFModule_TestThree`

## 当前用途
- 作为插件机制示例，演示模块注册/反注册与基础生命周期。
- 配合测试工程做局部功能验证。

## API接口说明
- 当前以示例模块与生命周期演示为主，未定义稳定业务 API。
- 对外可复用能力主要是插件/模块开发参考（注册、反注册、生命周期组织方式）。
- 若后续需要将测试能力产品化，建议在此插件新增明确的查询或调试接口并同步到核心网关。

## 关键代码入口
- `SSFPlugin_Test.cpp`
- `Module/SSFModule_TestOne.h`
- `Module/SSFModule_TestTwo.h`
- `Module/SSFModule_TestThree.h`
- `SSFTestExample.h`
