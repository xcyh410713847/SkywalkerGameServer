/*************************************************************************
**文件: SkywalkerTools\SkywalkerPool\SkywalkerPool.h
**作者: shyfan
**日期: 2023/08/30 18:10:35
**功能: 对象池
*************************************************************************/

#ifndef __SKYWALKER_POOL_H__
#define __SKYWALKER_POOL_H__

#include <list>

/** 对象池命名空间宏 */
#define SKYWALKER_POOL_NAMESPACE Skywalker::Pool
#define SKYWALKER_POOL_NAMESPACE_BEGIN \
    namespace SKYWALKER_POOL_NAMESPACE \
    {
#define SKYWALKER_POOL_NAMESPACE_END } // namespace SKYWALKER_POOL_NAMESPACE
#define SKYWALKER_POOL_NAMESPACE_USING using namespace SKYWALKER_POOL_NAMESPACE;

SKYWALKER_POOL_NAMESPACE_BEGIN

/**
 * 简单对象池
 * 说明：对象生命周期由对象池管理，回收时超过上限会直接释放。
 */
template <typename T>
class CSkywalkerPool
{
    std::list<T *> Pool; // 对象池
    int MaxSize = 0;

public:
    CSkywalkerPool(int Size)
        : MaxSize(Size) {}

    ~CSkywalkerPool()
    {
        for (auto it = Pool.begin(); it != Pool.end(); ++it)
        {
            delete *it;
        }
        Pool.clear();
    }

    /**
     * 获取对象
     * 注意：调用方应确保池中已有对象，否则 front() 行为未定义。
     */
    T *Get()
    {
        T *obj = Pool.front();
        Pool.pop_front();
        return obj;
    }

    /**
     * 回收对象
     * @param obj 对象指针
     */
    void Recycle(T *obj)
    {
        if (Pool.size() >= MaxSize)
        {
            delete obj;
            return;
        }

        Pool.push_back(obj);
    }

    /**
     * 获取当前池大小
     */
    size_t Size()
    {
        return Pool.size();
    }
};

SKYWALKER_POOL_NAMESPACE_END

/** 对象池指针类型 */
#define SKYWALKER_POOL_PTR(T) SKYWALKER_POOL_NAMESPACE::CSkywalkerPool<T> *
/** 创建对象池 */
#define SKYWALKER_POOL_NEW(T, MaxSize) new SKYWALKER_POOL_NAMESPACE::CSkywalkerPool<T>(MaxSize);

#endif // __SKYWALKER_POOL_H__
