#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include "Value.hpp"

/**
 * Structure to store tokens to be executed by the script interpreter
 *
 * \ingroup Scripting
 */
struct Token
{
	enum Type
	{
        Identifier=0,
        Assignment,
        DataType,
        IntVal,
        StrVal,
        Operator,
        ArgDelim,
        LineDelim,
        BlockOpen,
        BlockClose,
        Conditional,
        Elif,
        Else,
        Loop,
        Branch,
        Label,
        Return
	}type;
	std::string data; //stores the name, value, etc
	double value; //used in the case of an IntVal

	std::string file; //the file it appeared in
	int line; //the line it appeared on

	Token& operator= (Value v)
	{
		if (v.type==Value::Integer)
		{
			type = IntVal;
			value = v.iValue;
		}
		else
		{
			type = StrVal;
            data = v.sValue;
		}

		return *this;
	}
};

#endif // TOKEN_HPP
