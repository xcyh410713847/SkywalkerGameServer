/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SFFrameworkService.h
**作者: shyfan
**日期: 2024/01/22 12:29:38
**功能: SSFFrameworkService
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFFrameworkService_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFFrameworkService_H__

#include "Core/Service/SFService.h"

SF_NAMESPACE_BEGIN

/**
 * 框架级服务基类
 * 说明：用于承载框架公共能力（计时、事件等）的服务实现。
 */
class SSFFrameworkService : public SSFService
{
public:
    /** 构造函数 */
    SSFFrameworkService(SSFServiceContext &InContext, SFObjectErrors &InErrors);
    /** 析构函数 */
    virtual ~SSFFrameworkService();

#pragma region Process

public:
    /**
     * 初始化
     * @return true 成功；false 失败
     */
    virtual bool Init(SFObjectErrors &InErrors) override;

    /**
     * 启动
     * @return true 成功；false 失败
     */
    virtual bool Start(SFObjectErrors &InErrors) override;

    /**
     * 每帧更新
     * @return true 继续运行；false 结束
     */
    virtual bool Tick(SFObjectErrors &InErrors) override;

    /**
     * 停止
     * @return true 成功；false 失败
     */
    virtual bool Stop(SFObjectErrors &InErrors) override;

    /**
     * 销毁
     * 说明：释放服务持有资源。
     */
    virtual void Destroy(SFObjectErrors &InErrors) override;

#pragma endregion Process
};

SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFFrameworkService_H__
