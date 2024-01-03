/*************************************************************************
**文件: SkywalkerServerFramework\Core\Command\SSFCommandLine.h
**作者: shyfan
**日期: 2023/12/23 16:16:42
**功能: 命令行
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFCommandLine_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFCommandLine_H__

#include "Include/SSFCore.h"

#include "SSFOSSignal.h"

SSF_NAMESPACE_BEGIN

/**
 * 命令行
 */
class SSFCommandLine
{
private:
    /**
     * 系统信号处理
     */
    SSFOSSignal OSSignal;
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFCommandLine_H__
