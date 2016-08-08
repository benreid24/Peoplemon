#ifndef TRAVERSER_HPP
#define TRAVERSER_HPP

#include <string>

/**
 * Helper class for Parser. Responsible for reading strings and tracking the current line number
 *
 * \ingroup Scripting
 */
class Traverser
{
	std::string data;
	int line;
	unsigned int cChar;

	void ignoreWhitespace();

public:
	/**
	 * Creates an empty Traverser
	 */
	Traverser();
	/**
	 * Initialize the Traverser from the string to traverse
	 *
	 * \param str The string to traverse
	 */
	Traverser(std::string str);

	/**
	 * Sets the Traverser to traverse the given string
	 *
	 * \param str The string to traverse
	 */
	void setString(std::string str);

	/**
	 * Tells whether or not the end of the string has been reached
	 */
	bool done();

	/**
	 * Peek the next character without moving past it
	 *
	 * \return The next non-whitespace character in the string. nullptr if the end has been reached
	 */
	char peek();

	/**
	 * Read the next character and move past it
	 *
	 * \return The next non-whitespace character in the string. nullptr if the end has been reached
	 */
	char read();

	/**
	 * Reads all of the characters up to a deliminator. The deliminator is not returned and is moved past
	 *
	 * \param delim The character to stop reading at
	 * \return The string of characters preceding the deliminator, including whitespace
	 */
	std::string readTo(char delim);

	/**
	 * Reads a string of connected letters and numbers and stops when the first non-alphanumeric character is reached
	 */
	std::string readIdentifier();

	/**
	 * Returns the current line being read
	 */
	int getLine();
};

#endif // TRAVERSER_HPP
