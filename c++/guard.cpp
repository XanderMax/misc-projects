#include <iostream>
#include <functional>
#include <memory>

template <class TFunc> class guarded_func
{
    TFunc functor;
    const std::weak_ptr<bool> flag;
    friend class guard;

    template <class F> guarded_func(F&& functor, const std::weak_ptr<bool>& flag)
    : functor(std::forward<F>(functor))
    , flag(flag)
    {}
public:
    template <class ... TArgs> void operator() (TArgs&&... args)
    {
        if (!flag.expired())
        {
            functor(std::forward<TArgs>(args)...);
        }
    }
}; // class guarded_func

class guard
{
    std::shared_ptr<bool> flag;
public:
    guard()
    : flag(std::make_shared<bool>(true))
    {
    }
    
    guard(const guard&) = delete;
    guard& operator=(const guard&) = delete;

    template <class TFunc> auto bind(TFunc&& functor)
    {
        return guarded_func<TFunc>{std::forward<TFunc>(functor), flag};
    }
}; // class guard

void callFunc(const std::function<void(const std::string&)>& f, const char* cstr)
{
    f(cstr);
}

int main()
{
    const auto lambda = [](const std::string& s){std::cout << s << std::endl;};
    
    {
        std::function<void(const std::string&)> f = nullptr;
        {
            guard g;
            f = g.bind(lambda);
            f("use case 1: within scope!");
        }
        f("use case 1: out of scope - not printed");
    }

    {
        guard g;
        callFunc(g.bind(lambda), "use case 2!");
    }

    {
        guard g;
        callFunc(g.bind([](const auto& str) {std::cout << "use case 3" << str << std::endl;}), "!");
    }

    {
        guard g;
        std::function<void(const std::string&)> f = nullptr;
        f = g.bind([](const auto& str) {std::cout << "use case 4" << str << std::endl;});
        callFunc(f, "!");
    }

    {
        guard g;
        const auto f = g.bind([](const auto& str) {std::cout << "use case 5" << str << std::endl;});
        callFunc(f, "!");
    }
}