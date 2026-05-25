#include <gtest/gtest.h>
#include "regex/regex_engine.hpp"
#include <regex>
#include <string>
#include <vector>

using namespace regex;

namespace {

std::vector<std::string> generateStrings(const std::vector<char>& alphabet, int maxLen) {
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

TEST(MatchingTest, AbbPatternAgainstStdRegexUpToLength8) {
    RegexEngine engine("(a|b)*abb", {'a', 'b'});
    std::regex ref("(a|b)*abb");

    for (const auto& s : generateStrings({'a', 'b'}, 8)) {
        bool mine = engine.matches(s);
        bool expected = std::regex_match(s, ref);
        EXPECT_EQ(mine, expected) << "Mismatch on: \"" << s << "\"";
    }
}

TEST(MatchingTest, BinaryEvenOnesAgainstStdRegexUpToLength10) {
    RegexEngine engine("0*(10*10*)*", {'0', '1'});
    std::regex ref("0*(10*10*)*");

    for (const auto& s : generateStrings({'0', '1'}, 10)) {
        bool mine = engine.matches(s);
        bool expected = std::regex_match(s, ref);
        EXPECT_EQ(mine, expected) << "Mismatch on: \"" << s << "\"";
    }
}

TEST(MatchingTest, CountAcceptedWordsAbcOrDUpToLength6) {
    RegexEngine engine("ab*c|d", {'a', 'b', 'c', 'd'});

    int count = 0;
    for (const auto& s : generateStrings({'a', 'b', 'c', 'd'}, 6)) {
        if (engine.matches(s)) {
            ++count;
        }
    }
    EXPECT_EQ(count, 6);
}
