/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SFService.h
**作者: shyfan
**日期: 2024/01/16 21:10:23
**功能: SSFService
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFService_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFService_H__

#include "Include/SFCore.h"

#include "Core/Object/SFObject.h"

SF_NAMESPACE_BEGIN

/**
 * 服务上下文
 * 说明：服务创建时传入的上下文基类，可由具体服务扩展。
 */
struct SSFServiceContext : public SSFObjectContext
{
};

/**
 * 服务基类接口
 *
 * 职责：
 * 1. 约束服务生命周期（Init/Start/Tick/Stop/Destroy）
 * 2. 供服务管理器统一调度
 */
class SSFService : public SSFObject
{
public:
    /** 构造函数 */
    SSFService(SSFServiceContext &InContext, SFObjectErrors &InErrors);
    /** 析构函数 */
    virtual ~SSFService();

#pragma region Process

public:
    /**
     * 初始化
     * @return true 初始化成功；false 初始化失败
     */
    virtual bool Init(SFObjectErrors &InErrors) = 0;

    /**
     * 启动
     * @return true 启动成功；false 启动失败
     */
    virtual bool Start(SFObjectErrors &InErrors) = 0;

    /**
     * 每帧更新
     * @return true 继续运行；false 请求结束或暂停
     */
    virtual bool Tick(SFObjectErrors &InErrors) = 0;

    /**
     * 停止
     * @return true 停止成功；false 停止失败
     */
    virtual bool Stop(SFObjectErrors &InErrors) = 0;

    /**
     * 销毁
     * 说明：用于释放服务持有的内部资源。
     */
    virtual void Destroy(SFObjectErrors &InErrors) = 0;

#pragma endregion Process
};

SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFService_H__
