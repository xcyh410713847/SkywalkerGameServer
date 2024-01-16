/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\CommandLine\Module\SSFModule_OS.h
**作者: shyfan
**日期: 2024/01/16 14:45:30
**功能: 操作系统模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFModule_OS_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFModule_OS_H__

#include "Core/Module/SSFModule.h"

SSF_NAMESPACE_BEGIN

class SSFModule_OS : public SSFOModule
{
    SSF_OBJECT_CLASS(SSFModule_OS)

#pragma region Module

    virtual void Init(SSFObjectErrors &Errors) override;

    virtual void Destroy(SSFObjectErrors &Errors) override;

#pragma endregion Module

public:
    SSFModule_OS(SSFModuleContext &InContext, SSFObjectErrors &InErrors);
    virtual ~SSFModule_OS(){};

    /**
     * 信号处理函数
     * 只能用static修饰，否则不生效
     */
    static void SignalHandler(int Signal);
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFModule_OS_H__
