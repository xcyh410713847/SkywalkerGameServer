/*************************************************************************
**文件: SkywalkerFramework\Plugin\Auth\Module\SFModule_Auth.h
**作者: shyfan
**日期: 2026/03/19
**功能: 登录鉴权模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_AUTH_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_AUTH_H__

#include "Include/SFCore.h"
#include "Include/SFNetworkInterface.h"

#include "Core/Module/SFModule.h"

SF_NAMESPACE_BEGIN

/**
 * 鉴权模块
 *
 * 职责：
 * 1. 在 Init 阶段通过 ISFNetworkServer 接口注册 LoginReq Handler
 * 2. 验证 PlayerId + Token（Demo: 硬编码白名单）
 * 3. 验证成功 → 设置 Session 为已认证 + 回复 LoginResp(Success)
 * 4. 验证失败 → 回复 LoginResp(Failed) + 关闭 Session
 */
class SFModule_Auth : public SSFModule
{
#pragma region Object

public:
    virtual void Init(SFObjectErrors &Errors) override;
    virtual void Awake(SFObjectErrors &Errors) override;
    virtual void Start(SFObjectErrors &Errors) override;
    virtual void Tick(SFObjectErrors &Errors, SFUInt64 DelayMS) override;
    virtual void Stop(SFObjectErrors &Errors) override;
    virtual void Sleep(SFObjectErrors &Errors) override;
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SFModule_Auth(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors), NetworkServer(nullptr)
    {
    }
    virtual ~SFModule_Auth() {};

private:
    /** 通过 PluginManager 获取网络服务接口 */
    ISFNetworkServer *FindNetworkServer();

    /** 处理登录请求 */
    void HandleLoginReq(SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen);

    /** 验证 Token (Demo: 简单白名单) */
    bool ValidateToken(SFUInt32 PlayerId, const SFString &Token);

private:
    ISFNetworkServer *NetworkServer;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_MODULE_AUTH_H__ */
