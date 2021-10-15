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

int main(int argc, char** argv) {
    std::cout << LINK << std::endl;

    if (argc > 1) {
        std::cout << longestPlndrm(argv[1]) << std::endl;
        return 0;
    }
    
    test({.input = "",                  .expected = ""});
    test({.input = "aaaaa",             .expected = "aaaaa"});
    test({.input = "a",                 .expected = "a"});
    test({.input = "aa",                .expected = "aa"});
    test({.input = "aba",               .expected = "aba"});
    test({.input = "aaa",               .expected = "aaa"});
    test({.input = "abcdef",            .expected = "a"});
    test({.input = "abcddd",            .expected = "ddd"});
    test({.input = "saaaas",            .expected = "saaaas"});
    test({.input = "aabbbcccccbbdd",    .expected = "bbcccccbb"});
    test({.input = "abcdcbavvabcdcba",  .expected = "abcdcbavvabcdcba"});
    test({.input = "abcdcbavvabcdcb",   .expected = "bcdcbavvabcdcb"});
    test({.input = "abcdcbavabcdcb",    .expected = "bcdcbavabcdcb"});
    test({.input = "abcdxcbavabcdcb",   .expected = "cbavabc"});

    return 0;
}

std::string longestPlndrm(const std::string& input) {
   
    if (input.size() <= 1) return input;
   
    struct plndrm {size_t beg = 0; size_t end = 0;};
   
    std::vector<plndrm> palindromes;
   
    size_t size = input.size();
   
    for (size_t i = 0; i < size; ++i) {
        if (size > i + 2 && input[i] == input[i + 2]) {
            palindromes.emplace_back(plndrm{.beg = i, .end = i + 2});
        }
        if (size > i + 1 && input[i] == input[i + 1]) {
            palindromes.emplace_back(plndrm{.beg = i, .end = i + 1});
        }
    }

    plndrm maxP;
    for (auto& p : palindromes) {
        while (p.beg > 0 && p.end < input.size() - 1) {
            if (input[p.beg - 1] != input[p.end + 1]) break;
            p.beg--;
            p.end++;
        }
        if (maxP.end - maxP.beg < p.end - p.beg) maxP = p;
    }
   
    return input.substr(maxP.beg, maxP.end - maxP.beg + 1);
}