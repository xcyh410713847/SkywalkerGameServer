/*************************************************************************
**文件: SkywalkerServerFramework\Core\SkywalkerServerFramework.h
**作者: shyfan
**日期: 2023/08/07 15:54:41
**功能: 核心
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_H__
#define __SKYWALKER_SERVER_FRAMEWORK_H__

#include "Include\SSFCore.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class CSkywalkerServerFramework
{
public:
    CSkywalkerServerFramework();
    ~CSkywalkerServerFramework();

    bool Start();
    bool IsRunning() const;
    bool Tick();
    void Stop();

private:
    bool Running;
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_H__
