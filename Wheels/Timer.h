#pragma once
#include <chrono>
#include <thread>
#include <mutex>
#include <functional>
#include <vector>

//基础概念之进程与线程
//1. 进程指的是系统分配和调度资源的独立单位，具有一定独立功能关于某个数据集上的一次活动
//2. 线程是进程的一个实体，比进程更小的独立运行的基本单位，但是不拥有系统资源，线程消耗也比进程更小，一个线程崩溃，整个线程都会崩溃；但进程则不会相互影响。
//   另外，由于线程不拥有系统资源，共享进程的资源，导致访问时需要通过互斥与同步保证数据的完整和一致。
//3. 【更进一步】进程是线程的容器，一个进程至少包含一个主线程，进程和线程都可以并发运行，线程切换开销更小。

//基础概念之并发与并行
//1. 并发指的是单核CPU通过多线程或多进程交替执行多个任务
//2. 并行指的是多核CPU同时执行任务

//基础概念之同步与异步
//1. 同步指的是前一个任务执行完，然后再执行下一个任务
//2. 异步指的是多个任务不会相互等待，如多线程，单核CPU通过分配CPU时间片来交替执行。

//异步编程和多线程编程
//1. 异步编程指的是一种编程范式，能够让程序在等待如I/O的同时(不阻塞该线程)，执行其它任务
//2. 多线程则是需要投入CPU资源来调度多个线程执行不同的任务
// 前者适合密集I/O如网络请求和文件读写；后者适合密集计算如图像计算和大数据，能够充分利用多核CPU的并行处理能力


namespace CPPWheels
{
    using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

    struct ProfileResult
    {
        std::string Name;

        FloatingPointMicroseconds Start;
        std::chrono::microseconds ElapsedTime;
        std::thread::id ThreadID;
    };

    // 简单计时器
    class Timer
    {
    public:
        Timer(const char* name)
            : m_Name(name), m_Stopped(false)
        {
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        ~Timer()
        {
            if (!m_Stopped)
                Stop();
        }

        void Stop()
        {
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            // microseconds微秒
            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            m_Stopped = true;

            float duration = (end - start) * 0.001f;

            std::cout << m_Name << " Duration: " << duration << "ms" << std::endl;
        }
    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
        bool m_Stopped;
    };

    //ref:https://www.cnblogs.com/developing/p/10856299.html
    // 基于时间轮算法的定时器
    // 1. 时间轮是一个环形的数据结构
    // 2. 这个数据结构被分成若干槽slot，指针每前进一个槽，表示时间流逝了一个固定的时间单位
    // 3. 时间轮算法用于高效管理延时任务和周期任务，还有异步执行解耦。
    // 以下是一个单轮的时间轮类
    class TimerWheel
    {
    public:
        using Task = std::function<void()>;

        // 只能显示调用该构造函数
        explicit TimerWheel(size_t wheel_size, int interval_ms)
            : size(wheel_size),
            interval(interval_ms),
            current_slot(0) {
            wheel.resize(wheel_size);
        }

        // 析构函数
        ~TimerWheel() {
            Stop();
        }

        void Start()
        {
            if (isRunning)
                return;
            isRunning = true;

            // 创建线程对象
            m_Thread = std::thread([this]() {
                while (isRunning)
                {
                    // 隔interval毫秒执行一次
                    std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                    Tick();
                }
                std::cout << "timer tick thread end..." << std::endl;
            });

            // 释放线程及其相关资源
            // 一般使用join或detach
            // detach主要用于分离该线程，主线程继续执行，而失去被分离的这个线程的管理，不关心这个线程何时执行完
            m_Thread.detach();
        }

        void Stop()
        {
            if (!isRunning) {
                return;
            }
            isRunning = false;

            // 线程阻塞,之前已经detach过了，这里不能join
            if (m_Thread.joinable())
            {
                m_Thread.join();
            }
            //std::cout << "wheel size:" << wheel.size();
        }

        void AddTask(int timeout, Task task)
        {
            // RAII锁包装器，构造时自动给互斥锁上锁，析构时自动解锁
            std::lock_guard<std::mutex> lock(m_Mutex);
            size_t ticks = timeout / interval;//指针前进ticks个slot
            size_t index = (current_slot + ticks) % size;
            wheel[index].push_back(task);
        }

    private:
        void Tick()
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            auto& tasks = wheel[current_slot];
            for (const auto& task : tasks) {
                std::cout << "executing slot:" << current_slot << " ";
                task();
            }

            current_slot = (current_slot + 1) % size;
        }

        size_t size;
        int interval;//ms
        std::vector<std::list<Task>> wheel;
        size_t current_slot;
        bool isRunning = false;
        std::thread m_Thread;
        std::mutex m_Mutex;
    };

    // []表示捕获（capture）上下文变量，[=]按值，[&]按引用，[]什么也不捕获
//#define AS_PROFILE_SCOPE(name) Timer timer##__LINE__(name),[&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult); }
}