/*************************************************************************
**文件: SkywalkerFramework\Core\Object\SFObject.h
**作者: shyfan
**日期: 2023/08/07 23:52:40
**功能: 对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
#define __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__

#include "Include/SFCore.h"
#include "Include/SFErrors.h"
#include "Include/SFFramework.h"
#include "SFArchive.h"

SF_NAMESPACE_BEGIN

/**
 * 对象上下文基类
 * 说明：用于对象创建时传递初始化参数；具体对象可扩展自定义 Context。
 */
struct SSFObjectContext
{
};

/**
 * 框架对象基类
 *
 * 职责：
 * 1. 提供对象基础元信息（类名、GUID）
 * 2. 提供统一释放接口
 * 3. 提供对象工厂方法 NewObject（含多种重载）
 */
class SSFObject
{

protected:
	/** 对象类名缓存（首次获取时懒加载） */
	SFString ObjectClassName{};

private:
	/** 根对象指针（用于回溯框架实例） */
	SF_PTR(SSFObject)
	RootObject{};

	/** 对象唯一标识 */
	SFObjectGUID ObjectGUID{SF_OBJECT_INVALID_GUID};

public:
	/** 构造函数（带上下文） */
	SSFObject(SSFObjectContext &InContext, SFObjectErrors &InErrors) {};
	/** 默认构造函数 */
	SSFObject() {};
	/** 虚析构函数 */
	virtual ~SSFObject() {};

	/**
	 * 释放对象
	 * 说明：默认行为为 self-delete，要求对象由 NewObject/new 创建。
	 * @param Errors 错误输出对象（当前默认实现未写入错误）
	 */
	virtual void Release(SFObjectErrors &Errors)
	{
		delete this;
	};

	/**
	 * 获取类名
	 * @return 类名 const SSFString &
	 */
	virtual const SFString &GetObjectClassName()
	{
		if (ObjectClassName.empty())
		{
			SF_CLASS_NAME(*this, ObjectClassName);
		}

		return ObjectClassName;
	}

	/**
	 * 获取框架实例
	 * @return 框架指针；转换失败时返回 nullptr
	 */
	SF_PTR(SkywalkerFramework)
	GetFramework()
	{
		return SF_PTR_DYNAMIC_CAST(SkywalkerFramework)(GetRootObject());
	};

	/**
	 * 获取对象 GUID
	 * @return 对象 GUID
	 */
	SFObjectGUID GetObjectGUID()
	{
		return ObjectGUID;
	};

private:
	/**
	 * 获取根对象
	 * 说明：首次访问时将当前对象设为根对象。
	 * @return 根对象指针
	 */
	SF_PTR(SSFObject)
	GetRootObject()
	{
		if (SF_PTR_INVALID(RootObject))
		{
			RootObject = this;
		}

		return RootObject;
	};
#pragma region NewObject

public:
	/**
	 * 创建对象（完整重载：上下文 + 错误输出）
	 * @tparam ObjectT 目标对象类型
	 * @tparam ContextT 上下文类型
	 * @param InContext 对象初始化上下文
	 * @param InErrors 错误输出对象
	 * @return 新对象指针
	 * 说明：会自动继承 RootObject，并向框架申请新的 ObjectGUID。
	 */
	template <typename ObjectT, typename ContextT>
	SF_PTR(ObjectT)
	NewObject(ContextT &InContext, SFObjectErrors &InErrors)
	{
		auto Object = new ObjectT(InContext, InErrors);

		Object->RootObject = GetRootObject();
		Object->ObjectGUID = GetFramework()->NewObjectGUID();

		return Object;
	}

	/**
	 * 创建对象（无上下文，带错误输出）
	 * @tparam ObjectT 目标对象类型
	 * @param InErrors 错误输出对象
	 * @return 新对象指针
	 * 说明：使用静态 SSFObjectContext 作为默认上下文。
	 */
	template <typename ObjectT>
	SF_PTR(ObjectT)
	NewObject(SFObjectErrors &InErrors)
	{
		static SSFObjectContext InContext{};
		return NewObject<ObjectT>(InContext, InErrors);
		;
	}

	/**
	 * 创建对象（带上下文，不关心错误输出）
	 * @tparam ObjectT 目标对象类型
	 * @tparam ContextT 上下文类型
	 * @param InContext 对象初始化上下文
	 * @return 新对象指针
	 * 说明：内部使用临时错误对象承接错误信息。
	 */
	template <typename ObjectT, typename ContextT>
	SF_PTR(ObjectT)
	NewObject(ContextT &InContext)
	{
		SFObjectErrors Errors{};
		return NewObject<ObjectT>(InContext, Errors);
	}

	/**
	 * 创建对象（最简重载：默认上下文 + 忽略错误输出）
	 * @tparam ObjectT 目标对象类型
	 * @return 新对象指针
	 */
	template <typename ObjectT>
	SF_PTR(ObjectT)
	NewObject()
	{
		SFObjectErrors Errors{};
		return NewObject<ObjectT>(Errors);
	}

#pragma endregion NewObject

#pragma region Serialization

public:
	/**
	 * 序列化对象
	 * @param Ar 存档流
	 * 说明：子类应重写此函数以序列化自定义数据
	 */
	virtual void Serialize(SFArchive& Ar)
	{
		// 序列化ObjectGUID
		Ar << ObjectGUID;
	}

	/**
	 * 反序列化完成回调
	 * 说明：子类可重写此函数，在反序列化完成后执行初始化逻辑
	 */
	virtual void OnDeserialized()
	{
		// 子类可重写
	}

	/**
	 * 设置ObjectGUID
	 * @param GUID 新的GUID
	 */
	void SetObjectGUID(SFObjectGUID GUID)
	{
		ObjectGUID = GUID;
	}

#pragma endregion Serialization
};

/**
 * SF_PROPERTY 宏
 * 用于标记需要序列化的属性
 * 使用方式：SF_PROPERTY(PropertyName)
 */
#define SF_PROPERTY(PropertyName)

/**
 * SF_OBJECT_BEGIN 宏
 * 声明序列化相关函数和友元
 * 使用方式：SF_OBJECT_BEGIN(ClassName)
 */
#define SF_OBJECT_BEGIN(ClassName) \
public: \
    static SFString StaticClassName() { return #ClassName; } \
    SFString GetClassName() const override { return #ClassName; } \
    friend class SFArchive;

/**
 * SF_OBJECT_END 宏
 * 结束对象序列化声明
 */
#define SF_OBJECT_END()

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
