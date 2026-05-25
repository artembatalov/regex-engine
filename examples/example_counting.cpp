#include "regex/regex_engine.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace regex;

namespace {

std::vector<std::string> enumerate(const std::vector<char>& alphabet, int maxLen) {
    std::vector<std::string> result = {""};
    std::vector<std::string> layer = {""};
    for (int i = 0; i < maxLen; ++i) {
        std::vector<std::string> next;
        for (const auto& s : layer) {
            for (char c : alphabet) {
                next.push_back(s + c);
                result.push_back(s + c);
            }
        }
        layer = std::move(next);
    }
    return result;
}

}

int main() {
    const std::string pattern = "ab*c|d";
    const std::set<char> alphabet = {'a', 'b', 'c', 'd'};
    const int maxLen = 6;

    RegexEngine engine(pattern, alphabet);

    std::cout << "Regex: " << pattern << "\n";
    std::cout << "Alphabet: {a, b, c, d}\n";
    std::cout << "Accepted words of length <= " << maxLen << ":\n\n";

    int count = 0;
    for (const auto& s : enumerate({'a', 'b', 'c', 'd'}, maxLen)) {
        if (engine.matches(s)) {
            std::cout << "  \"" << s << "\"\n";
            ++count;
        }
    }

    std::cout << "\nTotal: " << count << "\n";
    return 0;
}
