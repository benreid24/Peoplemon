#include "Scripts/Script Interpreter.hpp"
#include "Scripts/Script Environment.hpp"
#include "Scripts/Parser.hpp"
#include "SFML.hpp"
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <cmath>
using namespace std;
using namespace sf;

string intToString(double i)
{
	stringstream ss;
	ss << i;
	return ss.str();
}

Script::Script()
{
	reset();
}

Script::Script(string scr) : Script()
{
    load(scr);
}

Script::~Script()
{
	//dtor
}

void Script::reset()
{
	stopping = false;
	stopped = true;
	functions.clear();
	globalFrame.locals.clear();
	while (!stackFrames.empty())
		stackFrames.pop();
}

void Script::locateFunctions()
{
	enum
	{
		Reading,
		CheckingParen,
		ScanningArgType,
		ScanningArgName,
		NextArgTransition
	}state = Reading;
	string name;
	FunctionEntry func;
	ArgumentEntry arg;

	for (unsigned int i = 0; i<tokens.size(); ++i)
	{
		switch (state)
		{
		case Reading:
			if (tokens.at(i).type==Token::Identifier)
			{
				if (i>0)
				{
					if (tokens.at(i-1).type==Token::DataType)
					{
						state = CheckingParen;
						name = tokens.at(i).data;
					}
				}
			}
			break;

		case CheckingParen:
			state = Reading;
			if (tokens.at(i).type==Token::Operator && tokens.at(i).data=="(")
			{
				if (tokens.at(i+1).type==Token::DataType)
					state = ScanningArgType;
				else if (tokens.at(i+1).type==Token::Operator && tokens.at(i+1).data==")")
					goto addFunc;
			}
			break;

		case ScanningArgType:
			cout << "Checking for paren, found: " << tokens[i].data << endl;
            if (tokens.at(i).type==Token::DataType)
			{
				state = ScanningArgName;
				arg.type = Value::String;
				if (tokens.at(i).data=="int")
					arg.type = Value::Integer;
			}
			else
				throw runtime_error("Was expecting a data type in file "+tokens.at(i).file+" line "+intToString(tokens.at(i).line));
			break;

		case ScanningArgName:
			if (tokens.at(i).type==Token::Identifier)
			{
				state = NextArgTransition;
				arg.name = tokens.at(i).data;
				func.arguments.push_back(arg);
			}
			else
				throw runtime_error("Was expecting an identifier next on line "+intToString(tokens.at(i).line)+" in file "+tokens.at(i).file);
			break;

		case NextArgTransition:
			if (tokens.at(i).type==Token::ArgDelim)
                state = ScanningArgType;
			else if (tokens.at(i).type==Token::Operator && tokens.at(i).data==")")
			{
				addFunc:
				state = Reading;
				func.address = i+2; //i+1 is the {, so i+2 is the first token in the function
				functions[name] = func;
				func.arguments.clear();
				cout << "Located function " << name << " at index " << func.address << endl;
			}
			else
				throw runtime_error("Was expecting ',' or ')' on line "+intToString(tokens.at(i).line)+" in file "+tokens.at(i).file);
		}
	}
}

void Script::load(string str)
{
	Parser parser(str);
	tokens = parser.getTokens();
	reset();
	locateFunctions();
	for (unsigned int i = 0; i<tokens.size(); ++i)
	{
		if (tokens.at(i).type==Token::Label)
		{
			if (branchTable.find(tokens.at(i).data)!=branchTable.end())
				throw runtime_error("Branch label '"+tokens.at(i).data+"' duplicate encountered on line "+intToString(tokens.at(i).line)+" in file "+tokens.at(i).file);
			branchTable[tokens.at(i).data] = i;
		}
	}
}

Value Script::combine(Token left, Token op, Token right)
{
	Value ret;
	ret.type = Value::String;
	if (left.type==Token::IntVal)
		ret.type = Value::Integer;

	if (op.data=="+")
	{
		ret.iValue = left.value+right.value;
        ret.sValue = left.data+right.data;
        if (left.type==Token::IntVal && right.type==Token::StrVal)
		{
			ret.sValue = intToString(left.value)+right.data.c_str();
			ret.type = Value::String;
		}
		if (left.type==Token::StrVal && right.type==Token::IntVal)
		{
			ret.sValue = left.data+intToString(right.value);
			ret.type = Value::String;
		}
		return ret;
	}
	else if (op.data=="!=")
	{
		ret.type = Value::Integer;
		ret.iValue = int(left.value!=right.value);
		if (right.type==Token::StrVal && left.type==Token::StrVal)
        ret.iValue = int(left.data!=right.data);
        else if (left.type==Token::IntVal && right.type==Token::StrVal)
            ret.iValue = int(left.value!=atoi(right.data.c_str()));
		else if (left.type==Token::StrVal && right.type==Token::IntVal)
			ret.iValue = int(left.data!=intToString(right.value));
		return ret;
	}
	else if (op.data=="==")
	{
		ret.type = Value::Integer;
		ret.iValue = int(left.data==right.data);
		if (right.type==Token::IntVal && left.type==Token::IntVal)
			ret.iValue = int(abs(left.value-right.value)<0.0001);
        else if (left.type==Token::IntVal && right.type==Token::StrVal)
            ret.iValue = int(left.value==atoi(right.data.c_str()) && atoi(right.data.c_str())!=0);
		else if (left.type==Token::StrVal && right.type==Token::IntVal)
			ret.iValue = int(left.data==intToString(right.value) && atoi(left.data.c_str())!=0);
		return ret;
	}

	if (right.type!=Token::IntVal)
		throw runtime_error("Was expecting integer on line "+intToString(right.line)+" in file "+right.file);
	if (left.type!=Token::IntVal)
		throw runtime_error("Was expecting integer on line "+intToString(left.line)+" in file "+left.file);

	if (op.data=="-")
		ret.iValue = left.value-right.value;
	else if (op.data=="*")
		ret.iValue = left.value*right.value;
	else if (op.data=="/")
		ret.iValue = left.value/right.value;
	else if (op.data=="%")
		ret.iValue = int(left.value)%int(right.value);
	else if (op.data=="<")
		ret.iValue = left.value<right.value;
	else if (op.data==">")
		ret.iValue = left.value>right.value;
	else if (op.data=="<=")
		ret.iValue = left.value<=right.value;
	else if (op.data==">=")
		ret.iValue = left.value>=right.value;
	else if (op.data=="&&")
		ret.iValue = int(int(left.value+0.5)!=0 && int(right.value+0.5)!=0);
	else if (op.data=="||")
		ret.iValue = int(int(left.value+0.5)!=0 || int(right.value+0.5)!=0);
	else
		throw runtime_error("Illegal operator '"+op.data+"' encountered on line "+intToString(op.line)+" in file "+op.file);

	return ret;
}

Value Script::evalEq(vector<Token> tkns)
{
    //1. recursively find and solve parenthesis
    //2. find and apply unary operators and check to see if the first value is negated
    //3. go left to right combining values with operators following pemdas

    if (tkns.size()==0)
		return Value();

    for (unsigned int i = 0; i<tkns.size(); ++i)
	{
		if (tkns.at(i).type==Token::Operator && tkns.at(i).data=="(")
		{
			unsigned int start = i+1;
			unsigned int end = 0;
			int parenCount = 1;
			vector<Token> subEq;

			while (parenCount>0)
			{
				i++;
				if (tkns.at(i).type==Token::Operator && tkns.at(i).data=="(")
					parenCount++;
				if (tkns.at(i).type==Token::Operator && tkns.at(i).data==")")
					parenCount--;
				if (i>=tkns.size())
					throw runtime_error("Unexpected end of equation on line "+intToString(tkns[tkns.size()-1].line)+" in file "+tkns[tkns.size()-1].file);
			}
			end = i-1;
			subEq.reserve(end-start+1);
			for (unsigned int i = start; i<=end; ++i)
				subEq.push_back(tkns.at(i));

			Token t;
			t = evalEq(subEq);
			tkns.erase(tkns.begin()+start,tkns.begin()+end+2);
			t.line = tkns[start].line;
			t.file = tkns[start].file;

			tkns[start-1] = t;
			i = start-1;
		}
	}

	if (tkns[0].type==Token::Operator && tkns[0].data=="-")
	{
		tkns[1].value = -tkns[1].value;
		tkns.erase(tkns.begin());
	}
	for (unsigned int i = 0; i<tkns.size(); ++i)
	{
		if (tkns.at(i).type==Token::Operator && tkns.at(i).data=="!")
		{
            if (tkns.at(i+1).type==Token::IntVal)
				tkns.at(i).value = !bool(int(tkns.at(i).value));
			else if (tkns.at(i+1).type==Token::StrVal)
			{
				tkns.at(i+1).type = Token::IntVal;
                tkns.at(i+1).value = double(tkns.at(i+1).data.size()!=0);
			}
			else
				throw runtime_error("Illegal operand type on line "+intToString(tkns.at(i+1).line)+" in file "+tkns.at(i+1).file);
		}
	}

	string ops[] = {"*/%", //times, divide, modulo
					"+-", //plus, minus
					">=<==!=", //greater (equal), less (equal), equal, not equal
					"&&||"}; //and, or

	for (int i = 0; i<4; ++i)
	{
		for (unsigned int j = 0; j<tkns.size(); ++j)
		{
			if (ops[i].find(tkns.at(j).data)!=string::npos && tkns.at(j).type==Token::Operator)
			{
				tkns.at(j-1) = combine(tkns.at(j-1),tkns.at(j),tkns.at(j+1));
				tkns.erase(tkns.begin()+j+1);
				tkns.erase(tkns.begin()+j);
				j--;
			}
		}
	}

	if (tkns.size()==1)
	{
		Value r;
		if (tkns[0].type==Token::IntVal)
		{
			r.type = Value::Integer;
			r.iValue = tkns[0].value;
		}
		else
		{
			r.type = Value::String;
			r.sValue = tkns[0].data;
		}
		return r;
	}
	throw runtime_error("An error occurred solving the equation on line "+intToString(tkns[0].line)+" in file "+tkns[0].file);
}

Value Script::evaluate(vector<Token> tkns)
{
	//1. search and replace variables
	//2. search and run functions
	//3. Call evalEq

	for (unsigned int i = 0; i<tkns.size(); ++i)
	{
		if (tkns.at(i).type==Token::Identifier)
		{
			if (globalFrame.locals.find(tkns.at(i).data)!=globalFrame.locals.end())
				tkns.at(i) = globalFrame.locals[tkns.at(i).data];
			else if (!stackFrames.empty())
			{
				if (stackFrames.top().locals.find(tkns.at(i).data)!=stackFrames.top().locals.end())
					tkns.at(i) = stackFrames.top().locals[tkns.at(i).data];
			}
			else if (!isFunction(tkns.at(i).data))
				throw  runtime_error("Unknown identifier '"+tkns.at(i).data+"' encountered on line "+intToString(tkns.at(i).line)+" in file "+tkns.at(i).file);
		}
	}

	enum
	{
		Searching,
		ReadingArgEq
	}state = Searching;
	string name;
	vector<Token> argEq;
	vector<Value> args;
	int parenCount = 0;
	unsigned int start = 0;

	for (unsigned int i = 0; i<tkns.size(); ++i)
	{
		switch (state)
		{
		case Searching:
			if (tkns.at(i).type==Token::Identifier)
			{
				name = tkns.at(i).data;
				state = ReadingArgEq;
				parenCount = 1;
				start = i;
				i++;
				argEq.clear();
				args.clear();
			}
			break;

		case ReadingArgEq:
			if (tkns.at(i).type==Token::Operator && tkns.at(i).data=="(")
			{
				argEq.push_back(tkns.at(i));
				parenCount++;
			}
			else if (tkns.at(i).type==Token::Operator && tkns.at(i).data==")")
			{
				parenCount--;
				if (parenCount==0)
				{
					args.push_back(evaluate(argEq));
					if (isLibraryFunction(name))
						tkns[start] = executeLibraryFunction(name,args);
					else
					{
						int pc = functions[name].address;
						Frame f;
						for (unsigned int j = 0; j<args.size(); ++j)
						{
							if (j>=functions[name].arguments.size())
								throw runtime_error("Too many arguments passed to function '"+name+"' on line "+intToString(tkns[start].line)+" in file "+tkns[start].file);
							else if (args[j].type!=functions[name].arguments[j].type)
								throw runtime_error("Type mismatch in call to '"+name+"' on line "+intToString(tkns[start].line)+" in file "+tkns[start].file);
							f.locals[functions[name].arguments[j].name] = args[j];
						}
						stackFrames.push(f);
						tkns[start] = runTokens(pc);
						stackFrames.pop();
					}
					tkns.erase(tkns.begin()+start+1,tkns.begin()+i+1);
					state = Searching;
				}
				else
					argEq.push_back(tkns.at(i));
			}
			else if (tkns.at(i).type==Token::ArgDelim)
			{
				args.push_back(evaluate(argEq));
				argEq.clear();
			}
			else
				argEq.push_back(tkns.at(i));
			break;
		}
	}

	return evalEq(tkns);
}

bool Script::isFunction(string name)
{
	return (isLibraryFunction(name) || functions.find(name)!=functions.end());
}

Value Script::runTokens(int pos)
{
	vector<Token> tkns;
	Value test;
	string name;
	int parenCount = 0;
	int blockCount = 0; //terminate if blockcount gets to -1 (for loops)
	ConditionalState conditionalState = NoConditional;
	bool execCond = false, resetCondState = true;

	for (unsigned int i = pos; i<tokens.size(); ++i)
	{
		if (stopping)
			break;

		switch (tokens.at(i).type)
		{
		case Token::DataType: //variable declaration or function definition
            if (tokens.at(i+1).type!=Token::Identifier)
				throw runtime_error("Was expecting identifier after data type on line "+intToString(tokens.at(i).line)+" in file "+tokens.at(i).file);

			name = tokens.at(i+1).data;
			i = i+2;
            if (tokens.at(i).type==Token::Assignment || tokens.at(i).type==Token::LineDelim) //variable declaration
			{
				if (stackFrames.empty())
				{
					if (globalFrame.locals.find(name)!=globalFrame.locals.end())
						throw runtime_error("Identifier on line "+intToString(tokens.at(i).line)+" in file "+tokens.at(i).file+" already exists!");
					globalFrame.locals[name] = Value();
					globalFrame.locals[name].type = Value::Integer;
				}
				else
				{
					if (stackFrames.top().locals.find(name)!=stackFrames.top().locals.end())
						throw runtime_error("Identifier on line "+intToString(tokens.at(i).line)+" in file "+tokens.at(i).file+" already exists!");
					stackFrames.top().locals[name] = Value();
					stackFrames.top().locals[name].type = Value::Integer;
				}

				if (tokens.at(i).type==Token::Assignment)
				{
					tkns.clear();
					while (tokens.at(i).type!=Token::LineDelim)
					{
						i++;
						tkns.push_back(tokens.at(i));
					}
					tkns.erase(tkns.begin()+tkns.size()-1); //last one will be ';'
					if (stackFrames.empty())
						globalFrame.locals[name] = evaluate(tkns);
					else
						stackFrames.top().locals[name] = evaluate(tkns);
				}
			}
			else //function definition, skip
			{
				while (tokens.at(i).type!=Token::BlockOpen)
					i++;
				parenCount = 1;
				while (parenCount>0)
				{
					i++;
					if (tokens.at(i).type==Token::BlockOpen)
						parenCount++;
					if (tokens.at(i).type==Token::BlockClose)
                        parenCount--;
				}
			}
			conditionalState = NoConditional;
			break;

		case Token::Branch:
			if (tokens.at(i+1).type!=Token::Identifier)
				throw runtime_error("Expecting identifier after branch on line "+intToString(tokens.at(i).line)+" in file "+tokens.at(i).file);
			if (branchTable.find(tokens.at(i+1).data)==branchTable.end())
				throw runtime_error("Branch to unknown identifier encountered on line "+intToString(tokens.at(i).line)+" in file "+tokens.at(i).file);
			i = branchTable[tokens.at(i+1).data];
			conditionalState = NoConditional;
			break;

		case Token::Elif:
			if (conditionalState==NoConditional)
				throw runtime_error("'elif' encountered without preceding 'if' on line "+intToString(tokens.at(i).line)+" in file "+tokens.at(i).file);
			resetCondState = false; //to allow failure to fall through a chain of elif's
			//fall through to check condition

		case Token::Conditional:
			if (tokens.at(i).type==Token::Conditional)
				resetCondState = true; //we want to update the state

            if (tokens.at(i+1).type!=Token::Operator || tokens.at(i+1).data!="(")
				throw runtime_error("Was expecting '(' after 'if' on line "+intToString(tokens.at(i).line)+" in file "+tokens.at(i).file);
			tkns.clear();
			parenCount = 1;
			i = i+2;
			while (parenCount>0)
			{
                if (tokens.at(i).type==Token::Operator && tokens.at(i).data=="(")
					parenCount++;
				if (tokens.at(i).type==Token::Operator && tokens.at(i).data==")")
					parenCount--;
				tkns.push_back(tokens.at(i));
				i++;
			}
			i--; //so i points to the ')', that way i+1 means the same thing for both if and else
			tkns.erase(tkns.begin()+tkns.size()-1); //last one will be ')'
			test = evaluate(tkns);
			execCond = test.nonZero();
			if (!resetCondState) //elif
				execCond = execCond && conditionalState!=LastConditionalExecuted;
			//fall through to execute or skip the conditional code

		case Token::Else:
			if (tokens.at(i).type==Token::Else)
			{
				execCond = conditionalState==LastConditionalFailed; //check condition in case of else
				resetCondState = true;
			}
			if (execCond)
			{
				conditionalState = LastConditionalExecuted;
				if (tokens.at(i+1).type==Token::BlockOpen)
				{
					runTokens(i+2);
					parenCount = 1;
					i++;
					while (parenCount>0)
					{
						i++;
						if (tokens.at(i).type==Token::BlockOpen)
							parenCount++;
						if (tokens.at(i).type==Token::BlockClose)
							parenCount--;
					}
				}
			}
			else
			{
				if (tokens.at(i+1).type==Token::BlockOpen)
				{
					parenCount = 1;
					i = i+2;
					while (parenCount>0)
					{
						if (tokens.at(i).type==Token::BlockOpen)
							parenCount++;
						if (tokens.at(i).type==Token::BlockClose)
							parenCount--;
						i++;
					}
					i--;
				}
				else
				{
					while (tokens.at(i).type!=Token::LineDelim)
						i++;
				}
				if (resetCondState)
					conditionalState = LastConditionalFailed;
			}
			break;

		case Token::Loop:
            if (tokens.at(i+1).type!=Token::Operator || tokens.at(i+1).data!="(")
				throw runtime_error("Was expecting '(' after 'while' on line "+intToString(tokens.at(i+1).line)+" in file "+tokens.at(i+1).file);

			i += 2;
			parenCount = 1;
			tkns.clear();
			while (parenCount>0)
			{
				tkns.push_back(tokens.at(i));
				i++;
				if (tokens.at(i).type==Token::Operator && tokens.at(i).data=="(")
					parenCount++;
				if (tokens.at(i).type==Token::Operator && tokens.at(i).data==")")
					parenCount--;
			}

			if (tokens.at(i+1).type!=Token::BlockOpen)
				throw runtime_error("Was expecting '{' after loop on line "+intToString(tokens.at(i).line)+" in file "+tokens.at(i).file);

			i += 2;
			test = evaluate(tkns);
			while ((test.type==Value::Integer && int(test.iValue+0.01)!=0) || (test.type==Value::String && test.sValue.size()>0))
			{
				if (stopping)
					break;

				runTokens(i);
				test = evaluate(tkns);
			}

			parenCount = 1;
			while (parenCount>0)
			{
				if (tokens.at(i).type==Token::BlockClose)
					parenCount--;
				if (tokens.at(i).type==Token::BlockOpen)
					parenCount++;
				i++;
			}
			i--;
			conditionalState = NoConditional;
			break;

		case Token::Return:
			i++;
			tkns.clear();
			while (tokens.at(i).type!=Token::LineDelim)
			{
                tkns.push_back(tokens.at(i));
                i++;
			}
			return evaluate(tkns);
			break;

		case Token::BlockOpen:
            blockCount++;
            conditionalState = NoConditional;
            break;

		case Token::BlockClose:
			blockCount--;
			if (blockCount<0)
                goto voidRet;
			conditionalState = NoConditional;
			break;

		case Token::LineDelim:
			conditionalState = NoConditional;
			break;

		case Token::Label:
			conditionalState = NoConditional;
			break;

		default: //likely just a regular line, but have to check for assignment
			name.clear();
			if (tokens.at(i+1).type==Token::Assignment)
			{
				name = tokens.at(i).data;
				if (tokens.at(i).type!=Token::Identifier || (globalFrame.locals.find(name)==globalFrame.locals.end() && stackFrames.top().locals.find(name)==stackFrames.top().locals.end()))
					throw runtime_error("Assignment to non-variable on line "+intToString(tokens.at(i).line)+" in file "+tokens.at(i).file);
				i = i+2;
			}
			tkns.clear();
			while (tokens.at(i).type!=Token::LineDelim)
			{
				tkns.push_back(tokens.at(i));
				i++;
			}
			test = evaluate(tkns);
			if (name.size()>0)
			{
				if (globalFrame.locals.find(name)!=globalFrame.locals.end())
					globalFrame.locals[name] = test;
				else
					stackFrames.top().locals[name] = test;
			}
			conditionalState = NoConditional;
			break;
		}
	}

	voidRet:
	Value v;
	v.type = Value::Void;
	v.iValue = 0;
	return v;
}

void Script::run(ScriptEnvironment* env)
{
	if (tokens.size()==0)
		return;

	stopped = stopping = false;
	environment = env;
	try
	{
		runTokens(0);
	}
	catch (const out_of_range& e)
	{
		cout << "Parenthesis or bracket mismatch somewhere in file "+tokens.at(0).file << endl;
	}
	catch (const runtime_error& e)
	{
		cout << e.what() << endl;
	}
	stopped = true;
}

void Script::stop()
{
	stopping = true;
	while (!stopped)
		sleep(milliseconds(30));
}

bool Script::isRunning()
{
	return !stopped;
}
