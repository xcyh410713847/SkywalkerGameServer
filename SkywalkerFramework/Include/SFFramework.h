/*************************************************************************
**文件: SkywalkerFramework\Include\SFFramework.h
**作者: shyfan
**日期: 2024/01/03 16:49:50
**功能: SkywalkerFramework
*************************************************************************/

#ifndef __SKYWALKER_FRAMEWORK_SFFRAMEWORK_H__
#define __SKYWALKER_FRAMEWORK_SFFRAMEWORK_H__

#include "Include/SFCore.h"
#include "SkywalkerPath/SkywalkerPath.h"
#include "SkywalkerPlatform/SkywalkerPlatform.h"

SF_NAMESPACE_BEGIN

class SFPluginManager;

/**
 * Skywalker Framework 抽象接口
 *
 * 说明：
 * 1. 定义框架生命周期控制能力（启动、主循环、停止、关闭）
 * 2. 对外提供运行状态查询、对象 GUID 分配与插件管理器访问能力
 */
class SkywalkerFramework
{
public:
    /**
     * 启动框架
     * @return true 启动成功；false 启动失败
     */
    virtual bool Start() = 0;

    /**
     * 执行框架主循环
     * @return true 继续循环；false 结束循环
     */
    virtual bool Tick() = 0;

    /**
     * 停止框架
     * @return true 停止成功；false 停止失败
     */
    virtual bool Stop() = 0;

    /**
     * 请求关闭框架
     * 说明：通常由外部触发关闭意图，具体关闭行为由实现类在 Tick/Stop 中处理。
     */
    virtual void Close() = 0;

    /**
     * 获取框架是否处于运行状态
     * @return true 运行中；false 非运行中
     */
    virtual bool IsRunning() const = 0;

    /**
     * 生成新的对象 GUID
     * @return 新分配的对象 GUID
     */
    virtual SFObjectGUID NewObjectGUID() = 0;
};

SF_NAMESPACE_END

/**
 * 全局 Skywalker Framework 实例
 *
 * 说明：由应用层创建并在进程生命周期内持有。
 */
extern SF_UNIQUE_PTR(SF_NAMESPACE::SkywalkerFramework) SFFramework;

/**
 * Skywalker Framework 启动流程宏
 *
 * 用法：
 *   int main(int argc, char* argv[])
 *   {
 *       SKYWALKER_FRAMEWORK_START(argc, argv);
 *       return 0;
 *   }
 *
 * 说明：
 * 1. 先执行 Start，失败直接返回 1
 * 2. 在 Tick 返回 true 时持续主循环
 * 3. 循环结束后执行 Stop
 * 4. argc/argv 仅用于保持主函数调用形式一致，当前宏内部未使用
 */
#define SKYWALKER_FRAMEWORK_START(argc, argv) \
    if (!SFFramework->Start())                \
    {                                         \
        return 1;                             \
    }                                         \
    while (SFFramework->Tick())               \
    {                                         \
    }                                         \
    SFFramework->Stop();

#endif //__SKYWALKER_FRAMEWORK_SFFRAMEWORK_H__
