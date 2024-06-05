/*
* @author: aspect
* @date: from 2023/09/05 to .
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <thread>
#include <vector>
#include <functional>

#include "Timer.h"
#include "instrumentor.h"
#include "Base.h"

#include "Core.h"

// Object-Oriented Programming
// 封装(Encapsulation)
// 继承(Inheritance)
// 多态(Polymophism)

//--------------------------------Basic Concepts--------------------------------
// 1. 继承的好处：代码重用，功能拓展，实现接口和抽象，实现设计模式
// 2. 多态指的是同一种接口多种行为。
//    静态联编：编译时的多态，例如重载和模板函数，被调函数和执行函数的地址已经确定，速度更快。
//    动态联编：运行时的多态，例如虚函数和纯虚函数，运行函数不是基于指针或引用，而是object,根据对象做出不同编译结果，所以需要程序运行才能决定被调函数和执行函数的关系。
// 3. 封装：增强数据安全性，代码复用性，模块化且耦合性更好(只需要知道接口，不需要具体实现)
// 4. 虚函数机制:
//    每个至少有一个虚函数的类都有一个虚函数表，对象共享同一个类的虚函数表，通过虚指针来查询虚函数，子类的虚函数表中虚函数的地址会根据重写后的变换，父类指针指向子类对象，可以指明实际的函数。之前提到虚函数和纯虚函数都是运行时多态，动态联编，但准确来说，只有指针或引用调用虚函数才表现为多态，如果是值类型调用，(或者编译时可确定函数)，则没有多态。
//--------------------------------Basic Concepts--------------------------------

namespace CppDaydaydream 
{
	// temporary variables
	static bool s_Finished = false;

	// User class for test
	class User
	{
	public:
		User() {};
		std::string Name()const { return name; }
		std::string GetName() const { 
			auto c = (AS_BIND_EVENT_FN(User::Name)); // 获取事件
			return c();
		}
		void SetName() { name = "User1"; }
		
	private:
		std::string name;
	};

	void func(std::string line)
	{
		std::cout << "func() is called at " << line << std::endl;
	}


	// =========================================
	// ===============泛型编程===================
	// 1.函数模板示例
	template<typename T>
	void Print(T a)
	{
		std::cout << a << std::endl;
	}

	template<typename T, typename N>
	void Print(T a, N b)
	{
		std::cout << a << ' ' << b << std::endl;
	}

	// 2.具体函数模板
	//template<> void Print(User& u1, User& u2)
	template<> void Print<User&>(User& u1, User& u2)
	{
		//std::cout << u1.name << ' ' << u2.name << std::endl;
	}

	// 3.decltype关键字
	// decltype(expression) var;
	void test_DeclType()
	{
		//1.
		/*short n1 = 1;
		short& n2 = n1;
		decltype(n2) n3 = n1; // 引用类型需要初始化
		Print(n3);*/

		//2.
		decltype(func(std::to_string(__LINE__)))* p1 = func; // 不会调用
		p1;

		decltype(func)* p2 = func; // 声明为函数指针
		p2(std::to_string(__LINE__));

		//3.
		int a1 = 0;
		decltype((a1)) a2 = a1; // expression = (a1)时等价于引用
		decltype((func)) a3 = func;
		a3(std::to_string(__LINE__));
	}

	// 4. 结合
	// - 4.1 auto Add() -> void 用于后置函数返回类型
	// - 4.2 decltype()用于推导已有类型,下面的示例用于解决无法确定返回类型的问题，只用auto解决不了
	template<typename T1,typename T2>
	auto Add(T1 a, T2 b) -> decltype(a + b)
	{
		return a + b;
	}

	// 5. 类模板,可以指定默认类型实现缺省
	template<class T,class N = std::string>
	class Student
	{
	public:
		Student(T age,N name) : age(age), name(name){}
		void PrintStudentData()
		{
			printf("该学生年龄=%d", age);
			std::cout << "名字是" << name << std::endl;
		}
	private:
		N name;
		T age;
	};

	// 6. 使用类模板实现 栈
	//typedef int DataType;
	template<class DataType>
	class Stack
	{
	public:
		// 构造函数：分配栈内存，初始化栈指针
		Stack(DataType s) : size(s),top(0)
		{
			items = new DataType[s];
		}
		~Stack()
		{
			delete items;
			items = nullptr;
		}
		bool isEmpty()const { return !top; }
		bool isFull()const { return top == size; }
		void push(const int& item){
			if (top < size)
			{
				items[top++] = item;
			}
		}
		DataType pop(){
			if (top > 0)
				return items[--top];
			else return -1; // TODO: repair
		}
		//int top()const { return items[size - 1]; }
	private:
		DataType* items;
		int size;
		int top;
		
	};

	void Test(int a)
	{

	}

	// 7. 线程的使用
	void DoWork()
	{
		using namespace std::literals::chrono_literals;

		while (!s_Finished)
		{
			std::cout << "Working...\n";

			// std::this_thread可以对当前线程进行指令操作
			std::this_thread::sleep_for(1s); // 如果不用这个，就会导致DoWork这个线程CPU占用率100%，不必高速循环
		}
	}

	// 8.函数指针
	void DoWork1(int a)
	{
		std::cout << "Working...Value is:" << a << std::endl;
	}
	// - 传递函数指针作为参数
	/*
	void ForEach(std::vector<int> values, void(*Print)(int))
	{
		for (auto v : values)
			Print(v);
	}*/

	void ForEach(std::vector<int> values, std::function<void(int)> Print)
	{
		for (auto v : values)
			Print(v);
	}
}

int main()
{
	using namespace CppDaydaydream;

	//Print(1.1,2);
	test_DeclType();

	//auto c = Add(1, 1.1);

	// 类模板
	Student<int> s(18,"小明");
	s.PrintStudentData();

	// 自定义stack
	{
		AS_PROFILE_SCOPE("Stack");
		Stack<int> st(3);
		st.push(1);
		st.push(2);
		st.push(3);
		std::cout << "出栈：" << std::endl;
		while (!st.isEmpty())
			std::cout << st.pop() << ' ' << std::endl;;
	}

	// 计时器
	{
		AS_PROFILE_SCOPE("Timer");
		for (int i = 10; i > 0; i--) continue;
	}


	// c++通过宏定义+匿名函数 获得事件(函数变量)
	{
		AS_PROFILE_SCOPE("Bind_Event");
		void (*f)();
		User u;
		u.SetName();
		std::cout << "Get User Name:" << u.GetName() << std::endl;
	}

	// 线程
	{
		/*
		AS_PROFILE_SCOPE("Thread_Test");

		// 下面的示例展示了线程的创建与阻塞，在阻塞之前可以多线程工作（输出的同时等待输入）

		std::thread worker(DoWork); // 创建线程

		std::cin.get(); // 等待输入，这两行代码表示回车后停止DoWork线程
		s_Finished = true;

		worker.join(); // wait,阻塞,直到该线程结束，只要函数调用没有结束，就不会执行下面的代码

		std::cout << "work is done!" << std::endl;
		std::cin.get();
		*/
	}

	// 函数指针
	{
		AS_PROFILE_SCOPE("函数指针");
		// 直接声明函数指针，取别名简化，使用auto自动推导创建
		typedef void(*DoWorkFunc)(int); // 对函数指针取别名
		DoWorkFunc function0 = DoWork1;
		auto function1 = DoWork1; // 此时没有调用函数，而是传递函数指针
		void(*function2)(int) = DoWork1; //声明变量funciton2为函数指针
		function1(8);

		// 函数指针传参+lambda
		std::cout << "Start Print Values" << std::endl;
		std::vector<int> v = { 0,3,4,1 };
		int b = 0;
		ForEach(v, [=](int a) {std::cout << b << std::endl; }); // [capture]通过值传递

		// 迭代器常用的lambda
		auto it = std::find_if(v.begin(), v.end(), [](int value) {return value > 3; });
		std::cout << "第一个大于三的值：" << *it << std::endl;
	}


	{
		// 静态链接，动态链接，制作库
		AS_PROFILE_SCOPE("Make Lib");

		// 要想调用另一个库的函数有以下方法(自己的库）
		// 1.（不推荐）#include"相对路径"，没有采用链接，直接包含了另一个项目的头文件，并且相对路径的使用会很凌乱
		// 2. addtional include direcs + $(SolutionDir)Core\src 直接把代码include进来，就可以使用绝对路径
		// 其他库，动态dll调用，静态include + lib
		Core::Print();
	}
	
	system("pause:");
	return 0;
}