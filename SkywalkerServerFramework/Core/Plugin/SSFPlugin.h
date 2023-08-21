/*************************************************************************
**文件: SkywalkerServerFramework\Core\Plugin\SSFPlugin.h
**作者: shyfan
**日期: 2023/08/07 23:53:05
**功能: 插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__

#include "Include/SSFCore.h"

#include "Core/Object/SSFObject.h"
#include "Core/Map/SSFMap.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFOPlugin
    : public SSFObject
{
#pragma region Object

public:
    /**
     * 初始化
     */
    virtual void Init(SSFObjectErrors &Errors) override;

    /**
     * 唤醒
     */
    virtual void Awake(SSFObjectErrors &Errors) override;

    /**
     * 开始
     */
    virtual void Start(SSFObjectErrors &Errors) override;

    /**
     * Tick
     */
    virtual void Tick(SSFObjectErrors &Errors, int DelayMS) override;

    /**
     * 结束
     */
    virtual void Stop(SSFObjectErrors &Errors) override;

    /**
     * 休眠
     */
    virtual void Sleep(SSFObjectErrors &Errors) override;

    /**
     * 销毁
     */
    virtual void Destroy(SSFObjectErrors &Errors) override;

    /**
     * 释放
     */
    virtual void Release(SSFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SSFOPlugin(SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager);
    virtual ~SSFOPlugin();

    /**
     * 获取插件名称
     * @return 插件名称
     */
    virtual const std::string GetName()
    {
        return SKYWALKER_SF_CLASS_NAME(SSFOPlugin);
    };

    /**
     * 获取模块
     * @param ModuleName 模块名称
     * @return 模块
     */
    template <typename T>
    SKYWALKER_SF_PTR(T)
    GetModule(const std::string &ModuleName);

private:
    /**
     * 安装
     */
    void Install(SSFModuleErrors &Errors);

    /**
     * 卸载
     */
    void Uninstall(SSFModuleErrors &Errors);

    /**
     * 加入模块
     */
    void AddModule(const std::string &ModuleName, SKYWALKER_SF_PTR_MODULE Module);

    /**
     * 移除模块
     */
    void RemoveModule(const std::string &ModuleName);

protected:
    SKYWALKER_SF_PTR_PLUGIN_MANAGER PluginManager;

private:
    /**
     * 模块
     */
    typedef SSFMap<std::string, bool> TMap_ModuleName;
    typedef std::map<std::string, SKYWALKER_SF_PTR_MODULE> TMap_Module;

    TMap_ModuleName ModuleNameMap;
    TMap_Module ModuleMap;
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_H__
