#include <iostream>
#include <iterator>
#include <vector>

static const char* LINK = "https://leetcode.com/problems/remove-element";

size_t removeAll(std::vector<size_t>&, size_t n = 0);
std::ostream& operator<<(std::ostream&, const std::vector<size_t>&);

struct testcase {
    const std::vector<size_t> input;
    const std::vector<size_t> expected;
};

void test(const testcase& tc);

int main() {
    std::cout << LINK << std::endl;

    test({.input = {},                  .expected = {}});
    test({.input = {0},                 .expected = {}});
    test({.input = {1},                 .expected = {1}});
    test({.input = {1, 2, 3, 4, 5},     .expected = {1, 2, 3, 4, 5}});
    test({.input = {1, 2, 3, 4, 0},     .expected = {1, 2, 3, 4}});
    test({.input = {1, 0, 3, 0, 5},     .expected = {1, 3, 5}});
    test({.input = {0, 2, 3, 4, 5},     .expected = {2, 3, 4, 5}});
    test({.input = {1, 0, 2, 3, 4},     .expected = {1, 2, 3, 4}});
    test({.input = {1, 0, 0, 2, 3},     .expected = {1, 2, 3}});
    test({.input = {1, 0, 2, 0, 0, 3},  .expected = {1, 2, 3}});
    test({.input = {0, 0, 0, 0, 0, 0},  .expected = {}});
    test({.input = {0, 0, 0, 0, 0, 5},  .expected = {5}});
}

size_t removeAll(std::vector<size_t>& v, size_t n)
{
    auto pivot = v.end();
    auto end = pivot;
    
    for (auto it = v.begin();  it != end; ++it) {
        
        if (*it == n)
        {
            if (pivot == end) pivot = it;
        }
        else if (pivot != end)
        {
            *pivot = *it;
            ++pivot;
        }  
    }
    
    size_t finalSize = v.size() - std::distance(pivot, end);
    v.resize(finalSize);
    
    return finalSize;
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

void test(const testcase& tc) {
    auto res = tc.input;
    removeAll(res);
    if (res != tc.expected) {
        std::cout << "for input '" << tc.input << "' test has failed\n";
        std::cout << "expected: '" << tc.expected << '\n';
        std::cout << "actual: '"   << res         << "'\n====" << std::endl;
    }
    else {
        std::cout << "PASSED: " << tc.expected << " == " << res << std::endl;
    }
}