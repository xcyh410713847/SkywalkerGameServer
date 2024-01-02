/*************************************************************************
**文件: SkywalkerServerFramework\Core\Command\SSFOSSignal.cpp
**作者: shyfan
**日期: 2023/12/23 12:22:48
**功能: 操作系统信号
*************************************************************************/

#include "SSFOSSignal.h"

#include <csignal>

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFOSSignal, LogLevel_Debug);

SSFOSSignal::SSFOSSignal()
{
    signal(SIGINT, &SSFOSSignal::SignalHandler);
}

SSFOSSignal::~SSFOSSignal()
{
}

void SSFOSSignal::SignalHandler(int Signal)
{
    SSF_LOG_DEBUG("SignalHandler Signal: " << Signal);

    // TODO Shyfan 通知框架停止
}
