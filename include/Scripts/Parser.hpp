#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include "Token.hpp"
#include "Traverser.hpp"

/**
 * This class is responsible for resolving file references and tokenizing scripts so that they can be executed
 *
 * \ingroup Scripting
 */
class Parser
{
	Traverser data;
	std::string file, script;

public:
	/**
	 * Initializes the Parser from the given string
	 *
	 * \param The script to parse, or the file containing the script
	 */
	Parser(std::string source);

	/**
	 * Parses the script and returns the token string
	 */
	std::vector<Token> getTokens();

	/**
	 * Returns the string of the script. Used for debug stuff
	 */
	std::string getScript();

	/**
	 * Tells whether or not the given character is a letter
	 *
	 * \param c The character to test
	 * \return True if the character is a letter, false otherwise
	 */
	static bool isCharacter(char c);

	/**
	 * Tells whether or not the given character is an operator
	 *
	 * \param c The character to test
	 * \return True if the character is an operator, false otherwise
	 */
	static bool isOperator(char c);

	/**
	 * Tells whether or not the given character is a number
	 *
	 * \param c The character to test
	 * \return True if the character is a number, false otherwise
	 */
	static bool isNumber(char c);
};

#endif // PARSER_HPP
