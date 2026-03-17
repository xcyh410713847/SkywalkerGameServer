/*************************************************************************
**文件: SkywalkerFramework\Core\Object\SFArchive.h
**作者: shyfan
**日期: 2026/03/17
**功能: 抽象存档流（参照虚幻FArchive）
*************************************************************************/

#ifndef __SKYWALKER_FRAMEWORK_ARCHIVE_H__
#define __SKYWALKER_FRAMEWORK_ARCHIVE_H__

#include "Include/SFCore.h"
#include "Include/SFErrors.h"
#include "Include/SFFramework.h"

SF_NAMESPACE_BEGIN

/**
 * 序列化版本号
 */
#define SF_ARCHIVE_VERSION 1

/**
 * 抽象存档流基类
 * 负责对象的序列化和反序列化操作
 * 参照虚幻引擎 FArchive 设计
 */
class SFArchive
{
public:
    SFArchive() = default;
    virtual ~SFArchive() = default;

    /**
     * 序列化基本类型
     * @tparam T 基本类型
     * @param Value 引用值
     * @return 存档流引用（支持链式调用）
     */
    template <typename T>
    SFArchive& operator<<(T& Value)
    {
        SerializeRaw(&Value, sizeof(T));
        return *this;
    }

    /**
     * 序列化STL字符串
     */
    template <>
    SFArchive& operator<< <SFString>(SFString& Value)
    {
        SerializeString(Value);
        return *this;
    }

    /**
     * 序列化bool类型
     */
    template <>
    SFArchive& operator<< <bool>(bool& Value)
    {
        SFUInt8 BoolValue = Value ? 1 : 0;
        SerializeRaw(&BoolValue, sizeof(SFUInt8));
        Value = (BoolValue != 0);
        return *this;
    }

    /**
     * 序列化对象指针
     * @tparam T 对象类型
     * @param Object 对象指针引用
     */
    template <typename T>
    void SerializeObject(SF_PTR(T)& Object)
    {
        if (IsLoading())
        {
            // 反序列化：从GUID查找或创建对象
            SFObjectGUID GUID;
            *this << GUID;
            // TODO: 通过ObjectManager查找或创建对象
        }
        else
        {
            // 序列化：写入GUID
            if (Object != nullptr)
            {
                SFObjectGUID GUID = Object->GetObjectGUID();
                *this << GUID;
            }
            else
            {
                SFObjectGUID InvalidGUID = SF_OBJECT_INVALID_GUID;
                *this << InvalidGUID;
            }
        }
    }

    /**
     * 判断是否为加载模式（反序列化）
     */
    virtual bool IsLoading() const = 0;

    /**
     * 判断是否为保存模式（序列化）
     */
    virtual bool IsSaving() const = 0;

    /**
     * 获取当前版本号
     */
    SFUInt32 GetVersion() const { return Version; }

    /**
     * 设置版本号
     */
    void SetVersion(SFUInt32 InVersion) { Version = InVersion; }

protected:
    /**
     * 序列化原始数据
     * @param Data 数据指针
     * @param Size 数据大小
     */
    virtual void SerializeRaw(void* Data, SFSize Size) = 0;

    /**
     * 序列化字符串
     * @param Str 字符串引用
     */
    virtual void SerializeString(SFString& Str) = 0;

    /** 版本号 */
    SFUInt32 Version = SF_ARCHIVE_VERSION;
};

/**
 * 存档流运算符重载宏
 * 用于简化基本类型序列化
 */
#define SF_ARCHIVE_SERIALIZE(Type) \
    SFArchive& operator<<(Type& Value) { \
        if (IsLoading()) { \
            SerializeRaw(&Value, sizeof(Type)); \
        } else { \
            const Type& ConstValue = Value; \
            SerializeRaw((void*)&ConstValue, sizeof(Type)); \
        } \
        return *this; \
    }

SF_NAMESPACE_END

#endif // __SKYWALKER_FRAMEWORK_ARCHIVE_H__
