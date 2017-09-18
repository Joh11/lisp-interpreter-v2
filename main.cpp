#include "main.hpp"

#include "tree.hpp"
#include "parser.hpp"

int main()
{
    auto tokens = parser::tokenize(input());

    display_tokens(tokens);
    
    std::cout << std::endl;
	
    auto ast{parser::parse(tokens)};
    
    std::cout << ast << std::endl;
    ast.display(std::cout);

    return 0;
}

std::string input()
{
    std::string str;
    std::getline(std::cin, str);

    return str;
}

void display_tokens(parser::token_list const& tokens)
{
    for(auto const& t : tokens)
	std::cout << t << "__";
}

void append_test()
{
    tree::ast t1{tree::empty_branch()};
    std::cout << "The following tree should be (nil . nil)" << std::endl;
    std::cout << t1 << std::endl;

    // We append "test"
    t1.append({"test"});
    std::cout << "The following tree should be (nil . (\"test\" . nil))" << std::endl;
    std::cout << t1 << std::endl;

    // We append "test 2"
    t1.append({"test 2"});
    std::cout << "The following tree should be (nil . (\"test\" . (\"test 2\" . nil)))" << std::endl;
    std::cout << t1 << std::endl;

    // We append itself ?
    t1.append(t1);
    std::cout << "The following tree should be (nil . (\"test\" . (\"test 2\" . (nil . (\"test\" . (\"test 2\" . nil))))))" << std::endl;
    std::cout << t1 << std::endl;
}
