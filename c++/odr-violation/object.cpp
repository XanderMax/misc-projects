#include <iostream>

struct SomeStruct
{
    SomeStruct() {std::cout << "BYE" << std::endl;}
};

static SomeStruct someStruct;