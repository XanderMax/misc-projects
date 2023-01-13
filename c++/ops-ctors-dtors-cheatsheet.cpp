#include <iostream>
#define ADDROF(x) ((void*)x)
#define CHECK_FUNC(x) \
    void x(); \
    static const call_at_ctor x##_var([]() \
    { \
        std::cout << ">> " << #x << ":beg <<" << std::endl; \
        x(); \
        std::cout << ">> " << #x << ":end <<\n" << std::endl; \
    }); \
    void x()

struct call_at_ctor
{
    template <class F> call_at_ctor(F&& f) {f();}
}; // struct call_at_ctor

struct A
{
    int val = 0;
    A() {std::cout << "A::A() this@" << ADDROF(this) << std::endl;}
    A(int val) : val(val) {std::cout << "A::A(int) this@" << ADDROF(this) << " v=" << val << std::endl;}  
    A(const A& o) : val(o.val) {std::cout << "A::A(&) this@" << ADDROF(this) << " v=" << val << std::endl;}
    A(A&& o) : val(o.val) {std::cout << "A::A(&&) this@" << ADDROF(this) << " v=" << val << std::endl;}

    A& operator=(const A& o)
    {
        std::cout << "A::=(&) this@" << ADDROF(this) << " o@" << ADDROF(&o) << std::endl;
        val = o.val;
        return *this;
    }

    A& operator=(A&& o)
    {
        std::cout << "A::=(&) this@" << ADDROF(this) << " o@" << ADDROF(&o) << std::endl;
        val = o.val;
        return *this;
    }

    operator bool()
    {
        std::cout << "A::bool() this@" << ADDROF(this) << std::endl;
        return val;
    }

    operator bool() const
    {
        std::cout << "A::bool() const this@" << ADDROF(this) << std::endl;
        return val;
    }

    operator int()
    {
        std::cout << "A::int() this@" << ADDROF(this) << std::endl;
        return val;
    }

    operator int() const
    {
        std::cout << "A::int() const this@" << ADDROF(this) << std::endl;
        return val;
    }

    auto operator<=>(int v)
    {
        std::cout << "A::<=>(int) this@" << ADDROF(this) << std::endl;
        return val <=> v;
    }

    auto operator<=>(int v) const
    {
        std::cout << "A::<=>(int) const this@" << ADDROF(this) << std::endl;
        return val <=> v;
    }

    auto operator<=>(const A& o)
    {
        std::cout << "A::<=>(int) this@" << ADDROF(this) << " o@" << ADDROF(&o) << std::endl;
        return val <=> o.val;
    }

    auto operator<=>(const A& o) const
    {
        std::cout << "A::<=>(int) const this@" << ADDROF(this) << " o@" << ADDROF(&o) << std::endl;
        return val <=> o.val;
    }
}; // struct A

inline bool operator==(const A& x, const A& y)
{
    std::cout << "==(A&) const x@" << ADDROF(&x) << " y@" << ADDROF(&y) << std::endl;
    return x.val == y.val;
}
inline bool operator==(const A& x, int y)
{
    std::cout << "==(A&) const x@" << ADDROF(&x) << " y=" << y << std::endl;
    return x.val == y;
}
inline bool operator==(int x, const A& y)
{
    std::cout << "==(A&) const x=" << x << " y@" << ADDROF(&y) << std::endl;
    return x == y.val;
}

struct B : public A
{}; //struct B

CHECK_FUNC(check_0)
{
    A a = 1;
    auto b = a == 1;
    b = 1 == a;
}

CHECK_FUNC(check_1)
{
    A a0 = A(1);
    A a1 = a0;
    bool b = (a0 == a1);
    b = a1 == a0;
}

CHECK_FUNC(check_2)
{
    A a;
    A b = std::move(a);
}

CHECK_FUNC(check_3)
{
    B b0, b1;
    bool r = b0 == b1;
    r = b0 == 0;
    r = 0 == b1;
}

CHECK_FUNC(check_4)
{
    const A a;
    int i = a;
}

CHECK_FUNC(check_5)
{
    const B b;
    int i = b;
}

CHECK_FUNC(check_6)
{
    const A a;
    const B b;
    const bool r = a == b;
}

CHECK_FUNC(check_7)
{
    A a;
    B b;
    a = b;
    //b = a; // doesn't compile
}

CHECK_FUNC(check_8)
{
    A a0, a1;
    a0 = a1;
}

CHECK_FUNC(check_9)
{
    B b0, b1;
    b0 = b1;
}

int main() {}

/*
#possible output
g++ --std=c++20 -O0 -o prog ../ops-ctors-dtors-cheatsheet.cpp && ./prog

>> check_0:beg <<
A::A(int) this@0x7ff7b71721e8 v=1
==(A&) const x@0x7ff7b71721e8 y=1
==(A&) const x=1 y@0x7ff7b71721e8
>> check_0:end <<

>> check_1:beg <<
A::A(int) this@0x7ff7b71721e8 v=1
A::A(&) this@0x7ff7b71721e0 v=1
==(A&) const x@0x7ff7b71721e8 y@0x7ff7b71721e0
==(A&) const x@0x7ff7b71721e0 y@0x7ff7b71721e8
>> check_1:end <<

>> check_2:beg <<
A::A() this@0x7ff7b71721e8
A::A(&&) this@0x7ff7b71721e0 v=0
>> check_2:end <<

>> check_3:beg <<
A::A() this@0x7ff7b71721e8
A::A() this@0x7ff7b71721e0
==(A&) const x@0x7ff7b71721e8 y@0x7ff7b71721e0
==(A&) const x@0x7ff7b71721e8 y=0
==(A&) const x=0 y@0x7ff7b71721e0
>> check_3:end <<

>> check_4:beg <<
A::A() this@0x7ff7b71721e8
A::int() const this@0x7ff7b71721e8
>> check_4:end <<

>> check_5:beg <<
A::A() this@0x7ff7b71721e8
A::int() const this@0x7ff7b71721e8
>> check_5:end <<

>> check_6:beg <<
A::A() this@0x7ff7b71721e8
A::A() this@0x7ff7b71721e0
==(A&) const x@0x7ff7b71721e8 y@0x7ff7b71721e0
>> check_6:end <<

>> check_7:beg <<
A::A() this@0x7ff7b71721e8
A::A() this@0x7ff7b71721e0
A::=(&) this@0x7ff7b71721e8 o@0x7ff7b71721e0
>> check_7:end <<

>> check_8:beg <<
A::A() this@0x7ff7b71721e8
A::A() this@0x7ff7b71721e0
A::=(&) this@0x7ff7b71721e8 o@0x7ff7b71721e0
>> check_8:end <<

>> check_9:beg <<
A::A() this@0x7ff7b71721e8
A::A() this@0x7ff7b71721e0
A::=(&) this@0x7ff7b71721e8 o@0x7ff7b71721e0
>> check_9:end <<
*/