#pragma once
#include "dfa.hpp"
#include <set>
#include <string>

namespace regex {

class RegexEngine {
public:
    RegexEngine(const std::string& pattern, const std::set<char>& alphabet);
    bool matches(const std::string& input) const;

private:
    DFA dfa_;
};

}
