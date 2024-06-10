#pragma once
#include <queue>
#include <unordered_map>
#include <time.h>
#include <algorithm>
#include <functional> 
#include <assert.h> 
#include <chrono>
namespace CPPWheels
{
    // 高性能定时器，时间堆Time heap, 最小堆


    using TimeoutCallBack = std::function<void()>;
    using Clock = std::chrono::high_resolution_clock;   // ns级时钟
    using MS = std::chrono::milliseconds;
    using TimeStamp = Clock::time_point;      // 具体时间

    // 定时器节点
    struct TimerNode {
        int id;
        TimeStamp expires;
        TimeoutCallBack cb;
        bool operator<(const TimerNode& t) {
            return expires < t.expires;
        }
    };

    // 定时器管理类
    class TimerHeap {
    public:
        TimerHeap() { heap_.reserve(64); }

        ~TimerHeap() { clear(); }

        void adjust(int id, int newExpires);

        void add(int id, int timeOut, const TimeoutCallBack& cb);

        void doWork(int id);

        void clear();

        void tick();

        void pop();

        int GetNextTick();

    private:
        void del_(size_t i);

        void siftup_(size_t i);

        bool siftdown_(size_t index, size_t n);

        void SwapNode_(size_t i, size_t j);

    private:
        std::vector<TimerNode> heap_;

        std::unordered_map<int, size_t> ref_;  // key：节点id, value：数组索引 

    };
}