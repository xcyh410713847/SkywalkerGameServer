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
#include <atomic>

/** 事件系统命名空间宏 */
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
 * 按从前到后的顺序执行
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
typedef uint64_t SkywalkerEventCallbackID;  // 回调唯一ID

/**
 * 事件回调签名
 * 返回值目前未用于中断流程，保留给上层扩展
 */
typedef std::function<bool(SkywalkerEventMainID, SkywalkerEventSubID, SkywalkerEventParam, SkywalkerEventParamSize)> SkywalkerEventCallback;

/**
 * 单个事件回调节点
 */
struct SSkywalkerEventData
{
    bool IsValid;
    SkywalkerEventCallback Callback;
    SkywalkerEventCallbackID CallbackID;  // 唯一标识符，用于注销匹配

    SSkywalkerEventData()
    {
        Reset();
    }

    void Reset()
    {
        IsValid = false;
        Callback = nullptr;
        CallbackID = 0;
    }
};

typedef std::list<SSkywalkerEventData> TList_SkywalkerEventCallback;
typedef std::map<SkywalkerEventID, TList_SkywalkerEventCallback[static_cast<size_t>(ESkywalkerEventOrder::Max)]> TMap_SkywalkerEventCallback;

/**
 * 事件总线（静态管理）
 */
class CSkywalkerEvent
{
public:
    CSkywalkerEvent() {};
    ~CSkywalkerEvent() {};

public:
    /**
     * 注册事件
     * @param InMainID 主事件ID
     * @param InSubID 子事件ID
     * @param InCallback 回调
     * @param InOrder 执行阶段
     * @return 返回回调ID，用于注销
     */
    static SkywalkerEventCallbackID RegisterEvent(SkywalkerEventMainID InMainID, SkywalkerEventSubID InSubID,
                              SkywalkerEventCallback InCallback,
                              ESkywalkerEventOrder InOrder = ESkywalkerEventOrder::During)
    {
        SkywalkerEventID EventID = SKYWALKER_EVENT_ID(InMainID, InSubID);

        SSkywalkerEventData EventData;
        EventData.IsValid = true;
        EventData.Callback = std::move(InCallback);
        EventData.CallbackID = ++s_CallbackIDGenerator;  // 原子递增生成唯一ID

        SkywalkerEventCallbackMap[EventID][static_cast<size_t>(InOrder)].push_back(std::move(EventData));
        return EventData.CallbackID;
    }

    /**
     * 注销事件
     * @param InCallbackID 注册时返回的回调ID
     */
    static void UnRegisterEventByID(SkywalkerEventCallbackID InCallbackID)
    {
        for (auto &IterEventID : SkywalkerEventCallbackMap)
        {
            for (size_t i = 0; i < static_cast<size_t>(ESkywalkerEventOrder::Max); ++i)
            {
                auto &ListEventCallback = IterEventID.second[i];
                for (auto &IterEventData : ListEventCallback)
                {
                    if (IterEventData.CallbackID == InCallbackID)
                    {
                        IterEventData.IsValid = false;
                        return;  // 找到后立即返回
                    }
                }
            }
        }
    }

    /**
     * 注销事件（兼容旧接口，按MainID/SubID清除所有回调）
     * @deprecated 建议使用 UnRegisterEventByID
     */
    static void UnRegisterEvent(SkywalkerEventMainID InMainID, SkywalkerEventSubID InSubID)
    {
        SkywalkerEventID EventID = SKYWALKER_EVENT_ID(InMainID, InSubID);
        SkywalkerEventCallbackMap.erase(EventID);
    }

    /**
     * 触发事件
     * 按 ESkywalkerEventOrder 顺序依次执行
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
    /** 事件回调总表：EventID -> [OrderList...] */
    static TMap_SkywalkerEventCallback SkywalkerEventCallbackMap;
    /** 回调ID生成器（原子递增） */
    static std::atomic<SkywalkerEventCallbackID> s_CallbackIDGenerator;
};

inline TMap_SkywalkerEventCallback CSkywalkerEvent::SkywalkerEventCallbackMap;
inline std::atomic<SkywalkerEventCallbackID> CSkywalkerEvent::s_CallbackIDGenerator(0);

/**
 * 注册事件，不指定 Order，返回回调ID
 */
#define SKYWALKER_REGISTER_EVENT(MainID, SubID, Callback) \
    CSkywalkerEvent::RegisterEvent(MainID, SubID, Callback);

/**
 * 注册事件，指定 Order，返回回调ID
 */
#define SKYWALKER_REGISTER_EVENT_ORDER(MainID, SubID, Callback, Order) \
    CSkywalkerEvent::RegisterEvent(MainID, SubID, Callback, Order);

/**
 * 注销事件（通过回调ID）
 */
#define SKYWALKER_UNREGISTER_EVENT_BY_ID(CallbackID) \
    CSkywalkerEvent::UnRegisterEventByID(CallbackID);

/**
 * 注销事件（清除指定EventID的所有回调 - 兼容旧接口）
 */
#define SKYWALKER_UNREGISTER_EVENT(MainID, SubID) \
    CSkywalkerEvent::UnRegisterEvent(MainID, SubID);

/**
 * 触发事件
 */
#define SKYWALKER_TRIGGER_EVENT(MainID, SubID, Param) \
    CSkywalkerEvent::TriggerEvent(MainID, SubID, &Param, sizeof(Param));

SKYWALKER_EVENT_NAMESPACE_END

#endif //__SKYWALKER_EVENT_H__
