#include <iostream>

void f()
{
    std::cout << "f()" << std::endl;
}

struct SomeStruct
{
    SomeStruct(const char* c) {std::cout << "HELLO:" << c << std::endl;}
};

static SomeStruct someStruct("shared.cpp");