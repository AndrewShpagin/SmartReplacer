#pragma once

class BaseToken;

#include <iostream>
#include <tuple>
#include <filesystem>
#include <type_traits>
#include <chrono>
#include <time.h>
#include "comms-Math/comms-Math.h"
#include "BracketsWalker.h"
#include "CharactersBitset.h"

using namespace comms;

/**
 * \brief The class helps to replace one pattern by another one. Pattern consists of charaters and tukens.\n
 * List of possible tokens\n
 * \b $expression - expression with same amount of vice-versa brackets(){}[]"". May contain any charactes in brackets.\n
 * Outside the brackets scope may contain a..z A..Z 0..9 () [] {} +- *&~\\!:_"'/ and spaces. Comma ',' may not be outside the brackets scope.\n
 * \b $numbers... - 0..9\n
 * \b $float... - floating point values\n
 * \b $string... - quoted string "..."\n
 * \b $characters... - regular Eng characters\n
 * \b $variable... - c++ valid variable name\n
 * \b ... look the complete list in commandline
 *
 * If you will add \b $zero before the token (like \b $zero$numbers1 ), it may be ignored if not present.\n
 *  
 */
class SmartReplacer {
public:
	/**
	 * \brief Verbose = 0 : show only processed file names, 1 : show found text, 2 : show found text and replaced text
	 */
	static int Verbose;
	static int ChangedFiles;
	static int SummaryReplacements;
	static cStr LogFileName;
	cStr curfile;
	cList<BaseToken*> palette;
	
	SmartReplacer();
	~SmartReplacer();
	
	/**
	 * \brief reset to initial state
	 */
	void clear();
	/**
	 * \brief parsed pattern to search
	 */
	cList<BaseToken*> parsed_in;
	/**
	 * \brief parsed pattern to replace
	 */
	cList<BaseToken*> parsed_out;
	/**
	 * \brief Performs find and replace in the file \b in_file_name and writes to \b out_file_name using patterns file \b patterns_file
	 * \param in_file_name input file name
	 * \param out_file_name output file name
	 * \param patterns_file file that contains patterns, see \b process_file example.
	 * \param src_folder the source folder
	 * \param dst_folder the destination folder to write file if changed
	 * \return
	 */
	bool process_file(const char* in_file_name, const char* out_file_name, const char* patterns_file, const char* src_folder = nullptr, const char*
	                  dst_folder = nullptr);
	/**
	 * \brief Creates the replacer structures from the patterns list in text file
	 * \param text text should be in format as in example below.\n
	 * There should be even count of patterns, separators should contain at leas 5 = signs.\n
	 * \code
	 * pattern_to_search_1
	 * ========================
	 * pattern_to_replace_1
	 * ========================
	 * pattern_to_search_2
	 * ========================
	 * pattern_to_replace_2
	 * ......
	 * \endcode
	 * \param start position in the text
	 * \return end position in the text, returns -1 if the end reached and there is nothing to do.
	 */
	int create_from_textfile(const char* text, int start);
	/**
	 * \brief parse input or output pattern
	 * \param pattern string
	 * \param list returns list of tokens
	 */
	void parse_pattern(const char* string, cList<BaseToken*>& list);
	/**
	 * \brief parses string to find and replace
	 * \param to_find string to find
	 * \param replace string to replace
	 */
	void set_patterns(const char* to_find, const char* replace);
	/**
	 * \brief Finds first occurence of the pattern in the string
	 * \param text string
	 * \param start start position
	 * \return
	 */
	std::tuple<int, int> find_occurence(const char* text, int start);
	/**
	 * \brief replaces previously found pattern with replacement pattern
	 * \param text the text to modify
	 * \param start start position
	 * \param end end of chunk position
	 * \return new end position
	 */
	int replace_with_current_context(cStr& text, int start, int end);
	int process(cStr& text);
	template <class y>
	void doooo(y x, int) const;
};

#include "Tokens.h"

///returns class name as const char*
#define classname() "$" + cStr(typeid(decltype(*this)).name()+6)
#define _autonew() (new std::remove_pointer<decltype(this)>::type)
cStr read_text(const char* path);
/**
 * \brief Save text to the file 
 * \param file filename
 * \param text text to write
 * \param initialfolder the initial folder of the file
 * \param changedfolder write to other folder if the parameter is not nullptr
 */
void save_text(const char* file, const char* text, const char* initialfolder = nullptr, const char* changedfolder = nullptr);
/**
 * \brief Create list of files in the folder.
 * \param folder The folder to search files in.
 * \param ext List of acceptable extensions, no wildcards
 * \param result Resulting files list
 * \param recursive Recursuve search if true
 */
void get_files(const char* folder, const cList<cStr>& ext, cList<cStr>& result, bool recursive);
/**
 * \brief Remove "", add / if need to make correct folder path
 * \param path raw string
 * \return correct folder path
 */
cStr _todir(const char* path);
