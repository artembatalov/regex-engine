#include "regex/regex_engine.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace regex;

namespace {

void run(const std::string& pattern, const std::set<char>& alphabet,
         const std::vector<std::string>& inputs) {
    RegexEngine engine(pattern, alphabet);
    std::cout << "Pattern: " << pattern << "\n";
    for (const auto& s : inputs) {
        std::cout << "  \"" << s << "\" -> " << (engine.matches(s) ? "match" : "no match") << "\n";
    }
    std::cout << "\n";
}

}

int main() {
    run("(a|b)*abb", {'a', 'b'}, {
        "abb", "aabb", "babb", "ababb", "bbabb",
        "ab", "bb", "ba", "abba", ""
    });

    run("0*(10*10*)*", {'0', '1'}, {
        "", "0", "00", "1", "11", "101", "110", "10",
        "0110", "1001", "111", "100"
    });

    return 0;
}
