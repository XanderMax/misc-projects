#include <iostream>

namespace
{
const char* g_greetingText = "Hello, Human I'm a Library0";
} // namespace

namespace mylib0
{
void printGreeting(int num)
{
    std::cout << g_greetingText << ' ' << num << std::endl;
}
} // namespace mylib0