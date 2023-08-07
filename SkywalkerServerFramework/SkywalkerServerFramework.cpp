/*************************************************************************
**文件: SkywalkerServerFramework\Core\SkywalkerServerFramework.cpp
**作者: shyfan
**日期: 2023/08/03 11:46:37
**功能: 核心
*************************************************************************/

#include "SkywalkerServerFramework.h"

#include <iostream>

SKYWALKER_SF_NAMESPACE_USE

CSkywalkerServerFramework::CSkywalkerServerFramework()
    : Running(false)
{
}

CSkywalkerServerFramework::~CSkywalkerServerFramework()
{
}

bool CSkywalkerServerFramework::Start()
{
    std::cout << "Hello SkywalkerServerFramework" << std::endl;

    Running = true;

    return true;
}

bool CSkywalkerServerFramework::IsRunning() const
{
    return Running;
}

bool CSkywalkerServerFramework::Tick()
{
    if (!IsRunning())
    {
        return false;
    }

    std::cout << "Tick SkywalkerServerFramework" << std::endl;

    // TODO Shyfan 临时
    SKYWALKER_PLATFORM_SLEEP(1000);

    return true;
}

void CSkywalkerServerFramework::Stop()
{
    std::cout << "Bye SkywalkerServerFramework" << std::endl;
}
