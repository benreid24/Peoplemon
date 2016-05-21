#include "Scripts/Traverser.hpp"
#include "Scripts/Parser.hpp"
using namespace std;

Traverser::Traverser()
{
	data = "";
	line = 1;
	cChar = 0;
}

Traverser::Traverser(string str)
{
    setString(str);
}

void Traverser::setString(string str)
{
	data = str;
    line = 1;
    cChar = 0;
    ignoreWhitespace();
    while (data.find("\\n")!=string::npos)
		data.replace(data.find("\\n"),2,"\n");
}

void Traverser::ignoreWhitespace()
{
    while ((data[cChar]==' ' || data[cChar]=='\n' || data[cChar]=='\t') && cChar<data.size())
	{
		if (data[cChar]=='\n')
			line++;
		cChar++;
	}
}

char Traverser::peek()
{
	if (cChar>=data.size())
		return 0;
	return data[cChar];
}

char Traverser::read()
{
	if (cChar>=data.size())
		return 0;
	char r = data[cChar];
	cChar++;
	if (r!='"')
	ignoreWhitespace();
	return r;
}

string Traverser::readTo(char delim)
{
	string r;
    r.reserve(64);
	while (data[cChar]!=delim && cChar<data.size())
	{
		r.push_back(data[cChar]);
		cChar++;
	}
	read(); //to ignore the deliminator and move on to the next non-whitespace
	return r;
}

string Traverser::readIdentifier()
{
	string r;
	r.reserve(32);
	while ((Parser::isCharacter(data[cChar]) || Parser::isNumber(data[cChar])) && cChar<data.size())
	{
		r.push_back(data[cChar]);
		cChar++;
	}
	ignoreWhitespace();
	return r;
}

int Traverser::getLine()
{
	return line;
}

bool Traverser::done()
{
	return cChar>=data.size();
}
