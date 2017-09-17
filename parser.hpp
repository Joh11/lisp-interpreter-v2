#ifndef PARSER_HPP
#define PARSER_HPP

#include <list>
#include <string>
#include <stack>

#include "tree.hpp"

// A namespace used to parse raw text and build an AST (Abstract Syntax Tree)
namespace parser
{
    using token_list = std::list<std::string>;

    bool in_string(char c, std::string const& s);

    // The function that is used to cut the raw text in chunks
    // i.e. "(concatenate (to-string (+ 2 3)) "euros")"
    // ---> "(", "concatenate", "(", "to-string", "(", "+", "2", "3", ")", ")", "euros", ")"
    token_list tokenize(std::string const& str);

    // Convert the token list in an AST
    tree::ast parse(token_list const& tokens);
}

#endif
