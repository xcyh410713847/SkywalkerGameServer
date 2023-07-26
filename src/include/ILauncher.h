/*************************************************************************
**文件: ILauncher.h
**作者: shyfan
**日期: 2023/07/25 18:18:17
**功能: 启动器接口
*************************************************************************/

#ifndef __SKYWALKER_GS_INCLUDE_I_LAUNCHER_H__
#define __SKYWALKER_GS_INCLUDE_I_LAUNCHER_H__

#include "Errors.h"

struct ILauncher
{
    /**
     * 启动
     * @param LauncherErrors &Errors	: 错误
     * @return void
     */
    virtual void LaunchServer(LauncherErrors &Errors) = 0;

    /**
     * 关闭
     * @param LauncherErrors &Errors	: 错误
     * @return void
     */
    virtual void CloseServer(LauncherErrors &Errors) = 0;
};

#endif // __SKYWALKER_GS_INCLUDE_I_LAUNCHER_H__
