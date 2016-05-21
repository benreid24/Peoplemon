#ifndef FRAME_HPP
#define FRAME_HPP

#include <map>
#include <string>
#include "Value.hpp"

/**
 * Structure to define a virtual "stack frame" for the script interpreter
 *
 * \ingroup Scripting
 */
struct Frame
{
    std::map<std::string, Value> locals; //function arguments are passed through this
};

#endif // FRAME_HPP
