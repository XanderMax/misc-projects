#include <iostream>

struct SomeStruct
{
    SomeStruct(const char* c) {std::cout << "BYE: " << c << std::endl;}
};

static SomeStruct someStruct("object.cpp");