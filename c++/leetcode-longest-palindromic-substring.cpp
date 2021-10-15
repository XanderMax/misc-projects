#include <iostream>
#include <string>
#include <vector>

static const char* LINK = "https://leetcode.com/problems/longest-palindromic-substring";

std::string longestPlndrm(const std::string&);

struct testcase {
    const char* input;
    const char* expected;
};

void test(const testcase& tc) {
    auto res = longestPlndrm(tc.input);
    if (res != tc.expected) {
        std::cout << "for input '" << tc.input    << "' test has failed\n";
        std::cout << "expected: '" << tc.expected << "'\n";
        std::cout << "actual: '"   << res         << "'\n====" << std::endl;
    }
}

int main() {
    std::cout << LINK << std::endl;
    test({.input = "aaaaa",             .expected = "aaaaa"});
    test({.input = "a",                 .expected = "a"});
    test({.input = "abcdef",            .expected = "a"});
    test({.input = "",                  .expected = ""});
    test({.input = "abcddd",            .expected = "ddd"});
    test({.input = "aabbbcccccbbdd",    .expected = "bbcccccbb"});
    test({.input = "abcdcbavvabcdcba",  .expected = "abcdcbavvabcdcba"});
    test({.input = "abcdcbavvabcdcb",   .expected = "bcdcbavvabcdcb"});
    test({.input = "abcdcbavabcdcb",    .expected = "bcdcbavabcdcb"});
    test({.input = "abcdxcbavabcdcb",   .expected = "cbavabc"});  
}

std::string longestPlndrm(const std::string& input) {
   
    if (input.size() <= 1) return input;
   
    struct plndrm {size_t beg = 0; size_t end = 0;};
   
    std::vector<plndrm> palindromes;
   
    size_t size = input.size();
   
    for (size_t i = 0; i < size; ++i) {
        if (size > i + 2 && input[i] == input[i + 2]) {
            palindromes.emplace_back(plndrm{.beg = i, .end = i + 2});
        } else if (size > i + 1 && input[i] == input[i + 1]) {
            palindromes.emplace_back(plndrm{.beg = i, .end = i + 1});
        }
    }
   
    if (palindromes.empty()) return input.substr(0, 1);
   
    for (auto& p : palindromes) {
        while (true) {
            if (p.beg == 0 || p.end == input.size() - 1) break;
           
            if (input[p.beg - 1] != input[p.end + 1]) break;
           
            p.beg--;
            p.end++;
        }
    }
   
    plndrm maxP;
    for (const auto& p : palindromes) {
        if (maxP.end - maxP.beg < p.end - p.beg) maxP = p;
    }
   
    if (maxP.end == maxP.beg) return "";
   
    return input.substr(maxP.beg, maxP.end - maxP.beg + 1);
}