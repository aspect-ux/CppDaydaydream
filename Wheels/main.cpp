#include <iostream>
#include <random>
#include "ObjectPool.h"
#include "Timer.h"
#include "TimerHeap.h"
#include "Memory.h"
#include "PokerDealer.h"

namespace CppDaydaydream
{
    
}

class User {
public:
    User(){}
    User(std::string id):id(id){}
    std::string id;
};

int main()
{
    using namespace CPPWheels;

    ObjectPool<User> pool = ObjectPool<User>(1000);

    pool.PushObject(new User("jack"));
    pool.PushObject(new User("bob"));

    {
        //【Timer】简单计时器，计算当前作用域的生命周期
        Timer timer("test");
        for (int i = 0; i < 1000; i++)continue;
    }

    {
        //【Timer Wheel】简单高精度时间轮定时器(两级时间轮)
        // size 10, interval 1000ms
        TimerWheel timer(10, 1000);
        timer.Start();//循环tick

        // 调用解释
        // 这里interval = 1000ms,所以下面这两个任务分别是3ticks 和 5ticks
        // 对于task1,存储在slot 3中，所以需要耗费3s才运行第一个task
        // 同理task2 存储在slot 5中，在运行完task1之后经过2s运行task2
        // 最后由于size = 10,经过5s后，重新到达slot 0,进入新一轮循环
        timer.AddTask(3000, []() {std::cout << "Task 1" << std::endl; });
        timer.AddTask(5000, []() {std::cout << "Task 2" << std::endl; });

        // 在指定的时间内循环定时执行任务
        std::this_thread::sleep_for(std::chrono::seconds(10));
        // 这里有个问题，虽然主线程Stop了，但是当主线程的对象和变量全部销毁，子线程可能还没立即停止，导致子线程访问资源失败。
        timer.Stop();
        // 需要等待子线程销毁掉，因为子线程访问了主线程的资源
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    {
        //【Timer Heap】时间堆，利用最小堆实现高性能定时器
        TimerHeap th;
        //th.add(0, 2, []() {std::cout << "Timer Heap1"; });
    }

    {
        Timer timer("memory copy");
        //【memcpy】函数重写
        char s[16] = "aabbcc";
        char d[16] = { 0 };

        Memcpy(s + 2, s, 4);
        printf("Memcpy :  %s\n", s);
        strcpy(s, "aabbcc");
        CPPWheels::memcpy(s + 2, s, 4);
        printf("memcpy:   %s\n", s);
    }

    {
        // 随机发牌
        Timer timer("Random Number Generator for Poker Card:");
        // 创建一幅扑克牌
        std::vector<PokerCard> deck;
        std::vector<std::string> suits = { "Club","Diamond","Heart","Spade" };//梅花，方块，红心，黑桃
        for (auto suit : suits)
        {
            for (char rank = '2'; rank <= '9'; ++rank)
            {
                deck.emplace_back(suit, rank);
            }
            // A - ACE, T- 10, J - Jack, Q - Queen, K - King
            deck.emplace_back(suit,'A');
            deck.emplace_back(suit,'J');
            deck.emplace_back(suit,'Q');
            deck.emplace_back(suit,'K');
        }

        //------使用std::shuffle实现洗牌---------
        // 使用随机设备来生成种子  
        std::random_device rd;
        std::mt19937 g(rd()); // 初始化Mersenne Twister随机数生成器  

        // 洗牌（随机排序）  
        std::shuffle(deck.begin(), deck.end(), g);

        //------使用rand()实现洗牌---------
        // srand((unsigned)time(0));为rand设置随机种子，不同时间种子不同，随机序列也不同
        // rand()%52+1 获取1-52的随机数，没有调用srand系统自动调用，并且相同srand随机序列也相同
        

        // 分发
        const int numbers = 4;
        std::vector<std::vector<PokerCard>> hands(numbers);
        for (size_t i = 0; i < deck.size(); i++)
        {
            hands[i % numbers].push_back(deck[i]);
        }

        // 输出结果
        for (int i = 0; i < numbers; i++)
        {
            std::cout << "player" << i + 1 << ":";
            for (const auto& card : hands[i])
            {
                std::cout << card << " ";
            }
            std::cout << std::endl;
        }
    }

    //std::cout << pool.GetObject()->id << std::endl;
    

    system("pause:");
    return 0;
}

