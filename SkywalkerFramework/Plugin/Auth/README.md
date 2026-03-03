# SSFPlugin_Auth

## 插件定位
认证插件，负责鉴权入口与认证回调承载。

## 当前模块
- `SSFModule_Auth`

## 当前能力
- 通过核心网关提供 Token 校验能力入口。
- 作为登录链路中的鉴权节点，与 Network 登录流程解耦协作。

## API接口说明
- 模块接口（`SSFModule_Auth`）：`ValidateToken(const SFString &Token) const`。
- 网关对外能力（由 `SSFGameplayServiceGateway` 暴露）：`ValidateToken(const SFString &Token)`。
- 典型调用链：Network 登录处理 -> 网关 `ValidateToken` -> Auth 插件校验结果返回。

## 关键代码入口
- `SSFPlugin_Auth.cpp`
- `Module/SSFModule_Auth.h`
- `Module/SSFModule_Auth.cpp`
