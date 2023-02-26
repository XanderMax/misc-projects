#include <iostream>

namespace
{
const char* g_greetingText = "Hello, Human I'm a Library1";
} // namespace

namespace mylib1
{
void printGreeting(int num)
{
    std::cout << g_greetingText << ' ' << num << std::endl;
}
} // namespace mylib1