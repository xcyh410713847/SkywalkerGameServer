/*************************************************************************
**文件: SkywalkerGameServer\Executable\SkywalkerGameServer.cpp
**作者: shyfan
**日期: 2023/08/03 11:19:47
**功能: 入口
*************************************************************************/

#include <stdio.h>

#include "SkywalkerServerFramework\SkywalkerServerFramework.h"

SKYWALKER_SF_NAMESPACE_USE

int main(int argc, char *argv[])
{
    CSkywalkerServerFramework *pSkywalkerServerFramework = new CSkywalkerServerFramework();

    if (!pSkywalkerServerFramework->Start())
    {
        return 1;
    }

    while (pSkywalkerServerFramework->IsRunning())
    {
        pSkywalkerServerFramework->Tick();
    }

    pSkywalkerServerFramework->Stop();

    return 0;
}
