/*************************************************************************
**文件: SkywalkerTools\SkywalkerPool\SkywalkerPool.h
**作者: shyfan
**日期: 2023/08/30 18:10:35
**功能: 对象池
*************************************************************************/

#ifndef __SKYWALKER_POOL_H__
#define __SKYWALKER_POOL_H__

#include <list>

#define SKYWALKER_POOL_NAMESPACE Skywalker::Pool
#define SKYWALKER_POOL_NAMESPACE_BEGIN \
    namespace SKYWALKER_POOL_NAMESPACE \
    {
#define SKYWALKER_POOL_NAMESPACE_END } // namespace SKYWALKER_POOL_NAMESPACE
#define SKYWALKER_POOL_NAMESPACE_USING using namespace SKYWALKER_POOL_NAMESPACE;

SKYWALKER_POOL_NAMESPACE_BEGIN

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

    /** 获取对象
     * @return T *	: 对象
     */
    T *Get()
    {
        if (Pool.empty())
        {
            return new T();
        }
        else
        {
            T *obj = Pool.front();
            Pool.pop_front();
            return obj;
        }
    }

    /** 回收对象
     * @param T *obj	: 对象
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

    /** 获取对象池大小
     * @return size_t	: 对象池大小
     */
    size_t Size()
    {
        return Pool.size();
    }
};

SKYWALKER_POOL_NAMESPACE_END

#endif // __SKYWALKER_POOL_H__
