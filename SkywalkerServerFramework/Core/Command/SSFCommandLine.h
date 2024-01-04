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

#include "Core/Object/SSFObject.h"

SSF_NAMESPACE_BEGIN

/**
 * 命令行
 */
class SSFCommandLine : public SSFObject
{
    SSF_OBJECT_CLASS();

public:
    SSFCommandLine(SSFObjectContext &InContext, SSFObjectErrors &InErrors)
        : SSFObject(InContext, InErrors){};
    virtual ~SSFCommandLine(){};

private:
    /**
     * 系统信号处理
     */
    SSFOSSignal OSSignal;
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFCommandLine_H__
