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
	std::stack<std::pair<tree::ast, bool> > trees;
	trees.push({{"parsing"}, false});
	
	// For now don't care about the quotes and backquotes
	for(auto const& t : tokens)
	{
	    if(t == "(")
	    {
		trees.push({tree::ast{"parsing"}, false});
	    }
	    else if(t == ")")
	    {
		// We append the current tree to the one before
		tree::ast sub{trees.top().first};
		trees.pop();

		if(trees.top().second) // If dot is true we dot it instead of append
		{
		    trees.top().first.left() = sub;
		    trees.top().second = false;
		}
		else
		    trees.top().first.append(sub);
	    }
	    else if(t == ".")
	    {
		// The next value should be dotted to the previous instead of appended
		trees.top().second = true;
	    }
	    else
	    {
		if(trees.top().second)
		{
		    trees.top().first.right() = tree::ast{t};
		    trees.top().second = false;		    
		}
		else
		    trees.top().first.append(tree::ast{t});
	    }
	}

	if (trees.size() != 1)
	    throw std::runtime_error("Failed to parse");

	return trees.top().first;
    }
}
