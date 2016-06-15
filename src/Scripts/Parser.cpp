#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include "Scripts/Parser.hpp"
#include "Util/File.hpp"
#include "Properties.hpp"
using namespace std;

Parser::Parser(string source)
{
	file = source;

	if (File::getExtension(source)=="psc")
	{
		if (!FileExists(source))
			source = Properties::ScriptPath+source;
		if (!FileExists(source))
			throw runtime_error("Failed to load script: "+source);
		File input(source);
		string temp;
		while (input.good())
		{
			temp.push_back(input.get<char>());
		}
		data.setString(temp);
	}
	else
		data.setString(source);
}

bool Parser::isCharacter(char c)
{
	return ((c>='a' && c<='z') || (c>='A' && c<='Z'));
}

bool Parser::isOperator(char c)
{
	return (c=='%' || c=='&' || c=='(' || c==')' || c=='*' || c=='+' || c=='-' || c=='/' || c=='>' || c=='<' || c=='!' || c=='|'); //note: '=' (61) is only an operator when it is preceded by an operator, otherwise it is assignment
}

bool Parser::isNumber(char c)
{
	return (c>='0' && c<='9');
}

vector<Token> Parser::getTokens()
{
	vector<Token> ret;

    while (!data.done())
	{
		Token t;
		t.file = file;
		t.line = data.getLine();
		char c = data.read();

		if (c=='/')
		{
			if (data.peek()=='/')
			{
				data.readTo('\n');
				continue;
			}
			else if (data.peek()=='*')
			{
				tryAgain:
					data.readTo('*');
					if (data.peek()=='/')
					{
						data.read();
						continue;
					}
					goto tryAgain;
			}
		}

		if (isNumber(c))
		{
			string num(1,c);
			while (isNumber(data.peek()) || data.peek()=='.')
			{
                num.push_back(data.read());
			}
			stringstream ss;
			ss << num;
			ss >> t.value;
			t.type = Token::IntVal;
		}
		else if (c=='"')
		{
			t.data = data.readTo('"');
			t.type = Token::StrVal;
		}
		else if (isOperator(c))
		{
			t.data.push_back(c);
			if ((data.peek()=='|' || data.peek()=='=' || data.peek()=='&') && c!=')' && c!='(')
				t.data.push_back(data.read());
			t.type = Token::Operator;
		}
		else if (c==';')
			t.type = Token::LineDelim;
		else if (c=='{')
			t.type = Token::BlockOpen;
		else if (c=='}')
			t.type = Token::BlockClose;
		else if (c==',')
			t.type = Token::ArgDelim;
		else if (c=='=')
		{
			if (data.peek()=='=')
			{
				data.read(); //move past the '='
				t.type = Token::Operator;
				t.data = "==";
			}
			else
				t.type = Token::Assignment;
		}
		else if (isCharacter(c))
		{
			//read whole word, check for keywords. Is likely identifier if none, peek next character to determine if label
			string word = data.readIdentifier();
			word.insert(0,1,c); //prepend the first character
			t.data = word;
			if (word=="goto")
				t.type = Token::Branch;
			else if (word=="if")
				t.type = Token::Conditional;
			else if (word=="elif")
				t.type = Token::Elif;
            else if (word=="else")
				t.type = Token::Else;
			else if (word=="return")
				t.type = Token::Return;
			else if (word=="int")
				t.type = Token::DataType;
			else if (word=="string")
				t.type = Token::DataType;
			else if (word=="while")
				t.type = Token::Loop;
			else
			{
				t.type = Token::Identifier;
				if (data.peek()==':')
				{
					data.read(); //move past the ':'
					t.type = Token::Label;
				}
			}
		}
		else if (c=='#')
		{
			string word = data.readIdentifier();

			if (word=="include")
			{
				char c = data.read();
				if (c=='"')
				{
					string f = data.readTo('"');
					Parser parser(f);
					vector<Token> tkns = parser.getTokens();
					for (unsigned int i = 0; i<tkns.size(); ++i)
					{
						ret.push_back(tkns[i]);
					}
				}
				else
					throw runtime_error("Expected '\"' after #include but found none!");
			}
		}
		else
			goto noAdd;
		ret.push_back(t);
		noAdd:;
	}

	return ret;
}
