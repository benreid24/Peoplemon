#ifndef FUNCTIONENTRY_HPP
#define FUNCTIONENTRY_HPP

#include <vector>
#include <string>
#include "Value.hpp"

struct ArgumentEntry
{
	Value::Type type;
	std::string name;
};

struct FunctionEntry
{
	unsigned int address;
	std::vector<ArgumentEntry> arguments;
};

#endif // FUNCTIONENTRY_HPP
