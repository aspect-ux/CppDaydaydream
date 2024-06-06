#include <iostream>
#include "ObjectPool.h"

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
    using namespace CPPTools;

    ObjectPool<User> pool = ObjectPool<User>(1000);

    pool.PushObject(new User("jack"));
    pool.PushObject(new User("bob"));

    //std::cout << pool.GetObject()->id << std::endl;

    system("pause:");
    return 0;
}