/*************************************************************************
**文件: SkywalkerTools\SkywalkerTimer\SkywalkerTimer.h
**作者: shyfan
**日期: 2023/09/21 17:04:11
**功能: 计时器
*************************************************************************/

#ifndef __SKYWALKER_TIMER_H__
#define __SKYWALKER_TIMER_H__

#include "SkywalkerPlatform/SkywalkerPlatform.h"
#include <ctime>
#include <format>
#include <chrono>

#define SKYWALKER_TIMER_NAMESPACE Skywalker::Timer
#define SKYWALKER_TIMER_NAMESPACE_BEGIN \
    namespace SKYWALKER_TIMER_NAMESPACE \
    {
#define SKYWALKER_TIMER_NAMESPACE_END } // namespace SKYWALKER_TIMER_NAMESPACE

SKYWALKER_TIMER_NAMESPACE_BEGIN

typedef unsigned long long UINT64;

/**
 * 计时器
 */
class SkywalkerTimer
{
public:
    SkywalkerTimer()
        : SecondsPerCount(0.0),
          DeltaTime(0),
          PrevTime(0),
          CurrTime(0)
    {
#if defined(SKYWALKER_PLATFORM_WINDOWS)
        UINT64 CountsPerSec;
        QueryPerformanceFrequency((LARGE_INTEGER *)&CountsPerSec);
        SecondsPerCount = 1.0 / (double)CountsPerSec;
#else
        timespec timeRes;
        clock_getres(CLOCK_MONOTONIC, &timeRes);
        SecondsPerCount = static_cast<double>(timeRes.tv_sec) + static_cast<double>(timeRes.tv_nsec) / 1e9;
#endif
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
#if defined(SKYWALKER_PLATFORM_WINDOWS)
        QueryPerformanceCounter((LARGE_INTEGER *)&CurrTime);

        BaseTime = CurrTime;
#else
        timespec currentTime;
        clock_gettime(CLOCK_MONOTONIC, &currentTime);

        CurrTime = currentTime.tv_sec * 1000000000 + currentTime.tv_nsec;
        BaseTime = CurrTime;
#endif

        BaseGMTTime = std::time(nullptr);
        PrevTime = BaseTime;
    }

    /**
     * 计时
     */
    void Tick()
    {
#if defined(SKYWALKER_PLATFORM_WINDOWS)
        QueryPerformanceCounter((LARGE_INTEGER *)&CurrTime);
        DeltaTime = (CurrTime - PrevTime) * SecondsPerCount;
#else
        timespec currentTime;
        clock_gettime(CLOCK_MONOTONIC, &currentTime);
        CurrTime = currentTime.tv_sec * 1000000000 + currentTime.tv_nsec;
        DeltaTime = (CurrTime - PrevTime) / 1000000; // 转换为毫秒
#endif

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
    UINT64 GetDeltaTime() const
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
    UINT64 GetStartGMTTime() const
    {
        return BaseGMTTime;
    }

    /**
     * 获得当前的格林威治时间(s)
     */
    UINT64 GetCurrGMTTime() const
    {
        return std::time(nullptr);
    }

    /**
     * 获取当前时间，格式：年-月-日 时:分:秒:毫秒
     * 通过 CurrTime 装换
     */
    std::string GetCurrTimeStr() const
    {
        auto now = std::chrono::system_clock::now();
        auto t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        // 声明 tm 结构体
        std::tm tm = {};

#if defined(SKYWALKER_PLATFORM_WINDOWS)
        gmtime_s(&tm, &t);
#else
        gmtime_r(&t, &tm);
#endif

        // 使用std::format来格式化时间
        return std::format("{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:03}",
                           tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                           tm.tm_hour, tm.tm_min, tm.tm_sec, ms.count());
    }

private:
    double SecondsPerCount{};

    UINT64 BaseTime{};
    UINT64 BaseGMTTime{};
    UINT64 PrevTime{};
    UINT64 CurrTime{};
    UINT64 DeltaTime{0}; // (ms)Time between current frame and last frame
};

SKYWALKER_TIMER_NAMESPACE_END

#endif // __SKYWALKER_TIMER_H__
