#include "TimerHeap.h"

namespace CPPWheels
{
    void TimerHeap::Siftup(size_t i) {
        assert(i >= 0 && i < m_Heap.size());
        size_t j = (i - 1) / 2;
        while (j >= 0) {
            if (m_Heap[j] < m_Heap[i])
                break;
            SwapNode(i, j);
            i = j;
            j = (i - 1) / 2;
        }
    }

    // false：不需要下滑  true：下滑成功
    bool TimerHeap::Siftdown(size_t index, size_t n) {
        assert(index >= 0 && index < m_Heap.size());
        assert(n >= 0 && n <= m_Heap.size());
        size_t i = index;
        size_t j = i * 2 + 1;  // 先标记i的左子节点
        while (j < n) {
            if (j + 1 < n && m_Heap[j + 1] < m_Heap[j]) // 得到左右子节点的较小者
                j++;
            if (m_Heap[i] < m_Heap[j])
                break;
            SwapNode(i, j);
            i = j;
            j = i * 2 + 1;
        }
        return i > index;
    }

    void TimerHeap::SwapNode(size_t i, size_t j) {
        assert(i >= 0 && i < m_Heap.size());
        assert(j >= 0 && j < m_Heap.size());
        std::swap(m_Heap[i], m_Heap[j]);
        ref_[m_Heap[i].id] = i;
        ref_[m_Heap[j].id] = j;
    }

    void TimerHeap::pop() {
        assert(!m_Heap.empty());
        del_(0);
    }

    void TimerHeap::clear() {
        ref_.clear();
        m_Heap.clear();
    }

    /* 删除指定位置的结点 */
    void TimerHeap::del_(size_t index) {
        assert(!m_Heap.empty() && index >= 0 && index < m_Heap.size());
        /* 将要删除的结点换到队尾，然后调整堆 */
        size_t i = index;
        size_t n = m_Heap.size() - 1;
        assert(i <= n);
        if (i < n) {
            SwapNode(i, n);
            if (!Siftdown(i, n)) {
                Siftup(i);
            }
        }
        // 队尾元素删除
        ref_.erase(m_Heap.back().id);
        m_Heap.pop_back();
    }

    // 调整指定id的结点
    void TimerHeap::adjust(int id, int timeout) {
        assert(!m_Heap.empty() && ref_.count(id) > 0);
        m_Heap[ref_[id]].expires = Clock::now() + MS(timeout);;
        Siftdown(ref_[id], m_Heap.size());
    }

    void TimerHeap::add(int id, int timeOut, const TimeoutCallBack& cb) {
        assert(id >= 0);
        size_t i;
        // 新元素，堆尾插入，调整堆
        if (ref_.count(id) == 0) { 
            i = m_Heap.size();
            ref_[id] = i;
            m_Heap.push_back({ id, Clock::now() + MS(timeOut), cb });
            Siftup(i);
        }
        else { 
            // 已存在，调整堆
            i = ref_[id];
            m_Heap[i].expires = Clock::now() + MS(timeOut);
            m_Heap[i].cb = cb;
            if (!Siftdown(i, m_Heap.size())) {
                Siftup(i);
            }
        }
    }

    // 删除指定id结点，并触发回调函数
    void TimerHeap::doWork(int id) {
        assert(id >= 0);
        if (m_Heap.empty() || ref_.count(id) == 0)
            return;

        size_t i = ref_[id];
        TimerNode node = m_Heap[i];
        node.cb();
        del_(i);
    }

    // 处理超时节点
    void TimerHeap::tick() {
        if (m_Heap.empty())
            return;

        while (!m_Heap.empty()) {
            TimerNode node = m_Heap.front();
            if (std::chrono::duration_cast<MS>(node.expires - Clock::now()).count() > 0)
                break;
            node.cb();
            pop();
        }
    }

    int TimerHeap::GetNextTick() {
        tick();
        size_t res = -1;
        if (!m_Heap.empty()) {
            res = std::chrono::duration_cast<MS>(m_Heap.front().expires - Clock::now()).count();
            if (res < 0)
                res = 0;
        }
        return res;
    }
}
