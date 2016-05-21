#ifndef VALUE_HPP
#define VALUE_HPP

#include <string>

/**
 * Structure for storing values. Used by the script interpreter
 *
 * \ingroup Scripting
 */
struct Value
{
    enum Type
    {
    	Void,
    	Integer,
    	String
    }type;
    double iValue;
    std::string sValue;

    bool nonZero()
    {
    	if (type==Integer)
			return int(iValue+0.01)!=0;
		return sValue.size()!=0;
    }
};

#endif // VALUE_HPP
