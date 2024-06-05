#include <iostream>
#include <memory>

//--------------------Smart pointer--------------------------
//Q: 1. 为什么引入智能指针？
//A: 因为在普通指针的拷贝的时候，编译器默认进行shallow copy(浅拷贝),只拷贝值，指针指向地址不变，这样如果指向同一地址的指针其中一个析构，另一个就会发生指针悬挂，可能导致程序崩溃。智能指针于是产生。
//Q：2. 智能指针有哪些？又是如何解决上述问题？
//A：【obsolete】c++98有auto_ptr,通过权限转让的方式，但是因为容易造成指针悬挂被弃用。
//   c++11中的三种智能指针
//   2.1 unique_ptr,同一时间段只有一个指针管理对象，一旦离开unique_ptr作用域，自动销毁。RAII(资源获取即初始化)思想，通过std::move来传递转让
//   2.2 shared_ptr,计数机制，最后一个指针被析构才析构
//      2.2.1 shared_ptr线程安全问题，1）同一个shared_ptr被多个线程“读”是安全的；2）同一个shared_ptr被多个线程“写”是不安全的；3）共享引用计数的不同的shared_ptr被多个线程”写“ 是安全的；
//   2.3 weak_ptr,由于shared_ptr会出现循环引用的问题，例如链表类型的shared_ptr,两个节点相互引用，都在等对方先释放，进而都无法释放。
//它**不影响共享资源的生命周期**：1. 如果需要使用 weak_ptr 正在观察的资源，可以将 weak_ptr 提升为 shared_ptr。2. 当 shared_ptr 管理的资源被释放时，weak_ptr 会自动变成 nullptr
//只要 weak_ptr 对象还存在，控制块就会保留，weak_ptr 可以通过控制块观察到对象是否存活。

namespace CppDaydaydream
{
    template<typename T>
    class Shared_Ptr
    {
    private:
        mutable T* m_Instance;
        //int* count;//不同类型共享引用计数，还是另取一个类做基类单独计数，游戏引擎中类多且复杂，往往需要单独计数
        mutable std::atomic<uint32_t> m_Count = 0;
    public:
        // ---------------------constructor/deconstructor------------------------
        Shared_Ptr():m_Instance(nullptr){}
        Shared_Ptr(std::nullptr_t n):m_Instance(nullptr){}

        // copy constructor1,普通指针拷贝
        Shared_Ptr(T* instance) : m_Instance(instance)
        {
            // 检查类型
            //static_assert(std::is_base_of<T>, "this is not shared ptr!");
            // 自增
            IncreaseCount();
        }
        // copy constructor2，同类型智能指针拷贝
        Shared_Ptr(const Shared_Ptr<T>& other):m_Instance(other.m_Instance)
        {
            // 自增
            IncreaseCount();
        }
        // copy constructor3，不同类型智能指针拷贝
        template<typename T2>
        Shared_Ptr(const Shared_Ptr<T2>& other) {
            m_Instance = (T*)other.m_Instance;
            // 自增
            IncreaseCount();
        }

        // 一般对大型对象或资源密集对象的拷贝，为了节省性能，将资源移动到另一个对象而不是重新创建
        // 总结
        // 1. 对象包含大量数据
        // 2. 对象即将被销毁，但是部分数据需要保留，直接move
        template<typename T2>
        Shared_Ptr(const Shared_Ptr<T2>&& other) {
            m_Instance = (T*)other.m_Instance;
            
            other.m_Instance = nullptr;
        }

        //析构函数
        ~Shared_Ptr()
        {
            DecreaseCount();
        }

        // ---------------------END constructor/deconstructor------------------------

        // ---------------------counter------------------------
        // decrease/increase count
        void IncreaseCount()
        {
            if (m_Instance)
            {
                m_Count++;
                //AspectRefUtils::AddToLiveAspectReferences((void*)m_Instance);
            }
            
        }
        void DecreaseCount()
        {
            if (m_Instance)
            {
                m_Count--;
                if (m_Count == 0)
                {
                    delete m_Instance;
                    //AspectRefUtils::RemoveFromLiveAspectReferences((void*)m_Instance);
                    m_Instance = nullptr;
                }
            }
        }
        // ---------------------END counter------------------------


        // operator override
        // operator =
        // 将A赋值给B，那么指针A的引用计数增加
        Shared_Ptr<T>& operator=(const Shared_Ptr<T>& other)
        {
            other.IncreaseCount();
            DecreaseCount();

            m_Instance = other.m_Instance;
            return *this;
        }

        // 不同类型智能指针赋值，浅拷贝，仍然由A来管理B
        template<typename T2>
        Shared_Ptr<T>& operator=(const Shared_Ptr<T2>& other)
        {
            other.IncreaseCount();
            DecreaseCount();

            m_Instance = other.m_Instance;//引用赋值
            return *this;
        }
        //大型对象赋值，移动赋值运算符重载
        template<typename T2>
        Shared_Ptr<T>& operator=(const Shared_Ptr<T2>&& other)
        {
            DecreaseCount();

            m_Instance = other.m_Instance;//移动赋值

            other.m_Instance = nullptr;
            return *this;
        }

        // operator bool
        operator bool() { return m_Instance != nullptr; }
        operator bool() const { return m_Instance != nullptr; }

        // 成员选择运算符
        T* operator->() { return m_Instance; }
        const T* operator->() const { return m_Instance; }

        // 指针访问器
        T& operator*() { return *m_Instance; }
        const T& operator*() const { return *m_Instance; }


        //others
        uint32_t GetCount() const { return m_Count.load(); }
    };

    class User {
    public:
        User(int id,const std::string& name):id(id),name(name){}
    public:
        int id;
        std::string name;
    };
}
int main()
{
    using namespace CppDaydaydream;
    User* user = new User(0,"jack");

    //1. 普通指针转智能指针
    Shared_Ptr<User> smart_user = Shared_Ptr<User>(user);

    std::cout <<"Customed smart pointer output:"<< smart_user->id << std::endl;


    system("pause:");
    return 0;
}