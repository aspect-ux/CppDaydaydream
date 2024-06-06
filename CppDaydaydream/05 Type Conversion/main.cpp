#include <iostream>
// cpp type conversion(c++ 类型转换)
// 主要分为两种
// 1. 隐式转换, 由编译器自动完成, 较大类型往较小类型转换容易出现数据丢失
// 2. 显示转换(强转)
namespace CppDaydaydream
{

}

int main()
{
    // 1. 隐式转换
    double num_double = 10;
    int num_int = num_double;
    std::cout << "num double size:" << sizeof(num_double) << "bytes." << std::endl;//sizeof获取对象或类型所占字节数
    std::cout << "after [Implicitly] converting from double to int:" << sizeof(num_int) << "bytes." << std::endl;

    // 2. 显示值转换(强制类型转换)
    // 2.1 c风格强转
    num_double = (double)num_int;
    // 2.2 老式c++函数式风格
    num_int = int(num_double);
    // 2.3【重要】 类型转换运算符
    // ref: https://www.cainiaojc.com/cpp/cpp-casting-operators.html
    // const_cast，用于指针或引用的转换
    // static_cast, 1)基本类型如int,float,struct,enum之间的相互转换。2)能够进行父类指针和子类指针之间的相互转换，但是父类转子类不安全。
    // dynamic_cast，安全地将基类指针或引用转换成派生类指针或引用，失败抛出异常，父类一定要有虚函数
    // reinterpret_cast，将数据二进制形式重新解释，与c风格强转类似，指针和内置类型都可以互相转，有安全隐患。


    //在我们日常使用中，const_cast可以用用来修改最初声明非const的值，而且应该尽量避免常量转换，除非我们真的需要使用它。
    const int j = 3; // 声明 j 为 const
    int* pj = const_cast<int*>(&j);
    *pj = 4;      // 未定义行为
    std::cout << "j = " << j << '\n';    //输出结果：j = 3
    std::cout << "*pj = " << *pj << '\n';  //输出结果：*pj = 4

    // volatile易变的
    // 1. 与硬件交互时，保证直接从硬件寄存器读取数据而不是缓存
    // 2. 多线程/中断服务... 过度使用可能会导致性能下降



    system("pause:");
    return 0;
}