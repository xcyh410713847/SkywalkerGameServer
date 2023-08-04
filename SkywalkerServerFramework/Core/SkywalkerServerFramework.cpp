/*************************************************************************
**文件: SkywalkerServerFramework\Core\SkywalkerServerFramework.cpp
**作者: shyfan
**日期: 2023/08/03 11:46:37
**功能: 核心
*************************************************************************/

#include <stdlib.h>

#include "Include\SSFCore.h"

#include "Include\SSFIPluginManager.h"
#include "SSFPluginManager.h"

SKYWALKER_SF_NAMESPACE_USE

static SSFSharedPtr_IPluginManager G_PluginManager;

bool SkywalkerServerFramework_Start()
{
    std::cout << "Hello SkywalkerServerFramework" << std::endl;

    G_PluginManager = new SSFCPluginManager();

    return true;
}

bool SkywalkerServerFramework_Tick()
{
    // TODO Shyfan 是否需要结束

    G_PluginManager->Tick();

    std::cout << "Tick SkywalkerServerFramework" << std::endl;

    return true;
}

void SkywalkerServerFramework_Stop()
{
    G_PluginManager->Stop();
    SKYWALKER_SF_RELEASE(G_PluginManager);

    std::cout << "Bye SkywalkerServerFramework" << std::endl;
}

SSFSharedPtr_IPluginManager SkywalkerServerFramework_GetPluginManager()
{
    return G_PluginManager;
}
