/*************************************************************************
**文件: SkywalkerGameServer\SkywalkerTestMain.cpp
**作者: shyfan
**日期: 2026/02/26
**功能: 测试入口
*************************************************************************/

#include "SkywalkerTest/SkywalkerTest.h"
#include "SkywalkerTest/SkywalkerToolsTest.h"
#include "SkywalkerTest/SkywalkerPlatformTest.h"
#include "SkywalkerServerFramework/Plugin/Test/SSFTestExample.h"

int main()
{
    return SkywalkerTestRunner::Instance().RunAll();
}
