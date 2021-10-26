#include <iostream>
#include <vector>
#include <utility>

struct testcase {
    const std::pair<std::vector<size_t>, size_t> input;
    const std::vector<size_t> expected;
};

void test(const testcase& tc);

std::vector<size_t> replace(std::vector<size_t>&, size_t);

std::ostream& operator<<(std::ostream&, const std::vector<size_t>&);

int main(int argv, char** argc)
{
    test({.input = {{},                 0},     .expected = {}});
    test({.input = {{},                 5},     .expected = {}});
    test({.input = {{1,2},              0},     .expected = {1,2}});
    test({.input = {{1,2,3},            0},     .expected = {2,3,1}});
    test({.input = {{1,2,3},            1},     .expected = {3,1,2}});
    test({.input = {{0,1,2,3},          1},     .expected = {2,3,0,1}});
    test({.input = {{0,1,2,3,4},        2},     .expected = {3,4,0,1,2}});
    test({.input = {{0,1,2,3,4},        5},     .expected = {0,1,2,3,4}});
    test({.input = {{0,1,2,3,4},        4},     .expected = {0,1,2,3,4}});
    test({.input = {{0,1,2,3,4},        3},     .expected = {4,0,1,2,3}});
    test({.input = {{0,1,2,3,4},        0},     .expected = {1,2,3,4,0}});
    test({.input = {{0,1,2,3,4},        0},     .expected = {1,2,3,4,0}});
    test({.input = {{0,1,2,3,4},        1},     .expected = {2,3,4,0,1}});
    test({.input = {{1,2,3,4,5,6,7,8},  2},     .expected = {4,5,6,7,8,1,2,3}});
}

std::ostream& operator<<(std::ostream& str, const std::vector<size_t>& v)
{
    str << "(";
    for (auto& n : v) {
        str << n << " ";
    }
    str << "):" << v.size();
   
    return str;
}

std::vector<size_t> replace(std::vector<size_t>& vec, size_t m)
{
    if (m >= vec.size() || vec.size() < 3) return vec;

    auto leftRotate = [](auto& arr) {
        if (arr.empty()) return;
        auto tmp = arr[0];
        for(size_t i = 0; i < arr.size() - 1; ++i)  {
            arr[i] = arr[i + 1];
        }
        arr.back() = tmp;
    };

    auto rightRotate = [](auto& arr) {
        if (arr.empty()) return;
        auto tmp = arr.back();
        for(size_t i = arr.size() - 1; i != 0; --i)  {
            arr[i] = arr[i - 1];
        }
        arr.front() = tmp;
    };

    bool left = m + 1 > vec.size() - m - 1;
    size_t steps = left ? m + 1 : vec.size() - m - 1;
    
    for (size_t j = 0; j < steps; ++j) {
        if (left) {
            leftRotate(vec);
        } else {
            rightRotate(vec);
        }
    }
    return vec;
}

void test(const testcase& tc) {
    auto m = tc.input.second;
    auto in = tc.input.first;
    auto res = replace(in, m);
    if (res != tc.expected) {
        std::cout << "for input '" << tc.input.first << "' and m=" << m << " test has failed\n";
        std::cout << "expected: '" << tc.expected << '\n';
        std::cout << "actual: '"   << res         << "'\n====" << std::endl;
    }
    else {
        std::cout << "PASSED: " << tc.expected << " == " << res << ", " << m << std::endl;
    }
}
