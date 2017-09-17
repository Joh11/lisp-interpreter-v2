#ifndef TREE_HPP
#define TREE_HPP

#include <string>
#include <iostream>
#include <utility>

namespace tree
{
    template <typename T>
    class binary_tree
    {
    public:
	// The enum to determine the type of the node
	enum class node_type {LEAF, BRANCH};

	// The two constructors to build a leaf and a branch
	binary_tree(T const& value = T()) : _value{value}, _nodeType{node_type::LEAF} {}
	
	binary_tree(binary_tree const& left, binary_tree const& right) : _children{new binary_tree{left}, new binary_tree{right}}, _nodeType{node_type::BRANCH} {}

	// Copy constructor (1/5)
	binary_tree(binary_tree const& other) : _nodeType{other._nodeType}
	{
	    // We have to deep copy (recursively)
	    if(_nodeType == node_type::LEAF)
		_value = other._value;
	    else
	    {
		_children.left = new binary_tree{*other._children.left};
		_children.right = new binary_tree{*other._children.right};		
	    }
	}

	// Copy assignment (2/5)
	// We cannot use copy and swap for this one because of ambiguity
	binary_tree & operator=(binary_tree const& other)
	{
	    if(&other == this)
		return *this;
	    
	    auto copy{other};

	    _nodeType = copy._nodeType;
	    
	    if(copy._nodeType == node_type::LEAF)
		std::swap(_value, copy._value);
	    else
	    {
		std::swap(_children.left, copy._children.left);
		std::swap(_children.right, copy._children.right);
	    }

	    return *this;
	}

	// Destrutor (3/5)
	// We have to free the subtrees if it's a branch
	~binary_tree()
	{
	    if(_nodeType == node_type::BRANCH)
	    {
		delete _children.left;
		delete _children.right;
	    }
	}

	// Move constructor (4/5)
	binary_tree(binary_tree && other) : _nodeType{std::move(other._nodeType)}
	{
	    if(_nodeType == node_type::LEAF)
		_value = std::move(other._value);
	    else
	    {
		_children.left = other._children.left;
		_children.right = other._children.right;


		// Set the other's children to nullptr to not delete our new children
		// We can do this because "delete nullptr;" is legit it seems
		other._children.left = nullptr;
		other._children.right = nullptr;
	    }
	}

	// Move assignment operator (5/5)
	binary_tree & operator=(binary_tree && other)
	{
	    _nodeType = std::move(other._nodeType);

	    if(_nodeType == node_type::LEAF)
		_value = std::move(other._value);
	    else
	    {
		_children.left = other._children.left;
		_children.right = other._children.right;


		// Set the other's children to nullptr to not delete our new children
		// We can do this because "delete nullptr;" is legit it seems
		other._children.left = nullptr;
		other._children.right = nullptr;
	    }

	    return *this;
	}

	// Other methods

	bool is_leaf() const {return _nodeType == node_type::LEAF;}

	T & value() { return _value;}
	T const& value() const { return _value;}

	binary_tree & left() {return *_children.left;}
	binary_tree const& left() const {return *_children.left;}

	binary_tree & right() {return *_children.right;}
	binary_tree const& right() const {return *_children.right;}

	// Replace the rightest leaf with (value . nil)
	void append(binary_tree const& t)
	{
	    if(_nodeType == node_type::LEAF)
	    {
		*this = {t, {"nil"}};
	    }
	    else
	    {
		_children.right->append(t);
	    }
	}

	std::ostream & display(std::ostream & o, bool first = true) const
	{
	    if(_nodeType == node_type::LEAF)
		return o << _value;

	    if(_children.right->is_leaf() && _children.right->value() != "nil")
	    {
		o << "(";
		_children.left->display(o);
		o << " . ";
		_children.right->display(o);
		return o << ")";
	    }

	    if(first)
	    {
		o << "(";
		_children.left->display(o);

		if(_children.right->is_leaf() && _children.right->value() == "nil")
		{
		    return o << ")";
		}
		
		o << " ";
		return _children.right->display(o, false);
	    }
	    else
	    {
		if(_children.right->is_leaf() && _children.right->value() == "nil")
		{
		    _children.left->display(o);
		    return o << ")";
		}
		else
		{
		    _children.left->display(o);
		    o << " ";
		    return _children.right->display(o, false);
		}
	    }
	}

    private:
	union
	{
	    // The leaf part
	    T  _value;

	    // The branch part
	    struct
	    {
		binary_tree *left;
		binary_tree *right;
	    } _children;
	};

	node_type _nodeType;
    };

    // A function to display a binary tree
    // The template type has to be <<able
    template <typename T>
    std::ostream & operator<<(std::ostream & o, binary_tree<T> const& t)
    {
	return t.display(o);
    }

    // Aliases and type stuff
    using ast = binary_tree<std::string>;

    // Return (nil . nil)
    inline ast empty_branch()
    {
	return {{"nil"}, {"nil"}};
    }
}

#endif
