#pragma once

#include <list>
//std::list 是一个双向链表容器，它允许在序列中的任何位置快速插入和删除元素。
//不需要所有元素在内存中连续存储，因此它不会因为重新分配内存和移动元素而导致迭代器、引用或指针失效（除了指向被删除元素的迭代器、引用或指针）。
namespace CPPTools
{
    // Simple Object Pool
    template<typename Object>
    class ObjectPool {
    public:
        // size_t 无符整型，大小根据平台和编译器改变，一般足以容内存中任意对象大小
        ObjectPool(size_t size) : m_Size(size)
        {
            for (size_t i = 0; i < m_Size; i++)
            {
                m_ObjectPool.push_back(new Object());
            }
        }
        ~ObjectPool()
        {
            typename std::list<Object*>::iterator iter = m_ObjectPool.begin();

            while (iter != m_ObjectPool.end())
            {
                delete(*iter);
                iter++;
            }
            m_Size = 0;
        }
        Object* GetObject()
        {
            Object* obj = nullptr;
            if (m_Size == 0)
            {
                obj = new Object();
            }
            else
            {
                obj = m_ObjectPool.front();
                m_ObjectPool.pop_front();
                m_Size--;
            }
            return obj;
        }

        void PushObject(Object* obj)
        {
            m_ObjectPool.push_back(obj);
            m_Size ++;
        }
    private:
        size_t m_Size;
        std::list<Object*>m_ObjectPool;
    };
}