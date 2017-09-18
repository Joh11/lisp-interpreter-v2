#include <cctype>
#include <stdexcept>

#include "parser.hpp"

namespace parser
{
    std::string specialChars{"().'`,"};
    
    bool in_string(char c, std::string const& s)
    {
	return s.find(c) != std::string::npos;
    }

    bool next_is_special(std::string::const_iterator & it, std::string const& str)
    {
	++it;
	if(it == str.end())
	{
	    --it;
	    return false;
	}
	
	if(in_string(*it, specialChars))
	{
	    --it;
	    return true;
	}

	--it;
	return false;
    }
    
    token_list tokenize(std::string const& str)
    {
	token_list tokens;
	std::string currentToken;

	bool inString{false};

	for(auto it{str.begin()} ; it != str.end() ; ++it)
	{
	    auto const& c{*it};
	    
	    if(inString)
	    {
		// In a string add everything
		currentToken += c;

		if(c == '"')
		{
		    // We go to the next token and set inString to false
		    tokens.push_back(currentToken);
		    currentToken = "";

		    inString = false;
		}
	    }
	    else
	    {
		bool whitespace{isspace(c)};
		bool specialChar{in_string(c, specialChars)};
		bool nextIsSpecial{next_is_special(it, str)};

		if(!whitespace)
		    currentToken += c;

		if(whitespace || specialChar || nextIsSpecial)
		{
		    // If the token is nonempty we add it
		    if(!currentToken.empty())
		    {
			tokens.push_back(currentToken);
			currentToken = "";
		    }
		}

		// Finally if we enter a string
		if(c == '"')
		    inString = true;
	    }
	}

	return tokens;
    }

    tree::ast parse(token_list const& tokens)
    {
	std::stack<tree::ast> trees;
	trees.push(tree::ast{"parsing"});
	
	// For now don't care about the quotes and backquotes
	for(auto const& t : tokens)
	{
	    if(t == "(")
	    {
		trees.push(tree::ast{"parsing"});
	    }
	    else if(t == ")")
	    {
		// We append the current tree to the one before
		auto sub{trees.top()};
		trees.pop();
		trees.top().append(sub);
	    }
	    else
	    {
		trees.top().append(tree::ast{t});
	    }
	}

	if (trees.size() != 1)
	    throw std::runtime_error("Failed to parse");

	return trees.top();
    }
}
