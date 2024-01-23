/*************************************************************************
**文件: SkywalkerTools\SkywalkerTimer\SkywalkerTimer.h
**作者: shyfan
**日期: 2023/09/21 17:04:11
**功能: 计时器
*************************************************************************/

#ifndef __SKYWALKER_TIMER_H__
#define __SKYWALKER_TIMER_H__

#include <windows.h>
#include <ctime>

#define SKYWALKER_TIMER_NAMESPACE Skywalker::Timer
#define SKYWALKER_TIMER_NAMESPACE_BEGIN \
    namespace SKYWALKER_TIMER_NAMESPACE \
    {
#define SKYWALKER_TIMER_NAMESPACE_END } // namespace SKYWALKER_TIMER_NAMESPACE

SKYWALKER_TIMER_NAMESPACE_BEGIN

/**
 * 计时器
 */
class SkywalkerTimer
{
public:
    SkywalkerTimer()
        : SecondsPerCount(0.0),
          DeltaTime(-1),
          PrevTime(0),
          CurrTime(0)
    {
        __int64 CountsPerSec;
        QueryPerformanceFrequency((LARGE_INTEGER *)&CountsPerSec);
        SecondsPerCount = 1.0 / (double)CountsPerSec;
    }
    virtual ~SkywalkerTimer()
    {
    }

    virtual void Release()
    {
        delete this;
    }

    /**
     * 重置
     */
    void Reset()
    {
        QueryPerformanceCounter((LARGE_INTEGER *)&CurrTime);

        BaseTime = CurrTime;
        BaseGMTTime = std::time(nullptr);
        PrevTime = CurrTime;
    }

    /**
     * 计时
     */
    void Tick()
    {
        QueryPerformanceCounter((LARGE_INTEGER *)&CurrTime);

        DeltaTime = (CurrTime - PrevTime) * SecondsPerCount;

        PrevTime = CurrTime;

        if (DeltaTime < 0)
        {
            DeltaTime = 0;
        }
    }

    /**
     * 获取总时间(s)
     */
    double GetTotalTime() const
    {
        return (double)(((CurrTime - BaseTime) * SecondsPerCount));
    }

    /**
     * 获取帧时间(ms)
     */
    __int64 GetDeltaTime() const
    {
        return DeltaTime;
    }

    /**
     * 获得启动时间(s)
     */
    double GetStartTime() const
    {
        return (double)(((BaseTime)*SecondsPerCount));
    }

    /**
     * 获得当前时间(s)
     */
    double GetCurrTime() const
    {
        return (double)(((CurrTime)*SecondsPerCount));
    }

    /**
     * 获得启动时的格林威治时间(s)
     */
    __int64 GetStartGMTTime() const
    {
        return BaseGMTTime;
    }

    /**
     * 获得当前的格林威治时间(s)
     */
    __int64 GetCurrGMTTime() const
    {
        return std::time(nullptr);
    }

private:
    double SecondsPerCount{};

    __int64 BaseTime{};
    __int64 BaseGMTTime{};
    __int64 PrevTime{};
    __int64 CurrTime{};
    __int64 DeltaTime{-1}; // (ms)Time between current frame and last frame
};

SKYWALKER_TIMER_NAMESPACE_END

#endif // __SKYWALKER_TIMER_H__
