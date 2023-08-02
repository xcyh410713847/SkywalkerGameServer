/*************************************************************************
**文件: Src\Module\Launcher\Launcher.h
**作者: shyfan
**日期: 2023/07/30 12:41:36
**功能: 启动器
*************************************************************************/

#ifndef __SKYWALKER_GS_MODULE_LAUNCHER_H__
#define __SKYWALKER_GS_MODULE_LAUNCHER_H__

#include "ModuleExporter.h"
#include "ILauncher.h"

class DLL_EXPORT CLauncher : public ILauncher
{
public:
    CLauncher();
    virtual ~CLauncher();

#pragma region ILauncher

public:
    /**
     * 启动
     * @param LauncherErrors &Errors	: 错误
     * @return void
     */
    virtual void LaunchServer(LauncherErrors &Errors) override;

    /**
     * 关闭
     * @param LauncherErrors &Errors	: 错误
     * @return void
     */
    virtual void CloseServer(LauncherErrors &Errors) override;

#pragma endregion // ILauncher
};

#endif // __SKYWALKER_GS_MODULE_LAUNCHER_H__
