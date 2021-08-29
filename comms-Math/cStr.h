#pragma once

//*****************************************************************************
// cStr
//*****************************************************************************
class cStr {
public:
	//-------------------------------------------------------------------------
	// .ctor
	//-------------------------------------------------------------------------
	cStr();
	cStr(const cStr &Src);
	cStr(const cStr &Src, const int StartIndex, const int Count);
	cStr(const char *Src);
	cStr(const char *Src, const int StartIndex, const int Count);
	cStr(const int Length, const char Fill = ' ');	// ... + cStr(20, '-') + ...
	
	// .dtor
	~cStr();

	//-------------------------------------------------------------------------
	// Copy
	//-------------------------------------------------------------------------
	void Copy(const cStr &);
	void Copy(const cStr &, const int StartIndex, const int Count);
	void Copy(const char *);
	void Copy(const char *, const int StartIndex, const int Count);
	void operator = (const cStr &);
	void operator = (const char *);

	//-------------------------------------------------------------------------
	// ToCharPtr
	//-------------------------------------------------------------------------
	const char * ToCharPtr() const;
	char * ToCharPtr();
	operator const char * () const;
	operator char * ();

	//---------------------------------------------------------------
	// operator []
	//---------------------------------------------------------------
	char operator [] (const int CharIndex) const;
	char & operator [] (const int CharIndex);

	//-------------------------------------------------------------------------
	// Length
	//-------------------------------------------------------------------------
	int Length() const;
	static int Length(const char *Str);
	
	//-------------------------------------------------------------------------
	// SetLength / CalcLength, Fill
	//-------------------------------------------------------------------------
	// Ensures needed capacity and sets a specified length. Fill char can be specified.
	void SetLength(const int Length, const char Fill = ' ');
	void CalcLength(); // Updates length based on actual '\0' character position.
	
	// cStr Path;
	// ...
	// Path.SetLength(MAX_PATH);
	// GetModuleFileName(NULL, Path.ToCharPtr(), Path.Length());
	// Path.CalcLength();

	void Fill(const char c); // Fills the whole string with the specified character.

	//-------------------------------------------------------------------------
	// IsEmpty, Clear
	//-------------------------------------------------------------------------
	bool IsEmpty() const;
	void Clear();
	static const cStr Empty;

	//-------------------------------------------------------------------------
	// Equals(NoCase) (==, !=)
	//-------------------------------------------------------------------------
	static bool Equals(const char *, const char *);
	static bool Equals(const char *, const cStr &);
	static bool Equals(const cStr &, const char *);
	static bool Equals(const cStr &, const cStr &);
	static bool Equals(const cStr *, const cStr *);

	friend bool operator == (const char *, const cStr &);
	friend bool operator == (const cStr &, const char *);
	friend bool operator == (const cStr &, const cStr &);

	friend bool operator != (const char *, const cStr &);
	friend bool operator != (const cStr &, const char *);
	friend bool operator != (const cStr &, const cStr &);
	
	static bool Equals(const char *, const char *, const int MaxLength);
	static bool Equals(const char *, const cStr &, const int MaxLength);
	static bool Equals(const cStr &, const char *, const int MaxLength);
	static bool Equals(const cStr &, const cStr &, const int MaxLength);
	
	static bool EqualsNoCase(const char *, const char *);
	static bool EqualsNoCase(const char *, const cStr &);
	static bool EqualsNoCase(const cStr &, const char *);
	static bool EqualsNoCase(const cStr &, const cStr &);
	static bool EqualsNoCase(const cStr *, const cStr *);
	
	static bool EqualsNoCase(const char *, const char *, const int MaxLength);
	static bool EqualsNoCase(const char *, const cStr &, const int MaxLength);
	static bool EqualsNoCase(const cStr &, const char *, const int MaxLength);
	static bool EqualsNoCase(const cStr &, const cStr &, const int MaxLength);
	
	//-------------------------------------------------------------------------
	// Compare(NoCase) (>, >=, <, <=)
	//-------------------------------------------------------------------------
	static int Compare(const char *, const char *);
	static int Compare(const char *, const cStr &);
	static int Compare(const cStr &, const char *);
	static int Compare(const cStr &, const cStr &);
	static int Compare(const cStr *, const cStr *);

	friend bool operator > (const char *, const cStr &);
	friend bool operator > (const cStr &, const char *);
	friend bool operator > (const cStr &, const cStr &);

	friend bool operator >= (const char *, const cStr &);
	friend bool operator >= (const cStr &, const char *);
	friend bool operator >= (const cStr &, const cStr &);

	friend bool operator < (const char *, const cStr &);
	friend bool operator < (const cStr &, const char *);
	friend bool operator < (const cStr &, const cStr &);

	friend bool operator <= (const char *, const cStr &);
	friend bool operator <= (const cStr &, const char *);
	friend bool operator <= (const cStr &, const cStr &);
	
	static int Compare(const char *, const char *, const int MaxLength);
	static int Compare(const char *, const cStr &, const int MaxLength);
	static int Compare(const cStr &, const char *, const int MaxLength);
	static int Compare(const cStr &, const cStr &, const int MaxLength);
	
	static int CompareNoCase(const char *, const char *);
	static int CompareNoCase(const char *, const cStr &);
	static int CompareNoCase(const cStr &, const char *);
	static int CompareNoCase(const cStr &, const cStr &);
	static int CompareNoCase(const cStr *, const cStr *);

	static int CompareNoCase(const char *, const char *, const int MaxLength);
	static int CompareNoCase(const char *, const cStr &, const int MaxLength);
	static int CompareNoCase(const cStr &, const char *, const int MaxLength);
	static int CompareNoCase(const cStr &, const cStr &, const int MaxLength);

	//-------------------------------------------------------------------------
	// EqualsPath
	//-------------------------------------------------------------------------
	static bool EqualsPath(const char *, const char *);
	static bool EqualsPath(const char *, const cStr &);
	static bool EqualsPath(const cStr &, const char *);
	static bool EqualsPath(const cStr &, const cStr &);
	static bool EqualsPath(const cStr *, const cStr *);

	static bool EqualsPath(const char *, const char *, const int MaxLength);
	static bool EqualsPath(const char *, const cStr &, const int MaxLength);
	static bool EqualsPath(const cStr &, const char *, const int MaxLength);
	static bool EqualsPath(const cStr &, const cStr &, const int MaxLength);

	//-------------------------------------------------------------------------
	// ComparePath
	//-------------------------------------------------------------------------
	static int ComparePath(const char *, const char *);
	static int ComparePath(const char *, const cStr &);
	static int ComparePath(const cStr &, const char *);
	static int ComparePath(const cStr &, const cStr &);
	static int ComparePath(const cStr *, const cStr *);

	static int ComparePath(const char *, const char *, const int MaxLength);
	static int ComparePath(const char *, const cStr &, const int MaxLength);
	static int ComparePath(const cStr &, const char *, const int MaxLength);
	static int ComparePath(const cStr &, const cStr &, const int MaxLength);

	// Path comparers consider '\\' and '/' as equivalents. They use no case compare.
	// Also they compare according to folder depth.
	
	//-------------------------------------------------------------------------
	// StartsWith / EndsWith
	//-------------------------------------------------------------------------
	// Determines whether the beginning of this instance matches the specified string.
	// If the specified string is empty (not NULL is asserted), the return value is "true".
	// Case insesitive compare can be specified.
	bool StartsWith(const char *Str, const bool NoCase = false) const;
	// Determines whether the end of this instance matches the specified string.
	// If the specified string is empty (not NULL is asserted), the return value is "true";
	// Case insesitive compare can be specified.
	bool EndsWith(const char *Str, const bool NoCase = false) const;

	//-------------------------------------------------------------------------
	// ToString
	//-------------------------------------------------------------------------
	// Returns a string that represents a specified value.
	// These functions are heavily used in "Append" and "Insert" functions,
	// as well as in "+=", "+" and "<<" operators.
	static const cStr ToString(const bool);
	static const cStr ToString(const int);
	static const cStr ToString(const int, const int Prec);
	static const cStr ToString(const int *IntArray, const int Count, const char *Separator = " ");
	static const cStr ToString(const float, const int Prec = 2);
	static const cStr ToString(const float *FloatArray, const int Count, const int Prec = 2, const char *Separator = " ");
	static const cStr ToString(const double, const int Prec = 6);
	static const cStr ToString(const double *DoubleArray, const int Count, const int Prec = 6, const char *Separator = " ");

	//-------------------------------------------------------------------------
	// Append (+=, <<, +)
	//-------------------------------------------------------------------------
	void Append(const cStr &);
	void Append(const cStr &, const int StartIndex, const int Count);
	void Append(const char *);
	void Append(const char *, const int StartIndex, const int Count);
	void Append(const char);
	void Append(const char, const int Count);
	void Append(const bool);
	void Append(const int);
	void Append(const float, const int Prec = 2);
	void Append(const double, const int Prec = 6);

	void operator += (const cStr &);
	void operator += (const char *);
	void operator += (const char);
	void operator += (const bool);
	void operator += (const int);
	void operator += (const float);
	void operator += (const double);

	cStr & operator << (const cStr &);
	cStr & operator << (const char *);
	cStr & operator << (const char);
	cStr & operator << (const bool);
	cStr & operator << (const int);
	cStr & operator << (const float);
	cStr & operator << (const double);

	friend const cStr operator + (const cStr &, const cStr &);
	friend const cStr operator + (const cStr &, const char *);
	friend const cStr operator + (const char *, const cStr &);
	friend const cStr operator + (const cStr &, const char);
	friend const cStr operator + (const char, const cStr &);
	friend const cStr operator + (const cStr &, const bool);
	friend const cStr operator + (const bool, const cStr &);
	friend const cStr operator + (const cStr &, const int);
	friend const cStr operator + (const int, const cStr &);
	friend const cStr operator + (const cStr &, const float);
	friend const cStr operator + (const float, const cStr &);
	friend const cStr operator + (const cStr &, const double);
	friend const cStr operator + (const double, const cStr &);

	//-------------------------------------------------------------------------
	// Append... (Path, Format)
	//-------------------------------------------------------------------------
	void AppendPath(const char *Path);
	void AppendFormat(const char *Format, ...);

	//-------------------------------------------------------------------------
	// Insert
	//-------------------------------------------------------------------------
	void Insert(const int Index, const cStr &);
	void Insert(const int Index, const cStr &, const int StartIndex, const int Count);
	void Insert(const int Index, const char *);
	void Insert(const int Index, const char *, const int StartIndex, const int Count);
	void Insert(const int Index, const char);
	void Insert(const int Index, const char, const int Count);
	void Insert(const int Index, const bool);
	void Insert(const int Index, const int);
	void Insert(const int Index, const float, const int Prec = 2);
	void Insert(const int Index, const double, const int Prec = 6);

	//-------------------------------------------------------------------------
	// Remove
	//-------------------------------------------------------------------------
	// Deletes all the characters from this string beginning at a specified position
	// and continuing through the last position.
	void Remove(const int StartIndex);
	// Deletes a specified number of characters from this instance
	// beginning at a specified position.
	void Remove(const int StartIndex, const int Count);

	//-------------------------------------------------------------------------
	// Replace : (const char, const char, ...)
	//-------------------------------------------------------------------------
	// Replaces all occurrences of a specified character in this instance// with another specified character.
	void Replace(const char Char, const char WithChar);
	// Replaces only within a substring of this instance.
	void Replace(const char Char, const char WithChar, const int StartIndex, const int Count);

	//-------------------------------------------------------------------------
	// Replace : (const char *, const char *, ...)
	//-------------------------------------------------------------------------
	// Replaces all occurrences of a specified string in this instance
	// with another specified string.
	// Returns number of replaces.
	// Case insensitive compare can be specified.
	int Replace(const char *String, const char *WithString, const bool NoCase = false);
	// Replaces in a substring beginning from a specified position to the end.
	int Replace(const char *String, const char *WithString, const int StartIndex, const bool NoCase = false);
	// Replaces only within a substring of this instance.
	int Replace(const char *String, const char *WithString, const int StartIndex, const int Count, const bool NoCase = false);
	// Replaces first occurrence of a specified string in this instance with another specified string.
	int ReplaceFirst(const char *String, const char *WithString, const bool NoCase = false);
	// Replaces only within a substring of this instance.
	int ReplaceFirst(const char *String, const char *WithString, const int StartIndex, const int Count, const bool NoCase = false);
	
	//-------------------------------------------------------------------------
	// ReplaceAny : (const char *, const char, ...)
	//-------------------------------------------------------------------------
	// Replaces all occurrences in this instance of every character in the
	// specified list of characters with another specified character.
	void ReplaceAny(const char *Chars, const char WithChar);		// S.ReplaceAny(",;\t", ' ');
	// Replaces only within a substring of this instance.
	void ReplaceAny(const char *Chars, const char WithChar, const int StartIndex, const int Count);

	//-------------------------------------------------------------------------
	// Substring
	//-------------------------------------------------------------------------
	// Retrieves a substring from this instance.
	// The substring starts at a specified character position.
	const cStr Substring(const int StartIndex) const;
	// The substring starts at a specified character position and has a specified length.
	const cStr Substring(const int StartIndex, const int Count) const;

	//-------------------------------------------------------------------------
	// Trim...
	//-------------------------------------------------------------------------
	// Removes all occurrences of a set of characters specified in a string
	// from the beginning of this instance.
	void TrimStart(const char *TrimChars);
	// Removes all occurrences of a set of characters specified in a string
	// from the end of this instance.
	void TrimEnd(const char *TrimChars);
	// Removes all occurrences of a set of characters specified in a string
	// from the beginning and end of this instance.
	void Trim(const char *TrimChars);
	
	//-------------------------------------------------------------------------
	// PadLeft / PadRight
	//-------------------------------------------------------------------------
	// Right - aligns the characters of this instance, padding on the left with
	// spaces or a specified character for a specified total length.
	void PadLeft(const int TotalWidth, const char PaddingChar = ' ');
	// Left - align the characters of this instance, padding on the right with
	// spaces or a specified character for a specified total length.
	void PadRight(const int TotalWidth, const char PaddingChar = ' ');

	//-------------------------------------------------------------------------
	// Contains
	//-------------------------------------------------------------------------
	// Returns a value indicating whether the specified character occurs within this string.
	bool Contains(const char c) const;
	// Returns "true" if the specified string occurs within this string, or if the
	// specified string is empty string or NULL; otherwise, "false".
	// Case insesitive compare can be specified.
	bool Contains(const char *Str, const bool NoCase = false) const;

	//-------------------------------------------------------------------------
	// IndexOf : (const char, ...)
	//-------------------------------------------------------------------------
	// Returns the index position of the first occurrence within this instance
	// where the specified character is found, or -1 if it is not.
	int IndexOf(const char c) const;
	static int IndexOf(const char *Src, const char c);
	// The search starts at a specified character position.
	int IndexOf(const char c, const int StartIndex) const;
	// The search starts at a specified character position and examines a specified number of character positions.
	int IndexOf(const char c, const int StartIndex, const int Count) const;
	
	//-------------------------------------------------------------------------
	// IndexOf : (const char *, ...)
	//-------------------------------------------------------------------------
	// Returns the index position of the first occurrence in this instance
	// where the specified string is found, or -1 if it is not.
	// If the specified string is empty or NULL, the return value is "StartIndex".
	// Case insesitive compare can be specified.
	int IndexOf(const char *Str, const bool NoCase = false) const;
	// The search starts at a specified character position.
	int IndexOf(const char *Str, const int StartIndex, const bool NoCase = false) const;
	// The search starts at a specified character position and examines a specified number of character positions.
	int IndexOf(const char *Str, const int StartIndex, const int Count, const bool NoCase = false) const;
	
	//-------------------------------------------------------------------------
	// IndexOfAny
	//-------------------------------------------------------------------------
	// Returns the index position of the first occurrence in this instance
	// where any character from the specified list of characters is found, or -1 if it is not.
	int IndexOfAny(const char *Chars) const;
	// The search starts at a specified character position.
	int IndexOfAny(const char *Chars, const int StartIndex) const;
	// The search starts at a specified character position and examines a specified number of character positions.
	int IndexOfAny(const char *Chars, const int StartIndex, const int Count) const;
	
	//-------------------------------------------------------------------------
	// LastIndexOf : (const char, ...)
	//-------------------------------------------------------------------------
	// Returns the index position of the last occurrence within this instance
	// where the specified character is found, or -1 if it is not.
	int LastIndexOf(const char c) const;
	// The search starts at a specified character position and
	// proceeds backwards towards the beginning.
	int LastIndexOf(const char c, const int StartIndex) const;
	// The search starts at a specified character position and
	// proceeds backwards towards the beginning until either character is found
	// or a specified number of character positions have been examined.
	int LastIndexOf(const char c, const int StartIndex, const int Count) const;

	//-------------------------------------------------------------------------
	// LastIndexOf : (const char *, ...)
	//-------------------------------------------------------------------------
	// Returns the index position of the last occurrence within this instance
	// where the specified string is found, or -1 if it is not.
	// If the specified string is empty or NULL, the return value is "StartIndex".
	// Case insesitive compare can be specified.
	int LastIndexOf(const char *Str, const bool NoCase = false) const;
	// The search starts at a specified character position and proceeds backwards towards the beginning.
	int LastIndexOf(const char *Str, const int StartIndex, const bool NoCase = false) const;
	// The search starts at a specified character position and proceeds backwards towards the beginning
	// until either the string is found or a specified character positions have been examined.
	int LastIndexOf(const char *Str, const int StartIndex, const int Count, const bool NoCase = false) const;

	//-------------------------------------------------------------------------
	// LastIndexOfAny
	//-------------------------------------------------------------------------
	// Returns the index position of the last occurrence in this instance
	// where any character from the specified list of characters is found, or -1 if it is not.
	int LastIndexOfAny(const char *Chars) const;
	// The search starts at a specified character position and
	// proceeds backwards towards the beginning.
	int LastIndexOfAny(const char *Chars, const int StartIndex) const;
	// The search starts at a specified character position and proceeds
	// backwards towards the beginning until either character in a specified string
	// is found or a specified number of character positions have been examined.
	int LastIndexOfAny(const char *Chars, const int StartIndex, const int Count) const;

	//-------------------------------------------------------------------------
	// ToLower / ToUpper, MakeLower / MakeUpper
	//-------------------------------------------------------------------------
	static char ToLower(const char c);
	static char ToUpper(const char c);
	void MakeLower(); // Converts this string instance to lowercase.
	void MakeUpper(); // Converts this string instance to uppercase.
	const cStr ToLower() const; // Returns a copy of this string converted to lowercase.
	const cStr ToUpper() const; // Returns a copy of this string converted to uppercase.

	//-------------------------------------------------------------------------
	// Join
	//-------------------------------------------------------------------------
	// Concatenates a specified separator between each element of a specified
	// string list, yielding a single concatenated string.
	static const cStr Join(const char *Separator, const cList<cStr> &Strings);
	// Parameters specify the first array element and number of elements to use.
	static const cStr Join(const char *Separator, const cList<cStr> &Strings, const int StartIndex, const int Count);

	//-------------------------------------------------------------------------
	// Split
	//-------------------------------------------------------------------------
	// Fills a list with the substrings from a specified string or this instance
	// that are delimited by a specified characters.
	static void Split(const char *Src, cList<cStr> *List, const char *Delimiters = " ,;\t");
	void Split(cList<cStr> *List, const char *Delimiters = " ,;\t") const;

	//-------------------------------------------------------------------------
	// Format
	//-------------------------------------------------------------------------
	// Returns a copy of the specified format string in which each format item is replaced
	// with the text equivalent of a corresponding value from arguments.
	static const cStr Format(const char *Format, ...);
	static const cStr Formatv(const char *Format, va_list Args); // Uses list of arguments.

	//-------------------------------------------------------------------------
	// CharIs...
	//-------------------------------------------------------------------------
	static bool CharIsLower(const int c);
	static bool CharIsUpper(const int c);
	static bool CharIsAlpha(const int c);
	static bool CharIsNumeric(const int c);
	static bool CharIsHexadecimal(const int c);
	static bool CharIsNewLine(const int c);
	static bool CharIsTab(const int c);
	static bool CharIsWhitespace(const int c);
	static bool CharIsDecimalPoint(const int c);
	static bool CharIsSign(const int c);
	static bool CharIsExponent(const int c);

	//-------------------------------------------------------------------------
	// ToInt(Array) / ToFloat(Array)
	//-------------------------------------------------------------------------
	static bool ToInt(const char *Str, int *Value);
	// [Whitespace]						Consists of space or tab characters, which are ignored.
	// [Sign]							Is either plus (+) or minus (-).
	// Digits							Are one or more decimal digits.

	static bool ToIntArray(const char *Str, cList<int> *IntArray);

	//-------------------------------------------------------------------------
	// GetHashCode
	//-------------------------------------------------------------------------
	int GetHashCode(const bool NoCase = false) const {
		return GetHashCode(m_Str, NoCase);
	}
	static int GetHashCode(const char *Str, const bool NoCase = false);

	//-------------------------------------------------------------------------
	// GetFile... Extension / Name / Base / Path
	//-------------------------------------------------------------------------
	const cStr GetFileExtension() const;
	const cStr GetFileName() const;
	const cStr GetFileBase() const;
	const cStr GetFilePath() const;

	//-------------------------------------------------------------------------
	// RemoveFile... Extension / Name / Path
	//-------------------------------------------------------------------------
	void RemoveFileExtension();
	void RemoveFileName();
	void RemoveFilePath();
	// If string contains absolute path to file below specified "AbsPath", it will be removed
	void RemoveFileAbsPath(const char *AbsPath);

	//-------------------------------------------------------------------------
	// SetFile... (Default)Extension / (Default)Path
	//-------------------------------------------------------------------------
	void SetFileExtension(const char *Extension);
	void SetFileDefaultExtension(const char *DefaultExtension); // If there's no extension set the default.
	void SetFilePath(const char *Path);
	void SetFileDefaultPath(const char *DefaultPath); // If there's no path set the default.
	
	void EnsureTrailingBackslash();
	void SlashesToBackSlashes(); // Replaces '/' with '\\'
	void BackSlashesToSlashes(); // Replaces '\\' with '/'

	//-------------------------------------------------------------------------
	// Unicode
	//-------------------------------------------------------------------------
	struct CodePage {
		enum Enum {
			_1251 = 0,
			Count = 1
		};
	};
	void Decode(const CodePage::Enum _CodePage, cList<word> *UniChars) const;
	bool DecodeUTF8(cList<word> *UniChars) const;
	int CalcUTF8Length(const int StartIndex) const;
	void Init();
private:
	char *m_Str;
	int m_Length, m_Capacity;
	static char def_str;

	struct Param {
		enum Values {
			BaseBufferLength = 20,
			GrowBy = 32
		};
	};
	
	//char m_BaseBuffer[Param::BaseBufferLength];
	bool IsValid() const;
	friend class cXml;
	void Free();
	void EnsureCapacity(const int MinCapacity, const bool KeepOld = true);
	void Resize(const int MinCapacity, const bool KeepOld);
	static cList<char*> pool;
	static cList<char*> free_strings;
	static char* getstr();
	static void freestr(char* s);
};

// cStr::IsValid
inline bool cStr::IsValid() const {
	//cAssert(m_Str != NULL);
	// Seriously slows down Debug build of 3D-Coat
	//cAssert(m_Length == Length(m_Str)); // Cached length should be the same as actual
	//cAssert(m_Capacity >= m_Length); // Capacity should be at least length + 1 (terminating zero)
	return true;
}

// cStr::Init
inline void cStr::Init() {
	m_Length = 0;
	m_Capacity = 0;// Param::BaseBufferLength;
	m_Str = NULL;// m_BaseBuffer;
	//m_Str[0] = '\0';
	// No need in "cAssert(IsValid())", since after "Init" this string is valid for sure
}

// cStr::Free
inline void cStr::Free() {
	// In "Free" we don't care was string valid or not, cause usage of
	// string buffer may be not only for storing one string with terminating zero.
	//cAssert(m_Str != NULL);
	if (m_Capacity >Param::BaseBufferLength) {
		cPool::Free((byte *)m_Str, m_Capacity);
	}
	else if (m_Str)freestr(m_Str);
	Init();
}

// cStr::EnsureCapacity
inline void cStr::EnsureCapacity(const int MinCapacity, const bool KeepOld) {
	if(MinCapacity > m_Capacity) {
		Resize(MinCapacity, KeepOld);
	}
}

//-----------------------------------------------------------------------------
// cStr.ctor
//-----------------------------------------------------------------------------

// cStr.ctor : ()
inline cStr::cStr() {
	Init();
}

// cStr.ctor : (const cStr &)
inline cStr::cStr(const cStr &Src) {
	Init();
	Copy(Src);
}

// cStr.ctor : (const cStr &, const int, const int)
inline cStr::cStr(const cStr &Src, const int StartIndex, const int Count) {
	Init();
	Copy(Src, StartIndex, Count);
}

// cStr.ctor : (const char *)
inline cStr::cStr(const char *Src) {
	Init();
	Copy(Src);
}

// cStr.ctor : (const char *, const int, const int)
inline cStr::cStr(const char *Src, const int StartIndex, const int Count) {
	Init();
	Copy(Src, StartIndex, Count);
}

// cStr.ctor : (const int, const char)
inline cStr::cStr(const int Length, const char Fill) {
	Init();
	SetLength(Length, Fill);
}

//-----------------------------------------------------------------------------
// cStr.dtor
//-----------------------------------------------------------------------------

// cStr.dtor
inline cStr::~cStr() {
	Free();
}

//-----------------------------------------------------------------------------
// cStr::Copy
//-----------------------------------------------------------------------------

// cStr::Copy : (const cStr &)
inline void cStr::Copy(const cStr &Src) {
	cAssert(Src.IsValid());
	Copy(Src.m_Str, 0, Src.m_Length);
}

// cStr::Copy : (const cStr &, const int, const int)
inline void cStr::Copy(const cStr &Src, const int StartIndex, const int Count) {
	cAssert(Src.IsValid());
	Copy(Src.m_Str, StartIndex, Count);
}

// cStr::Copy : (const char *)
inline void cStr::Copy(const char *Src) {
	Copy(Src, 0, Length(Src));
}

// cStr::operator = : (const cStr &)
inline void cStr::operator = (const cStr &Src) {
	Copy(Src);
}

// cStr::operator = : (const char *)
inline void cStr::operator = (const char *Src) {
	Copy(Src);
}

//-----------------------------------------------------------------------------
// cStr::ToCharPtr
//-----------------------------------------------------------------------------

// None of these casters has cAssert on validity, cause during work with
// string buffer as with temp storage, its state most probably is not valid.

// cStr::ToCharPtr : const char * () const
inline const char * cStr::ToCharPtr() const {
	//cAssert(m_Str != NULL);
	return m_Str ? m_Str : &def_str;
}

// cStr::ToCharPtr : char * ()
inline char * cStr::ToCharPtr() {
	return m_Str ? m_Str : &def_str;
}

// cStr::operator const char * () const
inline cStr::operator const char * () const {
	return m_Str ? m_Str : &def_str;
}

// cStr::operator char * ()
inline cStr::operator char * () {
	return m_Str ? m_Str : &def_str;
}

//-----------------------------------------------------------------------------
// cStr::operator []
//-----------------------------------------------------------------------------

// cStr::operator [] const
inline char cStr::operator [] (const int CharIndex) const {
	cAssert(IsValid());
	cAssert(CharIndex >= 0);
	cAssert(CharIndex <= m_Length);
	return m_Str ? m_Str[CharIndex] : 0;
}

// cStr::operator []
inline char & cStr::operator [] (const int CharIndex) {
	cAssert(IsValid());
	cAssert(CharIndex >= 0);
	cAssert(CharIndex <= m_Length);
	return m_Str ? m_Str[CharIndex] : def_str;
}

//-----------------------------------------------------------------------------
// cStr::Length
//-----------------------------------------------------------------------------

// cStr::Length : ()
inline int cStr::Length() const {
	cAssert(IsValid());
	return m_Length;
}

// cStr::Length : (const char *)
inline int cStr::Length(const char *Str) {
	if(Str == NULL) {
		return 0;
	}
	
	int l;
	
	for(l = 0; Str[l] != '\0'; l++) {
	}
	return l;
}

//-----------------------------------------------------------------------------
// cStr::SetLength / cStr::CalcLength
//-----------------------------------------------------------------------------

// cStr::SetLength : (const int, const char)
inline void cStr::SetLength(const int Length, const char Fill) {
	cAssert(IsValid());
	cAssert(Length >= 0);

	int i;

	if(Length <= 0) {
		Free();
	} else {
		EnsureCapacity(Length + 1);
		for(i = 0; i < Length; i++) {
			m_Str[i] = Fill;
		}
		m_Length = Length;
		m_Str[Length] = '\0';

	}
}

// cStr::CalcLength
inline void cStr::CalcLength() {
	m_Length = Length(m_Str);
	cAssert(IsValid());
}

// cStr::Fill : (const char)
inline void cStr::Fill(const char c) {
	cAssert(IsValid());
	
	int i;
	
	for(i = 0; i < m_Length; i++) {
		m_Str[i] = c;
	}
}

//-----------------------------------------------------------------------------
// cStr::IsEmpty, cStr::Clear
//-----------------------------------------------------------------------------

// cStr::IsEmpty
inline bool cStr::IsEmpty() const {
	return Equals(m_Str, "");
}

// cStr::Clear
inline void cStr::Clear() {
	if (m_Str){
		EnsureCapacity(1, false);
		m_Str[0] = '\0';
		m_Length = 0;
	}
	else m_Length = 0;
}

//-----------------------------------------------------------------------------
// cStr::Equals(NoCase) (==, !=)
//-----------------------------------------------------------------------------

// cStr::Equals : bool (const char *, const char *)
inline bool cStr::Equals(const char *l, const char *r) {
	return Compare(l, r) == 0;
}

// cStr::Equals : bool (const char *, const cStr &)
inline bool cStr::Equals(const char *l, const cStr &r) {
	cAssert(r.IsValid());
	return Compare(l, r.ToCharPtr()) == 0;
}

// cStr::Equals : bool (const cStr &, const char *)
inline bool cStr::Equals(const cStr &l, const char *r) {
	cAssert(l.IsValid());
	return Compare(l.ToCharPtr(), r) == 0;
}

// cStr::Equals : bool (const cStr &, const cStr &)
inline bool cStr::Equals(const cStr &l, const cStr &r) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return Compare(l.ToCharPtr(), r.ToCharPtr()) == 0;
}

// cStr::Equals : bool (const cStr *, const cStr *)
inline bool cStr::Equals(const cStr *l, const cStr *r) {
	cAssert(l != NULL);
	cAssert(r != NULL);
	cAssert(l->IsValid());
	cAssert(r->IsValid());
	return Compare(l->ToCharPtr(), r->ToCharPtr()) == 0;
}

// cStr.friend operator == : bool (const char *, const cStr &)
inline bool operator == (const char *l, const cStr &r) {
	cAssert(r.IsValid());
	return cStr::Compare(l, r.ToCharPtr()) == 0;
}

// cStr.friend operator == : bool (const cStr &, const char *)
inline bool operator == (const cStr &l, const char *r) {
	cAssert(l.IsValid());
	return cStr::Compare(l.ToCharPtr(), r) == 0;
}

// cStr.friend operator == : bool (const cStr &, const cStr &)
inline bool operator == (const cStr &l, const cStr &r) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return cStr::Compare(l.ToCharPtr(), r.ToCharPtr()) == 0;
}

// cStr.friend operator != : bool (const char *, const cStr &)
inline bool operator != (const char *l, const cStr &r) {
	cAssert(r.IsValid());
	return cStr::Compare(l, r.ToCharPtr()) != 0;
}

// cStr.friend operator != : bool (const cStr &, const char *)
inline bool operator != (const cStr &l, const char *r) {
	cAssert(l.IsValid());
	return cStr::Compare(l.ToCharPtr(), r) != 0;
}

// cStr.friend operator != : bool (const cStr &, const cStr &)
inline bool operator != (const cStr &l, const cStr &r) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return cStr::Compare(l.ToCharPtr(), r.ToCharPtr()) != 0;
}

// cStr::Equals : bool (const char *, const char *, const int)
inline bool cStr::Equals(const char *l, const char *r, const int MaxLength) {
	return Compare(l, r, MaxLength) == 0;
}

// cStr::Equals : bool (const char *, const cStr &, const int)
inline bool cStr::Equals(const char *l, const cStr &r, const int MaxLength) {
	cAssert(r.IsValid());
	return Compare(l, r.ToCharPtr(), MaxLength) == 0;
}

// cStr::Equals : bool (const cStr &, const char *, const int)
inline bool cStr::Equals(const cStr &l, const char *r, const int MaxLength) {
	cAssert(l.IsValid());
	return Compare(l.ToCharPtr(), r, MaxLength) == 0;
}

// cStr::Equals : bool (const cStr &, const cStr &, const int)
inline bool cStr::Equals(const cStr &l, const cStr &r, const int MaxLength) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return Compare(l.ToCharPtr(), r.ToCharPtr(), MaxLength) == 0;
}

// cStr::EqualsNoCase : bool (const char *, const char *)
inline bool cStr::EqualsNoCase(const char *l, const char *r) {
	return CompareNoCase(l, r) == 0;
}

// cStr::EqualsNoCase : bool (const char *, const cStr &)
inline bool cStr::EqualsNoCase(const char *l, const cStr &r) {
	cAssert(r.IsValid());
	return CompareNoCase(l, r.ToCharPtr()) == 0;
}

// cStr::EqualsNoCase : bool (const cStr &, const char *)
inline bool cStr::EqualsNoCase(const cStr &l, const char *r) {
	cAssert(l.IsValid());
	return CompareNoCase(l.ToCharPtr(), r) == 0;
}

// cStr::EqualsNoCase : bool (const cStr &, const cStr &)
inline bool cStr::EqualsNoCase(const cStr &l, const cStr &r) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return CompareNoCase(l.ToCharPtr(), r.ToCharPtr()) == 0;
}

// cStr::EqualsNoCase : bool (const cStr *, const cStr *)
inline bool cStr::EqualsNoCase(const cStr *l, const cStr *r) {
	cAssert(l != NULL);
	cAssert(r != NULL);
	cAssert(l->IsValid());
	cAssert(r->IsValid());
	return CompareNoCase(l->ToCharPtr(), r->ToCharPtr()) == 0;
}

// cStr::EqualsNoCase : bool (const char *, const char *, const int)
inline bool cStr::EqualsNoCase(const char *l, const char *r, const int MaxLength) {
	return CompareNoCase(l, r, MaxLength) == 0;
}

// cStr::EqualsNoCase : bool (const char *, const cStr &, const int)
inline bool cStr::EqualsNoCase(const char *l, const cStr &r, const int MaxLength) {
	cAssert(r.IsValid());
	return CompareNoCase(l, r.ToCharPtr(), MaxLength) == 0;
}

// cStr::EqualsNoCase : bool (const cStr &, const char *, const int)
inline bool cStr::EqualsNoCase(const cStr &l, const char *r, const int MaxLength) {
	cAssert(l.IsValid());
	return CompareNoCase(l.ToCharPtr(), r, MaxLength) == 0;
}

// cStr::EqualsNoCase : bool (const cStr &, const cStr &, const int)
inline bool cStr::EqualsNoCase(const cStr &l, const cStr &r, const int MaxLength) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return CompareNoCase(l.ToCharPtr(), r.ToCharPtr(), MaxLength) == 0;
}

//-----------------------------------------------------------------------------
// cStr::Compare(NoCase) (>, >=, <, <=)
//-----------------------------------------------------------------------------

// cStr::Compare : int (const char *, const char *)
inline int cStr::Compare(const char *l, const char *r) {
	return Compare(l, r, -1);
}

// cStr::Compare : int (const char *, const cStr &)
inline int cStr::Compare(const char *l, const cStr &r) {
	cAssert(r.IsValid());
	return Compare(l, r.ToCharPtr());
}

// cStr::Compare : int (const cStr &, const char *)
inline int cStr::Compare(const cStr &l, const char *r) {
	cAssert(l.IsValid());
	return Compare(l.ToCharPtr(), r);
}

// cStr::Compare : int (const cStr &, const cStr &)
inline int cStr::Compare(const cStr &l, const cStr &r) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return Compare(l.ToCharPtr(), r.ToCharPtr());
}

// cStr::Compare : int (const cStr *, const cStr *)
inline int cStr::Compare(const cStr *l, const cStr *r) {
	cAssert(l != NULL);
	cAssert(r != NULL);
	cAssert(l->IsValid());
	cAssert(r->IsValid());
	return Compare(l->ToCharPtr(), r->ToCharPtr());
}

// cStr.friend operator > : bool (const char *, const cStr &)
inline bool operator > (const char *l, const cStr &r) {
	cAssert(r.IsValid());
	return cStr::Compare(l, r.ToCharPtr()) > 0;
}

// cStr.friend operator > : bool (const cStr &, const char *)
inline bool operator > (const cStr &l, const char *r) {
	cAssert(l.IsValid());
	return cStr::Compare(l.ToCharPtr(), r) > 0;
}

// cStr.friend operator > : bool (const cStr &, const cStr &)
inline bool operator > (const cStr &l, const cStr &r) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return cStr::Compare(l.ToCharPtr(), r.ToCharPtr()) > 0;
}

// cStr.friend operator >= : bool (const char *, const cStr &)
inline bool operator >= (const char *l, const cStr &r) {
	cAssert(r.IsValid());
	return cStr::Compare(l, r.ToCharPtr()) >= 0;
}

// cStr.friend operator >= : bool (const cStr &, const char *)
inline bool operator >= (const cStr &l, const char *r) {
	cAssert(l.IsValid());
	return cStr::Compare(l.ToCharPtr(), r) >= 0;
}

// cStr.friend operator >= : bool (const cStr &, const cStr &)
inline bool operator >= (const cStr &l, const cStr &r) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return cStr::Compare(l.ToCharPtr(), r.ToCharPtr()) >= 0;
}

// cStr.friend operator < : bool (const char *, const cStr &)
inline bool operator < (const char *l, const cStr &r) {
	cAssert(r.IsValid());
	return cStr::Compare(l, r.ToCharPtr()) < 0;
}

// cStr.friend operator < : bool (const cStr &, const char *)
inline bool operator < (const cStr &l, const char *r) {
	cAssert(l.IsValid());
	return cStr::Compare(l.ToCharPtr(), r) < 0;
}

// cStr.friend operator < : bool (const cStr &, const cStr &)
inline bool operator < (const cStr &l, const cStr &r) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return cStr::Compare(l.ToCharPtr(), r.ToCharPtr()) < 0;
}

// cStr.friend operator <= : bool (const char *, const cStr &)
inline bool operator <= (const char *l, const cStr &r) {
	cAssert(r.IsValid());
	return cStr::Compare(l, r.ToCharPtr()) <= 0;
}

// cStr.friend operator <= : bool (const cStr &, const char *)
inline bool operator <= (const cStr &l, const char *r) {
	cAssert(l.IsValid());
	return cStr::Compare(l.ToCharPtr(), r) <= 0;
}

// cStr.friend operator <= : bool (const cStr &, const cStr &)
inline bool operator <= (const cStr &l, const cStr &r) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return cStr::Compare(l.ToCharPtr(), r.ToCharPtr()) <= 0;
}

// cStr::Compare : int (const char *, const cStr &, const int)
inline int cStr::Compare(const char *l, const cStr &r, const int MaxLength) {
	cAssert(r.IsValid());
	return Compare(l, r.ToCharPtr(), MaxLength);
}

// cStr::Compare : int (const cStr &, const char *, const int)
inline int cStr::Compare(const cStr &l, const char *r, const int MaxLength) {
	cAssert(l.IsValid());
	return Compare(l.ToCharPtr(), r, MaxLength);
}

// cStr::Compare : int (const cStr &, const cStr &, const int)
inline int cStr::Compare(const cStr &l, const cStr &r, const int MaxLength) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return Compare(l.ToCharPtr(), r.ToCharPtr(), MaxLength);
}

// cStr::CompareNoCase : int (const char *, const char *)
inline int cStr::CompareNoCase(const char *l, const char *r) {
	return CompareNoCase(l, r, -1);
}

// cStr::CompareNoCase : int (const char *, const cStr &)
inline int cStr::CompareNoCase(const char *l, const cStr &r) {
	cAssert(r.IsValid());
	return CompareNoCase(l, r.ToCharPtr());
}

// cStr::CompareNoCase : int (const cStr &, const char *)
inline int cStr::CompareNoCase(const cStr &l, const char *r) {
	cAssert(l.IsValid());
	return CompareNoCase(l.ToCharPtr(), r);
}

// cStr::CompareNoCase : int (const cStr &, const cStr &)
inline int cStr::CompareNoCase(const cStr &l, const cStr &r) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return CompareNoCase(l.ToCharPtr(), r.ToCharPtr());
}

// cStr::CompareNoCase : int (const cStr *, const cStr *)
inline int cStr::CompareNoCase(const cStr *l, const cStr *r) {
	cAssert(l != NULL);
	cAssert(r != NULL);
	cAssert(l->IsValid());
	cAssert(r->IsValid());
	return CompareNoCase(l->ToCharPtr(), r->ToCharPtr());
}

// cStr::CompareNoCase : int (const char *, const cStr &, const int)
inline int cStr::CompareNoCase(const char *l, const cStr &r, const int MaxLength) {
	cAssert(r.IsValid());
	return CompareNoCase(l, r.ToCharPtr(), MaxLength);
}

// cStr::CompareNoCase : int (const cStr &, const char *, const int)
inline int cStr::CompareNoCase(const cStr &l, const char *r, const int MaxLength) {
	cAssert(l.IsValid());
	return CompareNoCase(l.ToCharPtr(), r, MaxLength);
}

// cStr::CompareNoCase : int (const cStr &, const cStr &, const int)
inline int cStr::CompareNoCase(const cStr &l, const cStr &r, const int MaxLength) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return CompareNoCase(l.ToCharPtr(), r.ToCharPtr(), MaxLength);
}

//-----------------------------------------------------------------------------
// cStr::EqualsPath
//-----------------------------------------------------------------------------

// cStr::EqualsPath : bool (const char *, const char *)
inline bool cStr::EqualsPath(const char *l, const char *r) {
	return ComparePath(l, r) == 0;
}

// cStr::EqualsPath : bool (const char *, const cStr &)
inline bool cStr::EqualsPath(const char *l, const cStr &r) {
	cAssert(r.IsValid());
	return ComparePath(l, r.ToCharPtr()) == 0;
}

// cStr::EqualsPath : bool (const cStr &, const char *)
inline bool cStr::EqualsPath(const cStr &l, const char *r) {
	cAssert(l.IsValid());
	return ComparePath(l.ToCharPtr(), r) == 0;
}

// cStr::EqualsPath : bool (const cStr &, const cStr &)
inline bool cStr::EqualsPath(const cStr &l, const cStr &r) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return ComparePath(l.ToCharPtr(), r.ToCharPtr()) == 0;
}

// cStr::EqualsPath : bool (const cStr *, const cStr *)
inline bool cStr::EqualsPath(const cStr *l, const cStr *r) {
	cAssert(l != NULL);
	cAssert(r != NULL);
	cAssert(l->IsValid());
	cAssert(r->IsValid());
	return ComparePath(l->ToCharPtr(), r->ToCharPtr()) == 0;
}

// cStr::EqualsPath : bool (const char *, const char *, const int)
inline bool cStr::EqualsPath(const char *l, const char *r, const int MaxLength) {
	return ComparePath(l, r, MaxLength) == 0;
}

// cStr::EqualsPath : bool (const char *, const cStr &, const int)
inline bool cStr::EqualsPath(const char *l, const cStr &r, const int MaxLength) {
	cAssert(r.IsValid());
	return ComparePath(l, r.ToCharPtr(), MaxLength) == 0;
}

// cStr::EqualsPath : bool (const cStr &, const char *, const int)
inline bool cStr::EqualsPath(const cStr &l, const char *r, const int MaxLength) {
	cAssert(l.IsValid());
	return ComparePath(l.ToCharPtr(), r, MaxLength) == 0;
}

// cStr::EqualsPath : bool (const cStr &, const cStr &, const int)
inline bool cStr::EqualsPath(const cStr &l, const cStr &r, const int MaxLength) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return ComparePath(l.ToCharPtr(), r.ToCharPtr(), MaxLength) == 0;
}

//-----------------------------------------------------------------------------
// cStr::ComparePath
//-----------------------------------------------------------------------------

// cStr::ComparePath : int (const char *, const char *)
inline int cStr::ComparePath(const char *l, const char *r) {
	return ComparePath(l, r, -1);
}

// cStr::ComparePath : int (const char *, const cStr &)
inline int cStr::ComparePath(const char *l, const cStr &r) {
	cAssert(r.IsValid());
	return ComparePath(l, r.ToCharPtr());
}

// cStr::ComparePath : int (const cStr &, const char *)
inline int cStr::ComparePath(const cStr &l, const char *r) {
	cAssert(l.IsValid());
	return ComparePath(l.ToCharPtr(), r);
}

// cStr::ComparePath : int (const cStr &, const cStr &)
inline int cStr::ComparePath(const cStr &l, const cStr &r) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return ComparePath(l.ToCharPtr(), r.ToCharPtr());
}

// cStr::ComparePath : int (const cStr *, const cStr *)
inline int cStr::ComparePath(const cStr *l, const cStr *r) {
	cAssert(l != NULL);
	cAssert(r != NULL);
	cAssert(l->IsValid());
	cAssert(r->IsValid());
	return ComparePath(l->ToCharPtr(), r->ToCharPtr());
}

// cStr::ComparePath : int (const char *, const cStr &, const int)
inline int cStr::ComparePath(const char *l, const cStr &r, const int MaxLength) {
	cAssert(r.IsValid());
	return ComparePath(l, r.ToCharPtr(), MaxLength);
}

// cStr::ComparePath : int (const cStr &, const char *, const int)
inline int cStr::ComparePath(const cStr &l, const char *r, const int MaxLength) {
	cAssert(l.IsValid());
	return ComparePath(l.ToCharPtr(), r, MaxLength);
}

// cStr::ComparePath : int (const cStr &, const cStr &, const int)
inline int cStr::ComparePath(const cStr &l, const cStr &r, const int MaxLength) {
	cAssert(l.IsValid());
	cAssert(r.IsValid());
	return ComparePath(l.ToCharPtr(), r.ToCharPtr(), MaxLength);
}

//-----------------------------------------------------------------------------
// cStr::StartsWith / cStr::EndsWith
//-----------------------------------------------------------------------------

// cStr::StartsWith
inline bool cStr::StartsWith(const char *Str, const bool NoCase) const {
	cAssert(IsValid());
	cAssert(Str != NULL);
	
	int L;

	L = Length(Str);
	return NoCase ? EqualsNoCase(m_Str, Str, L) : Equals(m_Str, Str, L);
}

// cStr::EndsWith
inline bool cStr::EndsWith(const char *Str, const bool NoCase) const {
	cAssert(IsValid());
	cAssert(Str != NULL);

	int L;

	L = Length(Str);
	if(L > m_Length) {
		return false;
	}
	
	return NoCase ? EqualsNoCase(&m_Str[m_Length - L], Str) : Equals(&m_Str[m_Length - L], Str);
} // cStr::EndsWith

//-----------------------------------------------------------------------------
// cStr::ToString
//-----------------------------------------------------------------------------

// cStr::ToString : (const bool)
inline const cStr cStr::ToString(const bool b) {
	return b ? "True" : "False";
}

// cStr::ToString : (const int)
inline const cStr cStr::ToString(const int i) {
	return Format("%d", i);
}

// cStr::ToString : (const int, const int)
inline const cStr cStr::ToString(const int i, const int Prec) {
	return Format(Format("%%0%dd", Prec), i);
}

// cStr::ToString : (const float, ...)
inline const cStr cStr::ToString(const float f, const int Prec) {
	return Format(Format("%%.%df", Prec), f);
}

// cStr::ToString : (const double, ...)
inline const cStr cStr::ToString(const double d, const int Prec) {
	return Format(Format("%%.%df", Prec), d);
}

//-----------------------------------------------------------------------------
// cStr::Append
//-----------------------------------------------------------------------------

// cStr::Append : (const cStr &)
inline void cStr::Append(const cStr &Src) {
	Append(Src, 0, Src.m_Length);
}

// cStr::Append : (const char *)
inline void cStr::Append(const char *Src) {
	Append(Src, 0, Length(Src));
}

// cStr::Append : (const char)
inline void cStr::Append(const char c) {
	Append(c, 1);
}

// cStr::Append : (const bool)
inline void cStr::Append(const bool b) {
	Append(ToString(b));
}

// cStr::Append : (const int)
inline void cStr::Append(const int i) {
	Append(ToString(i));
}

// cStr::Append : (const float, const int)
inline void cStr::Append(const float f, const int Prec) {
	Append(ToString(f, Prec));
}

// cStr::Append : (const double, const int)
inline void cStr::Append(const double d, const int Prec) {
	Append(ToString(d, Prec));
}

//*****************************************************************************
// cStr::operator +=
//*****************************************************************************

// cStr::operator += : (const cStr &)
inline void cStr::operator += (const cStr &Src) {
	Append(Src);
}

// cStr::operator += : (const char *)
inline void cStr::operator += (const char *Src) {
	Append(Src);
}

// cStr::operator += : (const char)
inline void cStr::operator += (const char c) {
	Append(c);
}

// cStr::operator += (const bool)
inline void cStr::operator += (const bool b) {
	Append(b);
}

// cStr::operator += : (const int)
inline void cStr::operator += (const int i) {
	Append(i);
}

// cStr::operator += : (const float)
inline void cStr::operator += (const float f) {
	Append(f);
}

// cStr::operator += : (const double)
inline void cStr::operator += (const double d) {
	Append(d);
}

//*****************************************************************************
// cStr::operator <<
//*****************************************************************************

// cStr::operator << : (const cStr &)
inline cStr & cStr::operator << (const cStr &Src) {
	Append(Src);
	return *this;
}

// cStr::operator << : (const char *)
inline cStr & cStr::operator << (const char *Src) {
	Append(Src);
	return *this;
}

// cStr::operator << : (const char)
inline cStr & cStr::operator << (const char c) {
	Append(c);
	return *this;
}

// cStr::operator << : (const bool)
inline cStr & cStr::operator << (const bool b) {
	Append(b);
	return *this;
}

// cStr::operator << : (const int)
inline cStr & cStr::operator << (const int i) {
	Append(i);
	return *this;
}

// cStr::operator << : (const float)
inline cStr & cStr::operator << (const float f) {
	Append(f);
	return *this;
}

// cStr::operator << : (const double)
inline cStr & cStr::operator << (const double d) {
	Append(d);
	return *this;
}

//*****************************************************************************
// cStr::friend operator +
//*****************************************************************************

// cStr::friend operator + : (const cStr &, const cStr &)
inline const cStr operator + (const cStr &l, const cStr &r) {
	cStr t(l);
	t.Append(r);
	return t;
}

// cStr::friend operator + : (const cStr &, const char *)
inline const cStr operator + (const cStr &l, const char *r) {
	cStr t(l);
	t.Append(r);
	return t;
}

// cStr::friend operator + : (const char *, const cStr &)
inline const cStr operator + (const char *l, const cStr &r) {
	cStr t(l);
	t.Append(r);
	return t;
}

// cStr::friend operator + : (const cStr &, const char)
inline const cStr operator + (const cStr &l, const char c) {
	cStr t(l);
	t.Append(c);
	return t;
}

// cStr::friend operator + : (const char, const cStr &)
inline const cStr operator + (const char c, const cStr &r) {
	cStr t;
	t.Append(c);
	t.Append(r);
	return t;
}

// cStr::friend operator + : (const cStr &, const bool)
inline const cStr operator + (const cStr &l, const bool b) {
	cStr t(l);
	t.Append(b);
	return t;
}

// cStr::friend operator + : (const bool, const cStr &)
inline const cStr operator + (const bool b, const cStr &r) {
	cStr t;
	t.Append(b);
	t.Append(r);
	return t;
}

// cStr::friend operator + : (const cStr &, const int)
inline const cStr operator + (const cStr &l, const int i) {
	cStr t(l);
	t.Append(i);
	return t;
}

// cStr::friend operator + : (const int, const cStr &)
inline const cStr operator + (const int i, const cStr &r) {
	cStr t;
	t.Append(i);
	t.Append(r);
	return t;
}

// cStr::friend operator + : (const cStr &, const float)
inline const cStr operator + (const cStr &l, const float f) {
	cStr t(l);
	t.Append(f);
	return t;
}

// cStr::friend operator + : (const float, const cStr &)
inline const cStr operator + (const float f, const cStr &r) {
	cStr t;
	t.Append(f);
	t.Append(r);
	return t;
}

// cStr::friend operator + : (const cStr &, const double)
inline const cStr operator + (const cStr &l, const double d) {
	cStr t(l);
	t.Append(d);
	return t;
}

// cStr::friend operator + : (const double, const cStr &)
inline const cStr operator + (const double d, const cStr &r) {
	cStr t;
	t.Append(d);
	t.Append(r);
	return t;
}

//-----------------------------------------------------------------------------
// cStr::Insert
//-----------------------------------------------------------------------------

// cStr::Insert : (..., const cStr &)
inline void cStr::Insert(const int Index, const cStr &Src) {
	Insert(Index, Src, 0, Src.m_Length);
}

// cStr::Insert : (..., const char *)
inline void cStr::Insert(const int Index, const char *Src) {
	Insert(Index, Src, 0, Length(Src));
}

// cStr::Insert : (..., const char)
inline void cStr::Insert(const int Index, const char c) {
	Insert(Index, c, 1);
}

// cStr::Insert : (..., const bool)
inline void cStr::Insert(const int Index, const bool b) {
	Insert(Index, ToString(b));
}

// cStr::Insert : (..., const int)
inline void cStr::Insert(const int Index, const int i) {
	Insert(Index, ToString(i));
}

// cStr::Insert : (..., const float, ...)
inline void cStr::Insert(const int Index, const float f, const int Prec) {
	Insert(Index, ToString(f, Prec));
}

// cStr::Insert : (..., const double, ...)
inline void cStr::Insert(const int Index, const double d, const int Prec) {
	Insert(Index, ToString(d, Prec));
}

//-----------------------------------------------------------------------------
// cStr::Remove
//-----------------------------------------------------------------------------

// cStr::Remove : (const int)
inline void cStr::Remove(const int StartIndex) {
	Remove(StartIndex, m_Length - StartIndex);
}

// cStr::Remove : (const int, const int)
inline void cStr::Remove(const int StartIndex, const int Count) {
	cAssert(IsValid());
	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= m_Length);
	
	if(Count > 0) {
		memmove(&m_Str[StartIndex], &m_Str[StartIndex + Count], m_Length - (StartIndex + Count) + 1);
		m_Length -= Count;
	}
}

//-----------------------------------------------------------------------------
// cStr::Replace : (const char, const char, ...)
//-----------------------------------------------------------------------------

// cStr::Replace : (const char, const char)
inline void cStr::Replace(const char Char, const char WithChar) {
	Replace(Char, WithChar, 0, m_Length);
}

//-----------------------------------------------------------------------------
// cStr::Replace : (const char *, const char *, ...)
//-----------------------------------------------------------------------------

// cStr::Replace : (const char *, const char *)
inline int cStr::Replace(const char *String, const char *WithString, const bool NoCase) {
	return Replace(String, WithString, 0, m_Length, NoCase);
}

// cStr::Replace : (const char *, const char *, const int)
inline int cStr::Replace(const char *String, const char *WithString, const int StartIndex, const bool NoCase) {
	return Replace(String, WithString, StartIndex, m_Length - StartIndex, NoCase);
}

//-----------------------------------------------------------------------------
// cStr::ReplaceFirst : (const char *, const char *, ...)
//-----------------------------------------------------------------------------

// cStr::ReplaceFirst : (const char *, const char *)
inline int cStr::ReplaceFirst(const char *String, const char *WithString, const bool NoCase) {
	return ReplaceFirst(String, WithString, 0, m_Length, NoCase);
}

//-----------------------------------------------------------------------------
// cStr::ReplaceAny : (const char *, const char, ...)
//-----------------------------------------------------------------------------

// cStr::ReplaceAny : (const char *, const char)
inline void cStr::ReplaceAny(const char *Chars, const char WithChar) {
	ReplaceAny(Chars, WithChar, 0, m_Length);
}

//-----------------------------------------------------------------------------
// cStr::Substring
//-----------------------------------------------------------------------------

// cStr::Substring : (const int)
inline const cStr cStr::Substring(const int StartIndex) const {
	cAssert(IsValid());
	cAssert(StartIndex >= 0);
	cAssert(StartIndex <= m_Length); // For empty string returns empty string
	return cStr(m_Str, StartIndex, m_Length - StartIndex);
}

// cStr::Substring : (const int, const int)
inline const cStr cStr::Substring(const int StartIndex, const int Count) const {
	cAssert(IsValid());
	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= m_Length);
	return cStr(m_Str, StartIndex, Count);
}

//-----------------------------------------------------------------------------
// cStr::Trim...
//-----------------------------------------------------------------------------

// cStr::TrimStart
inline void cStr::TrimStart(const char *TrimChars) {
	cAssert(IsValid());
	if (m_Str){
		cStr t(TrimChars);
		while (t.Contains(m_Str[0])) {
			memmove(&m_Str[0], &m_Str[1], m_Length);
			m_Length--;
		}
	}
}

// cStr::TrimEnd
inline void cStr::TrimEnd(const char *TrimChars) {
	cAssert(IsValid());
	if (m_Str){
		int i;
		cStr t(TrimChars);
		for (i = m_Length; i > 0 && t.Contains(m_Str[i - 1]); i--) {
			m_Str[i - 1] = '\0';
			m_Length--;
		}
	}
}

// cStr::Trim
inline void cStr::Trim(const char *TrimChars) {
	TrimStart(TrimChars);
	TrimEnd(TrimChars);
}

//-----------------------------------------------------------------------------
// cStr::PadLeft / PadRight
//-----------------------------------------------------------------------------

// cStr::PadLeft
inline void cStr::PadLeft(const int TotalWidth, const char PaddingChar) {
	cAssert(IsValid());
	cAssert(TotalWidth >= 0);
	
	const char TrimChars[2] = {
		PaddingChar, '\0'
	};
	
	TrimStart(TrimChars);
	if(TotalWidth > m_Length) {
		Insert(0, PaddingChar, TotalWidth - m_Length);
	}
}

// cStr::PadRight
inline void cStr::PadRight(const int TotalWidth, const char PaddingChar) {
	cAssert(IsValid());
	cAssert(TotalWidth >= 0);

	const char TrimChars[2] = {
		PaddingChar, '\0'
	};

	TrimEnd(TrimChars);
	if(TotalWidth > m_Length) {
		Append(PaddingChar, TotalWidth - m_Length);
	}
}

//-----------------------------------------------------------------------------
// cStr::Contains
//-----------------------------------------------------------------------------

// cStr::Contains : (const char)
inline bool cStr::Contains(const char c) const {
	return IndexOf(c, 0, m_Length) != -1;
}

// cStr::Contains : (const char *, const bool)
inline bool cStr::Contains(const char *Str, const bool NoCase) const {
	return IndexOf(Str, 0, m_Length, NoCase) != -1;
}

//-----------------------------------------------------------------------------
// cStr::IndexOf : (const char, ...)
//-----------------------------------------------------------------------------

// cStr::IndexOf : (const char)
inline int cStr::IndexOf(const char c) const {
	return IndexOf(c, 0, m_Length);
}

// cStr::IndexOf : (const char *, const char)
inline int cStr::IndexOf(const char *Src, const char c) {
	cAssert(Src != NULL);
	int i = -1, j;
	if(Src != NULL) {
		j = 0;
		while(*Src != '\0') {
			if(*Src == c) {
				i = j;
				break;
			}
			Src++;
			j++;
		}
	}
	return i;
}

// cStr::IndexOf : (const char, const int)
inline int cStr::IndexOf(const char c, const int StartIndex) const {
	return IndexOf(c, StartIndex, m_Length - StartIndex);
}

// cStr::IndexOf : (const char, const int, const int)
inline int cStr::IndexOf(const char c, const int StartIndex, const int Count) const {
	cAssert(IsValid());
	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= m_Length);
	if (m_Str){
		int i1, i;

		i1 = StartIndex + Count - 1;
		for (i = StartIndex; i <= i1; i++) {
			if (m_Str[i] == c) {
				return i;
			}
		}
	}

	return -1;
} // cStr::IndexOf : (const char, ...)

//-----------------------------------------------------------------------------
// cStr::IndexOf : (const char *, ...)
//-----------------------------------------------------------------------------

// cStr::IndexOf : (const char *, const bool)
inline int cStr::IndexOf(const char *Str, const bool NoCase) const {
	return IndexOf(Str, 0, m_Length, NoCase);
}

// cStr::IndexOf : (const char *, const int, const bool)
inline int cStr::IndexOf(const char *Str, const int StartIndex, const bool NoCase) const {
	return IndexOf(Str, StartIndex, m_Length - StartIndex, NoCase);
}

// cStr::IndexOf : (const char *, const int, const int, const bool)
inline int cStr::IndexOf(const char *Str, const int StartIndex, const int Count, const bool NoCase) const {
	cAssert(IsValid());
	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= m_Length);
	if (m_Str){
		int L, i1, i;

		L = Length(Str);
		i1 = StartIndex + Count - 1;

		for (i = StartIndex; i <= i1; i++) {
			if (NoCase ? EqualsNoCase(&m_Str[i], Str, L) : Equals(&m_Str[i], Str, L)) {
				return i;
			}
		}
	}

	return -1;
} // cStr::IndexOf : (const char *, ...)

//-----------------------------------------------------------------------------
// cStr::IndexOfAny
//-----------------------------------------------------------------------------

// cStr::IndexOfAny : (const char *)
inline int cStr::IndexOfAny(const char *Chars) const {
	return IndexOfAny(Chars, 0, m_Length);
}

// cStr::IndexOfAny : (const char *, const int)
inline int cStr::IndexOfAny(const char *Chars, const int StartIndex) const {
	return IndexOfAny(Chars, StartIndex, m_Length - StartIndex);
}

// cStr::IndexOfAny : (const char *, const int, const int)
inline int cStr::IndexOfAny(const char *Chars, const int StartIndex, const int Count) const {
	cAssert(IsValid());
	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= m_Length);
	if (m_Str){
		int i1, i;
		char c;
		cStr t(Chars);

		i1 = StartIndex + Count - 1;
		for (i = StartIndex; i <= i1; i++) {
			c = m_Str[i];
			if (t.Contains(c)) {
				return i;
			}
		}
	}

	return -1;
} // cStr::IndexOfAny : (const char *, ...)

//-----------------------------------------------------------------------------
// cStr::LastIndexOf : (const char, ...)
//-----------------------------------------------------------------------------

// cStr::LastIndexOf : (const char)
inline int cStr::LastIndexOf(const char c) const {
	return LastIndexOf(c, m_Length - 1, m_Length);
}

// cStr::LastIndexOf : (const char, const int)
inline int cStr::LastIndexOf(const char c, const int StartIndex) const {
	return LastIndexOf(c, StartIndex, StartIndex + 1);
}

// cStr::LastIndexOf : (const char, const int, const int)
inline int cStr::LastIndexOf(const char c, const int StartIndex, const int Count) const {
	cAssert(IsValid());
	cAssert(Count >= 0);
	
	if (m_Length == 0 || m_Str == NULL) {
		cAssert(StartIndex >= -1);
		cAssert(StartIndex <= 0);

		return -1;
	}
	
	cAssert(StartIndex >= 0);
	cAssert(StartIndex < m_Length);
	cAssert(Count <= StartIndex + 1);

	int i0, i;
	
	i0 = StartIndex - Count + 1;
	for(i = StartIndex; i >= i0; i--) {
		if(c == m_Str[i]) {
			return i;
		}
	}
	
	return -1;
} // cStr::LastIndexOf : (const char, ...)

//-----------------------------------------------------------------------------
// cStr::LastIndexOf : (const char *, ...)
//-----------------------------------------------------------------------------

// cStr::LastIndexOf : (const char *, const bool)
inline int cStr::LastIndexOf(const char *Str, const bool NoCase) const {
	return LastIndexOf(Str, m_Length - 1, m_Length, NoCase);
}

// cStr::LastIndexOf : (const char *, const int, const bool)
inline int cStr::LastIndexOf(const char *Str, const int StartIndex, const bool NoCase) const {
	return LastIndexOf(Str, StartIndex, StartIndex + 1, NoCase);
}

// cStr::LastIndexOf : (const char *, const int, const int, const bool)
inline int cStr::LastIndexOf(const char *Str, const int StartIndex, const int Count, const bool NoCase) const {
	cAssert(IsValid());
	cAssert(Count >= 0);
	if (m_Str == NULL)return - 1;

	int L, i0, i;
	
	L = Length(Str);

	if(m_Length == 0) {
		cAssert(StartIndex >= -1);
		cAssert(StartIndex <= 0);
		
		if(L > 0) {
			return -1;
		}
		return 0;
	}
	
	cAssert(StartIndex >= 0);
	cAssert(StartIndex < m_Length);
	cAssert(Count <= StartIndex + 1);
	
	i0 = StartIndex - Count + 1;
	for(i = StartIndex; i >= i0; i--) {
		if(NoCase ? EqualsNoCase(&m_Str[i], Str, L) : Equals(&m_Str[i], Str, L)) {
			return i;
		}
	}

	return -1;
} // cStr::LastIndexOf : (const char *, ...)

//-----------------------------------------------------------------------------
// cStr::LastIndexOfAny
//-----------------------------------------------------------------------------

// cStr::LastIndexOfAny : (const char *)
inline int cStr::LastIndexOfAny(const char *Chars) const {
	return LastIndexOfAny(Chars, m_Length - 1, m_Length);
}

// cStr::LastIndexOfAny : (const char *, const int)
inline int cStr::LastIndexOfAny(const char *Chars, const int StartIndex) const {
	return LastIndexOfAny(Chars, StartIndex, StartIndex + 1);
}

// cStr::LastIndexOfAny : (const char *, const int, const int)
inline int cStr::LastIndexOfAny(const char *Chars, const int StartIndex, const int Count) const {
	cAssert(IsValid());
	cAssert(Count >= 0);
	if (m_Str == NULL)return -1;
	if(m_Length == 0) {
		cAssert(StartIndex >= -1);
		cAssert(StartIndex <= 0);
		
		return -1;
	}

	cAssert(StartIndex >= 0);
	cAssert(StartIndex < m_Length);
	cAssert(Count <= StartIndex + 1);

	int i0, i;
	char c;
	cStr t(Chars);

	i0 = StartIndex - Count + 1;
	for(i = StartIndex; i >= i0; i--) {
		c = m_Str[i];
		if(t.Contains(c)) {
			return i;
		}
	}

	return -1;
} // cStr::LastIndexOfAny


//-----------------------------------------------------------------------------
// ToLower / ToUpper, MakeLower / MakeUpper
//-----------------------------------------------------------------------------

// cStr::ToLower : (const char)
inline char cStr::ToLower(const char c) {
	if(c >= 'A' && c <= 'Z') {
		return c + ('a' - 'A');
	}
	return c;
}

// cStr::ToUpper : (const char)
inline char cStr::ToUpper(const char c) {
	if(c >= 'a' && c <= 'z') {
		return c - ('a' - 'A');
	}
	return c;
}

// cStr::MakeLower
inline void cStr::MakeLower() {
	cAssert(IsValid());
	if (m_Str == NULL)return;
	int i;

	for(i = 0; m_Str[i] != '\0'; i++) {
		m_Str[i] = ToLower(m_Str[i]);
	}
}

// cStr::MakeUpper
inline void cStr::MakeUpper() {
	cAssert(IsValid());
	if (m_Str == NULL)return;
	int i;

	for(i = 0; m_Str[i] != '\0'; i++) {
		m_Str[i] = ToUpper(m_Str[i]);
	}
}

// cStr::ToLower : const cStr () const
inline const cStr cStr::ToLower() const {
	cStr S = *this;
	S.MakeLower();
	return S;
}

// cStr::ToUpper : const cStr () const
inline const cStr cStr::ToUpper() const {
	cStr S = *this;
	S.MakeUpper();
	return S;
}

//-----------------------------------------------------------------------------
// cStr::Join
//-----------------------------------------------------------------------------

// cStr::Join : (const char *, const cList<cStr> &)
inline const cStr cStr::Join(const char *Separator, const cList<cStr> &Strings) {
	return Join(Separator, Strings, 0, Strings.Count());
}

//-----------------------------------------------------------------------------
// cStr::Split
//-----------------------------------------------------------------------------

// cStr::Split
inline void cStr::Split(cList<cStr> *List, const char *Delimiters) const {
	cAssert(IsValid());
	cStr::Split(m_Str, List, Delimiters);
}

//-----------------------------------------------------------------------------
// cStr::CharIs...
//-----------------------------------------------------------------------------

// cStr::CharIsLower
inline bool cStr::CharIsLower(const int c) {
	return c >= 'a' && c <= 'z';
}

// cStr::CharIsUpper
inline bool cStr::CharIsUpper(const int c) {
	return c >= 'A' && c <= 'Z';
}

// cStr::CharIsAlpha
inline bool cStr::CharIsAlpha(const int c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// cStr::CharIsNumeric
inline bool cStr::CharIsNumeric(const int c) {
	return c >= '0' && c <= '9';
}

// cStr::CharIsHexadecimal
inline bool cStr::CharIsHexadecimal(const int c) {
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

// cStr::CharIsNewLine
inline bool cStr::CharIsNewLine(const int c) {
	return c == '\n' || c == '\r' || c == '\v';
}

// cStr::CharIsTab
inline bool cStr::CharIsTab(const int c) {
	return c == '\t';
}

// cStr::CharIsWhitespace
inline bool cStr::CharIsWhitespace(const int c) {
	return ' ' == c || '\t' == c;
}

// cStr::CharIsDecimalPoint
inline bool cStr::CharIsDecimalPoint(const int c) {
	return '.' == c || ',' == c;
}

// cStr::CharIsSign
inline bool cStr::CharIsSign(const int c) {
	return '+' == c || '-' == c;
}

// cStr::CharIsExponent
inline bool cStr::CharIsExponent(const int c) {
	return 'd' == c || 'D' == c || 'e' == c || 'E' == c;
}

//-----------------------------------------------------------------------------
// cStr::GetHashCode
//-----------------------------------------------------------------------------
inline int cStr::GetHashCode(const char *Str, const bool NoCase) {
	cAssert(Str != NULL);
	
	int H = 0, i;
	
	for(i = 0; *Str != '\0'; i++) {
		H += (NoCase ? ToLower(*Str++) : (*Str++)) * (i + 119);
	}
	return H;
} // cStr::GetHashCode
