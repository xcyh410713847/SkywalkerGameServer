/*************************************************************************
**文件: SkywalkerFramework\Plugin\CommandLine\Module\SFModule_OS.h
**作者: shyfan
**日期: 2024/01/16 14:45:30
**功能: 操作系统模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFModule_OS_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFModule_OS_H__

#include "Core/Module/SFModule.h"

SF_NAMESPACE_BEGIN

class SFModule_OS : public SSFModule
{
#pragma region Module

    virtual void Init(SFObjectErrors &Errors) override;

    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Module

public:
    SFModule_OS(SFModuleContext &InContext, SFObjectErrors &InErrors);
    virtual ~SFModule_OS() {};

    /**
     * 信号处理函数
     * 只能用static修饰，否则不生效
     */
    static void SignalHandler(int Signal);
};

SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFModule_OS_H__
