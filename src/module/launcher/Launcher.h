/*************************************************************************
**文件: Launcher.h
**作者: shyfan
**日期: 2023/07/25 20:50:32
**功能: 启动器
*************************************************************************/

#ifndef __LAUNCHER_H__
#define __LAUNCHER_H__

#include "ILauncher.h"

class CLauncher : public ILauncher
{
public:
    CLauncher();
    virtual ~CLauncher();
};

#endif // __LAUNCHER_H__
