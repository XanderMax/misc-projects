#include <iostream>

int g_someGlobalInt = 10;
const char* g_greetingText = "Hello, Human I'm an Executable";

bool printGreeting();

namespace mylib0 {void printGreeting(int);}
namespace mylib1 {void printGreeting(int);}

int main(int, char**)
{
    printGreeting();
    mylib0::printGreeting(g_someGlobalInt);
    mylib1::printGreeting(g_someGlobalInt);
}

bool printGreeting()
{
    if (!g_greetingText)
        return false;

    std::cout << g_greetingText << ' ' << g_someGlobalInt << std::endl;
    return true;
}