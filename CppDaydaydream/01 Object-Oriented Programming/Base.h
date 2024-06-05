#pragma once

// 1. 普通宏,可以是常量，函数，类型
#define PI 3.1415926

// 2. 带参函数
#define SQUARE(x) (x * x)

// 3. 条件编译，只定义，通过if else控制开关
#define DEBUG


// []表示捕获（capture）上下文变量，[=]按值，[&]按引用，[]什么也不捕获; this指针只能在成员函数中使用,不能在静态函数中使用
#define AS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }