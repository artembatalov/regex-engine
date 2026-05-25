#pragma once
#include "ast.hpp"
#include <stdexcept>
#include <string>

namespace regex {

class Parser {
public:
    explicit Parser(std::string pattern);
    ASTNodePtr parse();

private:
    ASTNodePtr parseRegex();
    ASTNodePtr parseTerm();
    ASTNodePtr parseFactor();
    ASTNodePtr parseAtom();

    char peek() const;
    char consume();
    bool atEnd() const;

    std::string pattern_;
    size_t pos_;
};

}
