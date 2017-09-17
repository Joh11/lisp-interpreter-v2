#include <cctype>
#include <stdexcept>

#include "parser.hpp"

namespace parser
{
    bool in_string(char c, std::string const& s)
    {
	return s.find(c) != std::string::npos;
    }
    
    token_list tokenize(std::string const& str)
    {
	token_list tokens;
	std::string currentToken;

	bool inString{false};

	std::string specialChars{"().'`,"};

	for(auto const c : str)
	{
	    if(inString)
	    {
		// In a string add everything but the double quotes
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

		if(!whitespace)
		    currentToken += c;

		if(whitespace || specialChar)
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
	
	// For now don't care about the quotes and backquotes
	for(auto const& t : tokens)
	{
	    if(t == "(")
	    {
		trees.push(tree::empty_branch());
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

	return trees.top().right();
    }
}
