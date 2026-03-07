/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SFLevelService.h
**作者: shyfan
**日期: 2024/01/22 12:28:06
**功能: SSFLevelService
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFLevelService_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFLevelService_H__

#include "Core/Service/SFService.h"

SF_NAMESPACE_BEGIN

/**
 * 关卡级服务基类
 * 说明：用于承载关卡/场景域的服务能力。
 */
class SSFLevelService : public SSFService
{
public:
    /** 构造函数 */
    SSFLevelService(SSFServiceContext &InContext, SFObjectErrors &InErrors);
    /** 析构函数 */
    virtual ~SSFLevelService();

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
     * 说明：释放关卡服务相关资源。
     */
    virtual void Destroy(SFObjectErrors &InErrors) override;

#pragma endregion Process
};

SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFLevelService_H__
