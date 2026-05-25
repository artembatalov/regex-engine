#include "regex/regex_engine.hpp"
#include "regex/nfa.hpp"
#include "regex/parser.hpp"

namespace regex {

RegexEngine::RegexEngine(const std::string& pattern, const std::set<char>& alphabet) {
    Parser parser(pattern);
    auto ast = parser.parse();
    auto nfa = buildNFA(ast);
    dfa_ = buildDFA(nfa, alphabet);
}

bool RegexEngine::matches(const std::string& input) const {
    return dfa_.accepts(input);
}

}
