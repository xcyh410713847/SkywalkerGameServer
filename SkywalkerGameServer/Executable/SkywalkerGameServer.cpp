/*************************************************************************
**文件: SkywalkerGameServer\Executable\SkywalkerGameServer.cpp
**作者: shyfan
**日期: 2023/08/03 11:19:47
**功能: 入口
*************************************************************************/

#include <stdio.h>

#include "SkywalkerServerFramework\Include\SSFCore.h"
#include "SkywalkerServerFramework\Include\SSFIPluginManager.h"

#include "SGSILauncher.h"

SKYWALKER_SF_NAMESPACE_USE

int main(int argc, char *argv[])
{
    SkywalkerServerFramework_Start();

    bool bTick = true;
    while (bTick)
    {
        bTick = SkywalkerServerFramework_Tick();
    }

    SkywalkerServerFramework_Stop();

    return 0;
}
