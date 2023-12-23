/*************************************************************************
**文件: SkywalkerServerFramework\Core\Command\SSFSystemSignal.h
**作者: shyfan
**日期: 2023/12/23 12:16:42
**功能: 系统信号
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFSystemSignal_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFSystemSignal_H__

#include <csignal>

#include "Include/SSFCore.h"

SSF_NAMESPACE_BEGIN

/**
 * 系统信号
 */
class SSFSystemSignal
{
public:
    SSFSystemSignal();
    ~SSFSystemSignal();

    /**
     * 信号处理函数
     * 只能用static修饰，否则不生效
     */
    static void SignalHandler(int Signal);
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFSystemSignal_H__
