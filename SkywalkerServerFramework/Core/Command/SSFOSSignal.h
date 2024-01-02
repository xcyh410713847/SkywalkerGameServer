/*************************************************************************
**文件: SkywalkerServerFramework\Core\Command\SSFOSSignal.h
**作者: shyfan
**日期: 2023/12/23 12:16:42
**功能: 操作系统信号
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFOSSignal_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFOSSignal_H__

#include "Include/SSFCore.h"

SSF_NAMESPACE_BEGIN

/**
 * 系统信号
 */
class SSFOSSignal
{
public:
    SSFOSSignal();
    ~SSFOSSignal();

    /**
     * 信号处理函数
     * 只能用static修饰，否则不生效
     */
    static void SignalHandler(int Signal);
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFOSSignal_H__
