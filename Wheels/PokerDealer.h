#pragma once
#include <iostream>
#include <string>

namespace CPPWheels
{
    //push_back和emplace_back的区别
    // 1. 两种都用于stl的容器末尾添加元素
    // 2. emplace_back强调原地构造，面对大型对象的时性能更好，不需要额外复制
    // 3. push_back一般用于元素的值或右值引用，通过复制已有对象来添加末尾元素
    // 
    // A Poker Dealer
    // 扑克牌共计52张(无大小王),四个花色每个花色13张牌
    class PokerCard
    {
    public:
        PokerCard(std::string s, char r):suit(s),rank(r){}
        friend std::ostream& operator<<(std::ostream& os, const PokerCard& c)
        {
            os << c.rank << " of " << c.suit;
            return os;
        }
    public:
        // 花色: Heart(红心),Diamond(方块),Club(梅花),Spade(黑桃)
        std::string suit;
        // 点数
        char rank;
    };
}