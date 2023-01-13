#include <iostream>

void f();

int main()
{
    f();
}

/*
g++ -fPIC -shared -o libshared.so shared.cpp
g++ -L${PWD} -o prog main.cpp -lshared
g++ -L${PWD} -o prog main.cpp -lshared object.cpp

>>> LD_LIBRARY_PATH=. ./prog 
BYE: shared.cpp
BYE: object.cpp
f()
*/
