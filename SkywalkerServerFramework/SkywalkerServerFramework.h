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
    bool Tick();
    void Stop();

    bool IsRunning() const;
    void SetRunning(bool Running);

private:
    bool Running;
};

SKYWALKER_SF_NAMESPACE_END

#define SKYWALKER_SERVER_FRAMEWORK_START(argc, argv)                                        \
    CSkywalkerServerFramework *pSkywalkerServerFramework = new CSkywalkerServerFramework(); \
    if (!pSkywalkerServerFramework->Start())                                                \
    {                                                                                       \
        return 1;                                                                           \
    }                                                                                       \
    while (pSkywalkerServerFramework->IsRunning())                                          \
    {                                                                                       \
        pSkywalkerServerFramework->Tick();                                                  \
    }                                                                                       \
    pSkywalkerServerFramework->Stop();                                                      \
    return 0;

#endif // __SKYWALKER_SERVER_FRAMEWORK_H__
