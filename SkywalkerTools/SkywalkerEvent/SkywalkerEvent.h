/*************************************************************************
**文件: SkywalkerTools\SkywalkerEvent\SkywalkerEvent.h
**作者: shyfan
**日期: 2023/11/06 20:35:21
**功能: 事件
*************************************************************************/

#ifndef __SKYWALKER_EVENT_H__
#define __SKYWALKER_EVENT_H__

#include <functional>
#include <map>
#include <list>

#define SKYWALKER_EVENT_NAMESPACE Skywalker::Event
#define SKYWALKER_EVENT_NAMESPACE_BEGIN \
    namespace SKYWALKER_EVENT_NAMESPACE \
    {
#define SKYWALKER_EVENT_NAMESPACE_END } // namespace SKYWALKER_EVENT_NAMESPACE
#define SKYWALKER_EVENT_NAMESPACE_USING using namespace SKYWALKER_EVENT_NAMESPACE;

SKYWALKER_EVENT_NAMESPACE_BEGIN

#pragma region 事件ID宏

/**
 * 合并 MainID 和 SubID 生成事件ID
 */
#define SKYWALKER_EVENT_ID(MainID, SubID) ((static_cast<uint64_t>(MainID) << 32) | static_cast<uint64_t>(SubID))
/**
 * 获取事件的 MainID
 */
#define SKYWALKER_EVENT_GET_MAIN_ID(EventID) (static_cast<SkywalkerEventMainID>(EventID >> 32))
/**
 * 获取事件的 SubID
 */
#define SKYWALKER_EVENT_GET_SUB_ID(EventID) (static_cast<SkywalkerEventSubID>(EventID & 0xFFFFFFFF))

#pragma endregion 事件ID宏

/**
 * 事件顺序枚举
 */
enum class ESkywalkerEventOrder
{
    /* 事件前之前 */
    BeforeBefore = 0,
    /* 事件前 */
    Before,
    /* 事件中 */
    During,
    /* 事件后 */
    After,
    /* 事件后之后 */
    AfterAfter,
    /* Max */
    Max,
};

typedef uint32_t SkywalkerEventMainID;
typedef uint32_t SkywalkerEventSubID;
typedef uint64_t SkywalkerEventID;
typedef void *SkywalkerEventParam;
typedef uint32_t SkywalkerEventParamSize;

typedef std::function<bool(SkywalkerEventMainID, SkywalkerEventSubID, SkywalkerEventParam, SkywalkerEventParamSize)> SkywalkerEventCallback;
typedef SkywalkerEventCallback *SkywalkerEventCallbackPtr;

struct SSkywalkerEventData
{
    bool IsValid;
    SkywalkerEventCallback Callback;
    SkywalkerEventCallbackPtr CallbackPtr;

    SSkywalkerEventData()
    {
        Reset();
    }

    void Reset()
    {
        IsValid = false;
        Callback = nullptr;
    }
};

typedef std::list<SSkywalkerEventData> TList_SkywalkerEventCallback;
typedef std::map<SkywalkerEventID, TList_SkywalkerEventCallback[static_cast<size_t>(ESkywalkerEventOrder::Max)]> TMap_SkywalkerEventCallback;

class CSkywalkerEvent
{
public:
    CSkywalkerEvent(){};
    ~CSkywalkerEvent(){};

public:
    /**
     *  注册事件
     */
    static void RegisterEvent(SkywalkerEventMainID InMainID, SkywalkerEventSubID InSubID,
                              SkywalkerEventCallback InCallback,
                              ESkywalkerEventOrder InOrder = ESkywalkerEventOrder::During)
    {
        SkywalkerEventID EventID = SKYWALKER_EVENT_ID(InMainID, InSubID);

        SSkywalkerEventData EventData;
        EventData.IsValid = true;
        EventData.Callback = InCallback;
        EventData.CallbackPtr = &InCallback;

        SkywalkerEventCallbackMap[EventID][static_cast<size_t>(InOrder)].push_back(EventData);
    }

    /**
     *  注销事件
     */
    static void UnRegisterEvent(SkywalkerEventMainID InMainID, SkywalkerEventSubID InSubID,
                                SkywalkerEventCallback InCallback)
    {
        SkywalkerEventID EventID = SKYWALKER_EVENT_ID(InMainID, InSubID);

        auto IterEventID = SkywalkerEventCallbackMap.find(EventID);
        if (IterEventID == SkywalkerEventCallbackMap.end())
        {
            return;
        }

        for (size_t i = 0; i < static_cast<size_t>(ESkywalkerEventOrder::Max); ++i)
        {
            auto &ListEventCallback = IterEventID->second[i];
            for (auto IterEventData : ListEventCallback)
            {
                if (IterEventData.CallbackPtr == &InCallback)
                {
                    IterEventData.IsValid = false;
                }
            }
        }
    }

    /**
     *  触发事件
     */
    static void TriggerEvent(SkywalkerEventMainID InMainID, SkywalkerEventSubID InSubID,
                             SkywalkerEventParam InParam = nullptr, SkywalkerEventParamSize ParamSize = 0)
    {
        SkywalkerEventID EventID = SKYWALKER_EVENT_ID(InMainID, InSubID);

        auto IterEventID = SkywalkerEventCallbackMap.find(EventID);
        if (IterEventID == SkywalkerEventCallbackMap.end())
        {
            return;
        }

        for (size_t i = 0; i < static_cast<size_t>(ESkywalkerEventOrder::Max); ++i)
        {
            auto &ListEventCallback = IterEventID->second[i];
            for (auto IterEventData : ListEventCallback)
            {
                if (IterEventData.IsValid && IterEventData.Callback)
                {
                    IterEventData.Callback(InMainID, InSubID, InParam, ParamSize);
                }
            }
        }
    }

private:
    static TMap_SkywalkerEventCallback SkywalkerEventCallbackMap;
};

inline TMap_SkywalkerEventCallback CSkywalkerEvent::SkywalkerEventCallbackMap;

/**
 * 注册事件，不指定 Order
 */
#define SKYWALKER_REGISTER_EVENT(MainID, SubID, Callback) \
    CSkywalkerEvent::RegisterEvent(MainID, SubID, Callback);

/**
 * 注册事件，指定 Order
 */
#define SKYWALKER_REGISTER_EVENT_ORDER(MainID, SubID, Callback, Order) \
    CSkywalkerEvent::RegisterEvent(MainID, SubID, Callback, Order);

/**
 * 注销事件
 */
#define SKYWALKER_UNREGISTER_EVENT(MainID, SubID, Callback) \
    CSkywalkerEvent::UnRegisterEvent(MainID, SubID, Callback);

/**
 * 触发事件
 */
#define SKYWALKER_TRIGGER_EVENT(MainID, SubID, Param) \
    CSkywalkerEvent::TriggerEvent(MainID, SubID, &Param, sizeof(Param));

SKYWALKER_EVENT_NAMESPACE_END

#endif //__SKYWALKER_EVENT_H__
