/*************************************************************************
**文件: SkywalkerServerFramework\Core\Command\SSFSystemSignal.cpp
**作者: shyfan
**日期: 2023/12/23 12:22:48
**功能: 系统信号
*************************************************************************/

#include "SSFSystemSignal.h"

#include "Include/SSFILog.h"
#include <functional>

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFSystemSignal, LogLevel_Debug);

SSFSystemSignal::SSFSystemSignal()
{
    signal(SIGINT, &SSFSystemSignal::SignalHandler);
}

SSFSystemSignal::~SSFSystemSignal()
{
}

void SSFSystemSignal::SignalHandler(int Signal)
{
    SSF_LOG_DEBUG("SignalHandler Signal: " << Signal);

    // TODO Shyfan 通知框架停止
}
