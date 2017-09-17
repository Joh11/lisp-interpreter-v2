#include <string>
#include <iostream>

#include "tree.hpp"
#include "parser.hpp"

int main()
{

    //std::string str;
    //std::getline(std::cin, str);

    //std::cout << parser::parse(parser::tokenize(str));

    tree::ast t1{tree::ast{"lorem"}, tree::ast{"ipsum"}};    
    tree::ast t2{{"+"}, {{"a"}, {{{"-"}, {{"1"}, {{"2"}, {"nil"}}}}, {"nil"}}}};

    std::cout << t1 << std::endl << t2;

    return 0;
}
