#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>
#include <iostream>

#include "parser.hpp"

std::string input();
void append_test();
void display_tokens(parser::token_list const& tokens);

#endif
