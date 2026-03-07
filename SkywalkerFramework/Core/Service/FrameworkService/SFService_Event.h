/*************************************************************************
**文件: SkywalkerFramework\Core\Service\FrameworkService\SFService_Event.h
**作者: shyfan
**日期: 2024/01/22 16:03:19
**功能: SSFService_Event
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFService_Event_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFService_Event_H__

#include "Core/Service/FrameworkService/SFFrameworkService.h"

SF_NAMESPACE_BEGIN

/**
 * 事件服务
 * 说明：提供框架级事件系统能力入口。
 */
class SSFService_Event : public SSFFrameworkService
{
public:
    /** 构造函数 */
    SSFService_Event(SSFServiceContext &InContext, SFObjectErrors &InErrors);
    /** 析构函数 */
    virtual ~SSFService_Event();

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
     * 说明：释放事件服务相关资源。
     */
    virtual void Destroy(SFObjectErrors &InErrors) override;

#pragma endregion Process
};

SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFService_Event_H__
