#pragma once

const int nbrackets = 6;
class BracketsWalker {
public:
	const char* open_brackets[nbrackets] = { "\"","(","[","{","/*","'" };
	const char* close_brackets[nbrackets] = { "\"",")","]","}","*/","'" };
	BracketsWalker();
	/**
	 * \brief depth of each bracket type occurence
	 */
	int depth[nbrackets] = { 0 };
	/**
	 * \brief detects brackets occurence, modifies depth array
	 * \param str input string
	 * \param pos position
	 * \return new position in the text is bracket found, othervice returns initial position
	 */
	int detect_bracket(const char* str, int pos);
	/**
	 * \brief check if any bracket depth is negative
	 * \return true if negative
	 */
	bool is_zero();
	/**
	 * \brief detects if all brackets closed
	 * \return true if all brackets closed
	 */
	bool is_negative();
};