#include <iostream>

void f()
{
    std::cout << "f()" << std::endl;
}

struct SomeStruct
{
    SomeStruct() {std::cout << "HELLO" << std::endl;}
};

static SomeStruct someStruct;