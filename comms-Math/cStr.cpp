#include "comms-Math.h"

namespace comms {

const cStr cStr::Empty("");

template<class Type>
static Type Sign(const Type x) {
	return x > (Type)0 ? (Type)1 : (x < (Type)0 ? -(Type)1 : (Type)0);
}

//-----------------------------------------------------------------------------
// cStr simple pool implementation, static variables and mutex-es
//-----------------------------------------------------------------------------

cList<char*> cStr::pool;
cList<char*> cStr::free_strings;
char cStr::def_str = 0;

//allocation short strings from pool

static bool COMMS_CSTR_MUTEX_CREATED = false;

//-----------------------------------------------------------------------------
// cStr::getstr()
//-----------------------------------------------------------------------------

char* cStr::getstr(){
	if (!COMMS_CSTR_MUTEX_CREATED) {
		COMMS_CSTR_MUTEX_CREATED = true;
	}
	if (free_strings.Count() == 0){
		int n = 256;
		char* s = new char[n*Param::BaseBufferLength];
		for (int i = 0; i < n; i++){
			char* s1 = s + i*Param::BaseBufferLength;
			s1[0] = 31;
			free_strings.Add(s1);
		}
		pool.Add(s);
	}
	char* l = free_strings.GetLast();
	cAssert(l[0] == 31);//check double usage of same pool chunk
	free_strings.RemoveLast();
	return l;
}

//-----------------------------------------------------------------------------
// cStr::freestr(char* s)
//-----------------------------------------------------------------------------

void cStr::freestr(char* s){
	if (!COMMS_CSTR_MUTEX_CREATED) {
		COMMS_CSTR_MUTEX_CREATED = true;
	}
	s[0] = 31;
	free_strings.Add(s);
}

//-----------------------------------------------------------------------------
// cStr::Resize
//-----------------------------------------------------------------------------


void cStr::Resize(const int MinCapacity, const bool KeepOld) {
	cAssert(IsValid());
	cAssert(MinCapacity > 0);

	if (m_Str == NULL && MinCapacity < Param::BaseBufferLength){
		m_Str = getstr();
		m_Capacity = Param::BaseBufferLength;
		m_Str[0] = 0;
		return;
	}

	int Mod, Capacity;
	char *S = NULL;
	
	Mod = MinCapacity % Param::GrowBy;
	if(0 == Mod) {
		Capacity = MinCapacity;
	} else {
		Capacity = MinCapacity + Param::GrowBy - Mod; // Aligning capacity with "GrowBy" granularity
	}
	int OldCapacity = m_Capacity;
	m_Capacity = Capacity;

	S = (char *)cPool::Alloc(m_Capacity);
	
	if(KeepOld) {
		if (m_Str){
			memcpy(S, m_Str, m_Length + 1);
		}
		else S[0] = 0;
	}

	if(OldCapacity>Param::BaseBufferLength) {
		cPool::Free((byte *)m_Str, OldCapacity);
	}
	else if (m_Str)freestr(m_Str);

	m_Str = S;
} // cStr::Resize

//-----------------------------------------------------------------------------
// cStr::Copy : void (const char *)
//-----------------------------------------------------------------------------
void cStr::Copy(const char *Src, const int StartIndex, const int Count) {
	cAssert(IsValid());
	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= Length(Src));
	
	if(NULL == Src) {
		Clear();
		return;
	}
	
	int i1, i, d;
	
	i1 = StartIndex + Count - 1;
	if(Src >= m_Str && Src <= m_Str + m_Length) {
		d = int(Src + StartIndex - m_Str);
		cAssert(d + Count <= m_Length);
		for(i = StartIndex; i <= i1; i++) {
			m_Str[i - StartIndex] = Src[i];
		}
		m_Str[i] = '\0';
		m_Length -= m_Length - Count;
		cAssert(IsValid());
		return;
	}

	EnsureCapacity(Count + 1, false);
	memcpy(m_Str, Src + StartIndex, Count);
	m_Str[Count] = '\0';
	m_Length = Count;
	cAssert(IsValid());
} // cStr::Copy

//-----------------------------------------------------------------------------
// cStr::Compare : int (const char *, const char *, const int)
//-----------------------------------------------------------------------------
int cStr::Compare(const char *l, const char *r, const int MaxLength) {
	if (l == NULL && r == NULL) return true;
	if ((l == NULL) != (r == NULL))return false;
	cAssert(MaxLength >= -1);
	int cl, cr, d, i = MaxLength;
	do {
		cl = *l++;
		cr = *r++;
		if(MaxLength != -1) {
			if(0 == i--) {
				return 0;
			}
		}
		d = cl - cr;
		if(d != 0) {
			return Sign(d);
		}
	} while(cl != 0);
	return 0;
} // cStr::Compare

//-----------------------------------------------------------------------------
// cStr::CompareNoCase
//-----------------------------------------------------------------------------
int cStr::CompareNoCase(const char *l, const char *r, const int MaxLength) {
	cAssert(l != NULL);
	cAssert(r != NULL);
	cAssert(MaxLength >= -1);
	int cl, cr, d, i = MaxLength;
	do {
		cl = *l++;
		cr = *r++;
		if(MaxLength != -1) {
			if(0 == i--) {
				return 0;
			}
		}
		d = cl - cr;
		while(d != 0) {
			if(cl >= 'A' && cl <= 'Z') {
				d += 'a' - 'A';
				if(0 == d) {
					break;
				}
			}
			if(cr >= 'A' && cr <= 'Z') {
				d -= 'a' - 'A';
				if(0 == d) {
					break;
				}
			}
			return Sign(d);
		}
	} while(cl != 0);
	return 0;
} // cStr::CompareNoCase

//-----------------------------------------------------------------------------
// cStr::ComparePath : int (const char *, const char *, const int)
//-----------------------------------------------------------------------------
int cStr::ComparePath(const char *l, const char *r, const int MaxLength) {
	cAssert(l != NULL);
	cAssert(r != NULL);
	cAssert(MaxLength >= -1);
	int cl, cr, d, i = MaxLength, c;
	do {
		cl = *l++;
		cr = *r++;
		if(MaxLength != -1) {
			if(0 == i--) {
				c = 0;
				// Consider the case:
				// l = /home/sergyi/3D-CoatV4-BETA/Shaders/Custom/ver.txt
				// r = /home/sergyi/3D-CoatV4
				if(cl != 0) {
					cAssert(0 == cr);
					if(cl != '/' && cl != '\\') {
						c = 1;
					}
				}
				if(cr != 0) {
					cAssert(0 == cl);
					if(cr != '/' && cr != '\\') {
						c = -1;
					}
				}
				return c;
			}
		}
		d = cl - cr;
		while(d != 0) {
			if(cl >= 'A' && cl <= 'Z') {
				d += 'a' - 'A';
				if(0 == d) {
					break;
				}
			}
			if('/' == cl) {
				d += '\\' - '/';
				if(0 == d) {
					break;
				}
			}
			if(cr >= 'A' && cr <= 'Z') {
				d -= 'a' - 'A';
				if(0 == d) {
					break;
				}
			}
			if('/' == cr) {
				d -= '\\' - '/';
				if(0 == d) {
					break;
				}
			}
			while(cl != 0) {
				if('/' == cl || '\\' == cr) {
					break;
				}
				cl = *l++;
			}
			while(cr != 0) {
				if('/' == cr || '\\' == cr) {
					break;
				}
				cr = *r++;
			}
			// Compare folder depth
			if(cl != 0 && 0 == cr) {
				return 1;
			} else if(0 == cl && cr != 0) {
				return -1;
			}
			// Same folder depth
			return Sign(d);
		}
	} while(cl != 0);
	return 0;
} // cStr::ComparePath

//---------------------------------------------------------------------------------------------
// cStr::ToString : (const int *, ...)
//---------------------------------------------------------------------------------------------
const cStr cStr::ToString(const int *IntArray, const int Count, const char *Separator) {
	cAssert(Count >= 0);
	
	int i;
	cList<cStr> L;

	for(i = 0; i < Count; i++) {
		L.Add(ToString(IntArray[i]));
	}
	return Join(Separator, L);
} // cStr::ToString : (const int *, ...)

//----------------------------------------------------------------------------------------------------------
// cStr::ToString : (const float *, ...)
//----------------------------------------------------------------------------------------------------------
const cStr cStr::ToString(const float *FloatArray, const int Count, const int Prec, const char *Separator) {
	cAssert(Count >= 0);
	cAssert(Prec >= 0);
	
	int i;
	cList<cStr> L;
	
	for(i = 0; i < Count; i++) {
		L.Add(ToString(FloatArray[i], Prec));
	}
	return Join(Separator, L);
} // cStr::ToString : (const float *, ...)

//------------------------------------------------------------------------------------------------------------
// cStr::ToString : (const double *, ...)
//------------------------------------------------------------------------------------------------------------
const cStr cStr::ToString(const double *DoubleArray, const int Count, const int Prec, const char *Separator) {
	cAssert(Count >= 0);
	cAssert(Prec >= 0);

	int i;
	cList<cStr> L;
	
	for(i = 0; i < Count; i++) {
		L.Add(ToString(DoubleArray[i], Prec));
	}
	return Join(Separator, L);
} // cStr::ToString : (const double *, ...)

//--------------------------------------------------------------------------------
// cStr::Append : (const cStr &, ...)
//--------------------------------------------------------------------------------
void cStr::Append(const cStr &Src, const int StartIndex, const int Count) {
	cAssert(IsValid());
	cAssert(Src.IsValid());
	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= Src.m_Length);

	int L, i;

	L = m_Length + Count;
	EnsureCapacity(L + 1);
	for(i = 0; i < Count; i++) {
		m_Str[m_Length + i] = Src.m_Str[StartIndex + i];
	}
	m_Length = L;
	m_Str[m_Length] = '\0';
} // cStr::Append : (const cStr &, ...)

//--------------------------------------------------------------------------------
// cStr::Append : (const char *, ...)
//--------------------------------------------------------------------------------
void cStr::Append(const char *Src, const int StartIndex, const int Count) {
	cAssert(IsValid());
	
	if(NULL == Src) {
		cAssert(StartIndex == 0);
		cAssert(Count == 0);
		return;
	}

	int L, i;

	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= Length(Src));

	L = m_Length + Count;
	EnsureCapacity(L + 1);
	for(i = 0; i < Count; i++) {
		m_Str[m_Length + i] = Src[StartIndex + i];
	}
	m_Length = L;
	m_Str[m_Length] = '\0';
} // cStr::Append : (const char *, ...)

//-----------------------------------------------------------------------------
// cStr::Append : (const char, ...)
//-----------------------------------------------------------------------------
void cStr::Append(const char c, const int Count) {
	cAssert(IsValid());
	cAssert(Count >= 0);

	int L, i;

	L = m_Length + Count;
	EnsureCapacity(L + 1);
	for(i = 0; i < Count; i++) {
		m_Str[m_Length + i] = c;
	}
	m_Length = L;
	m_Str[m_Length] = '\0';
} // cStr::Append : (const char, ...)

//-----------------------------------------------------------------------------
// cStr::AppendPath
//-----------------------------------------------------------------------------
void cStr::AppendPath(const char *Path) {
	cAssert(IsValid());

	int Pos, i = 0;
	
	if(Path != NULL && Path[0] != '\0') {
		EnsureCapacity(m_Length + Length(Path) + 2);
		Pos = m_Length;
		if(Pos > 0) {
			if(m_Str[Pos - 1] != '/' && m_Str[Pos - 1] != '\\') {
				m_Str[Pos++] = '\\';
			}
		}
		if(Path[i] == '/' || Path[i] == '\\') {
			i++;
		}
		for(; Path[i] != '\0'; i++) {
			if(Path[i] == '/') {
				m_Str[Pos++] = '\\';
			} else {
				m_Str[Pos++] = Path[i];
			}
		}
		m_Length = Pos;
		m_Str[Pos] = '\0';
	}
} // cStr::AppendPath

// cStr::AppendFormat : (const char *, ...)
void cStr::AppendFormat(const char *Format, ...) {
	va_list Args;
	va_start(Args, Format);
	Append(Formatv(Format, Args));
	va_end(Args);
}

//------------------------------------------------------------------------------------------
// cStr::Insert : (..., const cStr &, ...)
//------------------------------------------------------------------------------------------
void cStr::Insert(const int Index, const cStr &Src, const int StartIndex, const int Count) {
	cAssert(IsValid());
	cAssert(Src.IsValid());
	cAssert(Index >= 0);
	cAssert(Index <= m_Length);
	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= Src.m_Length);

	int L, i;

	L = m_Length + Count;
	EnsureCapacity(L + 1);
	for(i = m_Length; i >= Index; i--) {
		m_Str[i + Count] = m_Str[i];
	}
	for(i = 0; i < Count; i++) {
		m_Str[Index + i] = Src.m_Str[StartIndex + i];
	}
	m_Length = L;
} // cStr::Insert : (..., const cStr &, ...)

//------------------------------------------------------------------------------------------
// cStr::Insert : (..., const char *, ...)
//------------------------------------------------------------------------------------------
void cStr::Insert(const int Index, const char *Src, const int StartIndex, const int Count) {
	cAssert(IsValid());
	cAssert(Index >= 0);
	cAssert(Index <= m_Length);
	
	if(NULL == Src) {
		cAssert(StartIndex == 0);
		cAssert(Count == 0);
		return;
	}
	
	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= Length(Src));
	
	int L, i;

	L = m_Length + Count;
	EnsureCapacity(L + 1);
	for(i = m_Length; i >= Index; i--) {
		m_Str[i + Count] = m_Str[i];
	}
	for(i = 0; i < Count; i++) {
		m_Str[Index + i] = Src[StartIndex + i];
	}
	m_Length = L;
} // cStr::Insert : (..., const char *, ...)

//-----------------------------------------------------------------------------
// cStr::Insert : (..., const char, ...)
//-----------------------------------------------------------------------------
void cStr::Insert(const int Index, const char c, const int Count) {
	cAssert(IsValid());
	cAssert(Index >= 0);
	cAssert(Index <= m_Length);
	cAssert(Count >= 0);

	int L, i;

	L = m_Length + Count;
	EnsureCapacity(L + 1);
	for(i = m_Length; i >= Index; i--) {
		m_Str[i + Count] = m_Str[i];
	}
	for(i = 0; i < Count; i++) {
		m_Str[Index + i] = c;
	}
	m_Length = L;
} // cStr::Insert : (..., const char, ...)

//---------------------------------------------------------------------------------------------------
// cStr::Replace : (const char, const char, ...)
//---------------------------------------------------------------------------------------------------
void cStr::Replace(const char WhatChar, const char WithChar, const int StartIndex, const int Count) {
	cAssert(IsValid());
	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= m_Length);
	
	int i1, i;
	
	i1 = StartIndex + Count - 1;
	for(i = StartIndex; i <= i1; i++) {
		if(WhatChar == m_Str[i]) {
			m_Str[i] = WithChar;
		}
	}
} // cStr::Replace : (const char, const char, ...)


//---------------------------------------------------------------------------------------------------------------------------
// cStr::Replace : (const char *, const char *, ...)
//---------------------------------------------------------------------------------------------------------------------------
int cStr::Replace(const char *WhatString, const char *WithString, const int StartIndex, const int Count, const bool NoCase) {
	cAssert(IsValid());
	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= m_Length);
	
	int i1, ll, rl, R = 0, i, Replaced = 0, j;
	cStr S = m_Str;
	
	i1 = StartIndex + Count - 1;
	ll = Length(WhatString);
	rl = Length(WithString);
	
	cAssert(ll > 0);
	
	// How big this string will be after replacements?
	for(i = StartIndex; i <= i1; i++) {
		if(NoCase ? EqualsNoCase(&m_Str[i], WhatString, ll) : Equals(&m_Str[i], WhatString, ll)) {
			R++;
			i += ll - 1;
		}
	}
	
	if(Count > 0) {
		EnsureCapacity(m_Length + (rl - ll) * R + 1, false);
		// Leave prefix w/o change
		for(i = StartIndex, j = StartIndex; i <= i1; i++) {
			if(NoCase ? EqualsNoCase(&S[i], WhatString, ll) : Equals(&S[i], WhatString, ll)) {
				memcpy(m_Str + j, WithString, rl);
				i += ll - 1;
				j += rl;
				Replaced++;
			} else {
				m_Str[j] = S[i];
				j++;
			}
		}
		memcpy(m_Str + j, &S[i1 + 1], S.m_Length - (i1 + 1)); // Append suffix w/o change
		m_Str[j] = '\0';
		m_Length += (rl - ll) * R;
		cAssert(m_Length == Length(m_Str));
	}
	return Replaced;
} // cStr::Replace : (const char *, const char *, ...)

//---------------------------------------------------------------------------------------------------------------------------
// cStr::ReplaceFirst : (const char *, const char *, ...)
//---------------------------------------------------------------------------------------------------------------------------
int cStr::ReplaceFirst(const char *WhatString, const char *WithString, const int StartIndex, const int Count, const bool NoCase) {
	cAssert(IsValid());
	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= m_Length);

	int i1, ll, rl, R = 0, i, Replaced = 0, j;
	cStr S = m_Str;

	i1 = StartIndex + Count - 1;
	ll = Length(WhatString);
	rl = Length(WithString);

	cAssert(ll > 0);

	// How big this string will be after replacements?
	for(i = StartIndex; i <= i1; i++) {
		if(NoCase ? EqualsNoCase(&m_Str[i], WhatString, ll) : Equals(&m_Str[i], WhatString, ll)) {
			R++;
			i += ll - 1;
			break;
		}
	}

	if(Count > 0) {
		EnsureCapacity(m_Length + (rl - ll) * R + 1, false);
		// Leave prefix w/o change
		for(i = StartIndex, j = StartIndex; i <= i1; i++) {
			if(Replaced==0 && (NoCase ? EqualsNoCase(&S[i], WhatString, ll) : Equals(&S[i], WhatString, ll))) {
				memcpy(m_Str + j, WithString, rl);
				i += ll - 1;
				j += rl;
				Replaced++;
			} else {
				m_Str[j] = S[i];
				j++;
			}
		}
		memcpy(m_Str + j, &S[i1 + 1], S.m_Length - (i1 + 1)); // Append suffix w/o change
		m_Str[j] = '\0';
		m_Length += (rl - ll) * R;
		cAssert(m_Length == Length(m_Str));
	}
	return Replaced;
} // cStr::ReplaceFirst : (const char *, const char *, ...)


//----------------------------------------------------------------------------------------------------
// cStr::ReplaceAny : (const char *, const char, ...)
//----------------------------------------------------------------------------------------------------
void cStr::ReplaceAny(const char *Chars, const char WithChar, const int StartIndex, const int Count) {
	cAssert(IsValid());
	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= m_Length);
	
	int i1, i;
	
	i1 = StartIndex + Count - 1;
	while(*Chars) {
		for(i = StartIndex; i <= i1; i++) {
			if(*Chars == m_Str[i]) {
				m_Str[i] = WithChar;
			}
		}
		Chars++;
	}
} // cStr::ReplaceAny : (const char *, const char, ...)

//---------------------------------------------------------------------------------------------------------------
// cStr::Join : (const char *, const cList<cStr> &, ...)
//---------------------------------------------------------------------------------------------------------------
const cStr cStr::Join(const char *Separator, const cList<cStr> &Strings, const int StartIndex, const int Count) {
	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= Strings.Count());
	
	int i1, i;
	cStr S;

	i1 = StartIndex + Count - 1;
	for(i = StartIndex; i <= i1; i++) {
		S.Append(Strings[i]);
		if(i < i1) {
			S.Append(Separator);
		}
	}
	return S;
} // cStr::Join : (const char *, const cList<cStr> &, ...)

//-----------------------------------------------------------------------------
// cStr::Split : (const char *, ...)
//-----------------------------------------------------------------------------
void cStr::Split(const char *Src, cList<cStr> *List, const char *Delimiters) {
	cStr S;
	cList<char *> Words;
	char *c = NULL;
	int i;

	List->Clear();

	S = Src;
	if(S.IsEmpty()) { // Nothing to split.
		return;
	}

	if(Length(Delimiters) < 1) { // No delimiters. Add whole string.
		List->Add(S);
		return;
	}
	
	S.ReplaceAny(Delimiters, Delimiters[0]);

	c = S.ToCharPtr();
	while(*c != '\0') {
		// Jump over leading delimiters:
		while(Delimiters[0] == *c) {
			c++;
		}
		// Break if we reach the end:
		if('\0' == *c) {
			break;
		}
		// Save pointer to the beginning of word:
		Words.Add(c);
		// Jump over non delimiters:
		while(*c != '\0' && *c != Delimiters[0]) {
			c++;
		}
		// Break if we reach the end:
		if('\0' == *c) {
			break;
		}
		// Place a null char here to mark the end of the word:
		*c++ = '\0';
	}

	for(i = 0; i < Words.Count(); i++) {
		List->Add(Words[i]);
	}
} // cStr::Split : (const char *, ...)

//-----------------------------------------------------------------------------
// cStr::Formatv
//-----------------------------------------------------------------------------
const cStr cStr::Formatv(const char *Format, va_list Args) {
	char Buffer[20000];
    static const size_t B = sizeof( Buffer ) / sizeof( char );
    
#if defined COMMS_WINDOWS
	const int rc = vsnprintf_s(Buffer, B - 1, _TRUNCATE, Format, Args);
#endif // COMMS_WINDOWS

#if defined COMMS_MACOS || defined COMMS_LINUX || defined COMMS_IOS || defined COMMS_TIZEN
    const int rc = vsnprintf(Buffer, B - 1, Format, Args);
#endif // COMMS_MACOS || COMMS_LINUX || COMMS_IOS || COMMS_TIZEN

    cAssert( (rc >= 0) && (rc < (int)B) );
	
	return cStr(Buffer);
} // cStr::Formatv

//-----------------------------------------------------------------------------
// cStr::Format
//-----------------------------------------------------------------------------
const cStr cStr::Format(const char *Format, ...) {
	char Buffer[10000];
    static const size_t B = sizeof( Buffer ) / sizeof( char );

	va_list Args;
	va_start(Args, Format);

#if defined COMMS_WINDOWS
    const int rc = vsnprintf_s( Buffer, B - 1, _TRUNCATE, Format, Args );
#endif // COMMS_WINDOWS

#if defined COMMS_MACOS || defined COMMS_LINUX || defined COMMS_IOS || defined COMMS_TIZEN
    const int rc = vsnprintf(Buffer, B - 1, Format, Args);
#endif // COMMS_MACOS || COMMS_LINUX || COMMS_IOS || COMMS_TIZEN

    cAssert( (rc >= 0) && (rc < (int)B) );

	va_end(Args);
	return cStr(Buffer);
} // cStr::Format

//-----------------------------------------------------------------------------
// cStr::ToInt
//-----------------------------------------------------------------------------
bool cStr::ToInt(const char *Str, int *Value) {
	// [Whitespace]						Consists of space or tab characters, which are ignored.
	// [Sign]							Is either plus (+) or minus (-).
	// Digits							Are one or more decimal digits.

	cAssert(Value != NULL);
	*Value = 0;
	bool Negative = false;
	int Number = 0, DigitsCount = 0;

	// [Whitespace]
	while(CharIsWhitespace(*Str)) {
		Str++;
	}
	// [Sign]
	switch(*Str) {
		case '-': Negative = true;
		case '+': Str++;
	}
	// Digits
	while(CharIsNumeric(*Str)) {
		Number = Number * 10 + (*Str - '0');
		Str++;
		DigitsCount++;
	}
	if(0 == DigitsCount) {
		return false;
	}
	// Apply sign
	if(Negative) {
		Number = -Number;
	}
	*Value = Number;
	return true;
} // cStr::ToInt

//-----------------------------------------------------------------------------
// cStr::ToIntArray
//-----------------------------------------------------------------------------
bool cStr::ToIntArray(const char *Str, cList<int> *IntArray) {
	cAssert(IntArray != NULL);
	IntArray->Clear();

	cList<cStr> Tokens;
	int i, Value;

	cStr::Split(Str, &Tokens);
	for(i = 0; i < Tokens.Count(); i++) {
		if(!ToInt(Tokens[i], &Value)) {
			return false;
		}
		IntArray->Add(Value);
	}
	return true;
} // cStr::ToIntArray

//-----------------------------------------------------------------------------
// cStr::GetFileExtension
//-----------------------------------------------------------------------------
const cStr cStr::GetFileExtension() const {
	cAssert(IsValid());

	int i0, i;
	
	i0 = m_Length - 1;
	for(i = i0; i >= 0; i--) {
		if(m_Str[i] == '/' || m_Str[i] == '\\') {
			return cStr::Empty;
		}
		if(m_Str[i] == '.') {
			return Substring(i + 1);
		}
	}
	return cStr::Empty;
} // cStr::GetFileExtension

//-----------------------------------------------------------------------------
// cStr::GetFileName
//-----------------------------------------------------------------------------
const cStr cStr::GetFileName() const {
	cAssert(IsValid());
	if(IsEmpty()) {
		return Empty;
	}

	int Pos;

	Pos = m_Length - 1;
	while(Pos > 0 && m_Str[Pos - 1] != '/' && m_Str[Pos - 1] != '\\') {
		Pos--;
	}
	return Substring(Pos);
} // cStr::GetFileName

//-----------------------------------------------------------------------------
// cStr::GetFileBase
//-----------------------------------------------------------------------------
const cStr cStr::GetFileBase() const {
	cAssert(IsValid());
	if(IsEmpty()) {
		return Empty;
	}
	
	int Pos, Start;
	
	Pos = m_Length - 1;
	while(Pos > 0 && m_Str[Pos - 1] != '/' && m_Str[Pos - 1] != '\\') {
		Pos--;
	}
	Start = Pos;
	while(Pos < m_Length && m_Str[Pos] != '.') {
		Pos++;
	}
	return Substring(Start, Pos - Start);
} // cStr::GetFileBase

//-----------------------------------------------------------------------------
// cStr::GetFilePath
//-----------------------------------------------------------------------------
const cStr cStr::GetFilePath() const {
	cAssert(IsValid());
	if(IsEmpty()) {
		return Empty;
	}

	int Pos;

	Pos = m_Length;
	while(Pos > 0 && m_Str[Pos - 1] != '/' && m_Str[Pos - 1] != '\\') {
		Pos--;
	}
	return Substring(0, Pos);
} // cStr::GetFilePath

//-----------------------------------------------------------------------------
// cStr::RemoveFileExtension
//-----------------------------------------------------------------------------
void cStr::RemoveFileExtension() {
	cAssert(IsValid());

	int i;
	
	for(i = m_Length - 1; i >= 0; i--) {
		if('/' == m_Str[i] || '\\' == m_Str[i]) {
			return;
		}
		if('.' == m_Str[i]) {
			Remove(i);
			break;
		}
	}
} // cStr::RemoveFileExtension

//-----------------------------------------------------------------------------
// cStr::RemoveFileName
//-----------------------------------------------------------------------------
void cStr::RemoveFileName() {
	cAssert(IsValid());

	int Pos;
	
	Pos = m_Length - 1;
	while(Pos > 0 && m_Str[Pos] != '/' && m_Str[Pos] != '\\') {
		Pos--;
	}
	if(Pos < 0) {
		Pos = 0;
	}
	Remove(Pos);
} // cStr::RemoveFileName

//-----------------------------------------------------------------------------
// cStr::RemoveFilePath
//-----------------------------------------------------------------------------
void cStr::RemoveFilePath() {
	cAssert(IsValid());

	int Pos;

	Pos = m_Length;
	while(Pos > 0 && m_Str[Pos - 1] != '/' && m_Str[Pos - 1] != '\\') {
		Pos--;
	}
	*this = Substring(Pos);
} // cStr::RemoveFilePath

//-----------------------------------------------------------------------------
// cStr::RemoveFileAbsPath
//-----------------------------------------------------------------------------
void cStr::RemoveFileAbsPath(const char *AbsPath) {
	// Consider case, when this absolute file pathname lies completely within the specified "AbsPath"
	if(IndexOf(AbsPath, true) == 0) {
		Remove(0, Length(AbsPath));
		if(!IsEmpty() && ('\\' == m_Str[0] || '/' == m_Str[0])) {
			Remove(0, 1);
		}
		return;
	}
	// Case of partial match
	cList<cStr> This;
	cList<cStr> Rel;
	Split(&This, "\\/");
	Split(AbsPath, &Rel, "\\/");
	if(This.IsEmpty() || Rel.IsEmpty()) {
		return;
	}
	if(!cStr::EqualsNoCase(This[0], Rel[0])) { // Even root folder does not match
		return;
	}

	This.RemoveAt(0);
	Rel.RemoveAt(0);

	// Remove all match
	int C = min(This.Count(), Rel.Count());
	while(C > 0) {
		if(cStr::EqualsNoCase(This[0], Rel[0])) {
			This.RemoveAt(0);
			Rel.RemoveAt(0);
		} else {
			break;
		}
		C--;
	}

	// Add ".."
	cStr Pn;
	int i;

	for(i = 0; i < Rel.Count(); i++) {
		Pn.AppendPath("..");
	}

	// Add file pathname
	for(i = 0; i < This.Count(); i++) {
		Pn.AppendPath(This[i]);
	}
	Copy(Pn);
} // cStr::RemoveFileAbsPath

//-----------------------------------------------------------------------------
// cStr::SetFileExtension
//-----------------------------------------------------------------------------
void cStr::SetFileExtension(const char *Extension) {
	cAssert(IsValid());
	cAssert(Extension != NULL);
	
	RemoveFileExtension();
	if(*Extension != '.') {
		Append('.');
	}
	Append(Extension);
} // cStr::SetFileExtension

//-----------------------------------------------------------------------------
// cStr::SetFileDefaultExtension
//-----------------------------------------------------------------------------
void cStr::SetFileDefaultExtension(const char *DefaultExtension) {
	cAssert(IsValid());
	cAssert(DefaultExtension != NULL);

	int i0, i;
	
	i0 = m_Length - 1;
	for(i = i0; i >= 0; i--) {
		if(m_Str[i] == '/' || m_Str[i] == '\\') {
			break;
		}
		if(m_Str[i] == '.') {
			return; // String already has an extension.
		}
	}
	if(*DefaultExtension != '.') {
		Append('.');
	}
	Append(DefaultExtension);
} // cStr::SetDefaultFileExtension

//-----------------------------------------------------------------------------
// cStr::SetFilePath
//-----------------------------------------------------------------------------
void cStr::SetFilePath(const char *Path) {
	cAssert(IsValid());

	cStr Fn;
	
	Fn = GetFileName();
	*this = Path;
	AppendPath(Fn);
} // cStr::SetFilePath

//-----------------------------------------------------------------------------
// cStr::SetDefaultFilePath
//-----------------------------------------------------------------------------
void cStr::SetFileDefaultPath(const char *DefaultPath) {
	cAssert(IsValid());

	cStr Fn;

	if(GetFilePath().IsEmpty()) {
		Fn = *this;
		*this = DefaultPath;
		AppendPath(Fn);
	}
} // cStr::SetFileDefaultPath

//-----------------------------------------------------------------------------
// cStr::EnsureTrailingBackslash
//-----------------------------------------------------------------------------
void cStr::EnsureTrailingBackslash() {
	cAssert(IsValid());
	
	char c;
	
	if(m_Length > 0) {
		c = m_Str[m_Length - 1];
		if(c != '\\' && c != '/') {
			Append('\\');
		}
	}
} // cStr::EnsureTrailingBackslash

//-----------------------------------------------------------------------------
// cStr::SlashesToBackSlashes
//-----------------------------------------------------------------------------
void cStr::SlashesToBackSlashes() {
	cAssert(IsValid());

	int i;
	
	for(i = 0; i < m_Length; i++) {
		if('/' == m_Str[i]) {
			m_Str[i] = '\\';
		}
	}
} // cStr::SlashesToBackSlashes

//-----------------------------------------------------------------------------
// cStr::BackSlashesToSlashes
//-----------------------------------------------------------------------------
void cStr::BackSlashesToSlashes() {
	cAssert(IsValid());

	int i;
	
	for(i = 0; i < m_Length; i++) {
		if('\\' == m_Str[i]) {
			m_Str[i] = '/';
		}
	}
} // cStr::BackSlashesToSlashes

// cStr::Decode
void cStr::Decode(const CodePage::Enum _CodePage, cList<word> *UniChars) const {
	cAssert(IsValid());
	UniChars->Clear();
#ifdef COMMS_WINDOWS
	const UINT CP[] = { 1251 };
	int L = MultiByteToWideChar(CP[_CodePage], 0, m_Str, -1, nullptr, 0);
	if(L > 0) {
		UniChars->SetCount(L);
		MultiByteToWideChar(CP[_CodePage], 0, m_Str, -1, (LPWSTR)UniChars->ToPtr(), UniChars->Count());
		if(!UniChars->IsEmpty()) {
			UniChars->RemoveLast();
		}
	}
#endif // COMMS_WINDOWS
#ifdef COMMS_LINUX
	const char *CP[] = { "CP1251" };
	static iconv_t Conv[] = { nullptr };
	if(nullptr == Conv[_CodePage]) {
		Conv[_CodePage] = iconv_open("UTF-16LE", CP[_CodePage]);
	}
	if(Conv[_CodePage] != (iconv_t)-1) {
		UniChars->SetCount(10 * m_Length);
		size_t InBytesLeft = m_Length, OutBytesLeft = UniChars->Size();
		char *InPtr = (char *)m_Str;
		char *OutPtr = (char *)UniChars->ToPtr();
		size_t r = iconv(Conv[_CodePage], &InPtr, &InBytesLeft, &OutPtr, &OutBytesLeft);
		if((size_t)-1 == r) {
			UniChars->Clear();
		} else {
			int l = (UniChars->Size() - OutBytesLeft) / sizeof(word);
			UniChars->SetCount(l);
		}
	}
#endif // COMMS_LINUX
#ifdef COMMS_MACOS
    const CFStringEncoding Enc[] = { kCFStringEncodingWindowsCyrillic };
    CFStringRef r = CFStringCreateWithCString(nullptr, m_Str, Enc[_CodePage]);
    UniCharCount l = CFStringGetLength(r);
    if(l > 0) {
        UniChars->SetCount((int)l);
        CFStringGetCharacters(r, CFRangeMake(0, l), UniChars->ToPtr());
    }
    CFRelease(r);
#endif // COMMS_MACOS
}

static const byte UTF8_Table[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	10,3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3,11, 6, 6, 6, 5, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,

	0,  12, 24, 36, 60, 96, 84, 12, 12, 12, 48, 72, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	12,  0, 12, 12, 12, 12, 12,  0, 12,  0, 12, 12, 12, 24, 12, 12, 12, 12, 12, 24, 12, 24, 12, 12,
	12, 12, 12, 12, 12, 12, 12, 24, 12, 12, 12, 12, 12, 24, 12, 12, 12, 12, 12, 12, 12, 24, 12, 12,
	12, 12, 12, 12, 12, 12, 12, 36, 12, 36, 12, 12, 12, 36, 12, 12, 12, 12, 12, 36, 12, 36, 12, 12,
	12, 36, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12
};

// UTF8_Decode
static bool UTF8_Decode(dword *State, dword *CodePoint, const int Char) {
	dword Type = UTF8_Table[Char];
	*CodePoint = (*State != 0) ? (Char & 0x3fu) | (*CodePoint << 6) : (0xff >> Type) & (Char);
	*State = UTF8_Table[256 + *State + Type];
	return (0 == *State);
}

// cStr::DecodeUTF8
bool cStr::DecodeUTF8(cList<word> *UniChars) const {
	cAssert(IsValid());
	UniChars->Clear();
	dword State = 0, CodePoint = 0;
	int i, c = 0;
	for(i = 0; i < m_Length; i++) {
		c = m_Str[i];
		if(c < 0) { // Char is from second page (128 - 255)
			c += 256;
		}
		if(UTF8_Decode(&State, &CodePoint, c)) {
			if(CodePoint <= 0xffff) {
				UniChars->Add((word)CodePoint);
			} else {
				UniChars->Add((word)(0xd7c0 + (CodePoint >> 10)));
				UniChars->Add((word)(0xdc00 + (CodePoint & 0x3ff)));
			}
		}
	}
	return (0 == State);
}

// cStr::CalcUTF8Length
int cStr::CalcUTF8Length(const int StartIndex) const {
	int i, c, l = 0;
	dword State = 0, CodePoint;
	for(i = StartIndex; i < m_Length; i++) {
		c = m_Str[i];
		if(c < 0) { // Char is from second page (128 - 255)
			c += 256;
		}
		l++;
		if(UTF8_Decode(&State, &CodePoint, c)) {
			break;
		}
	}
	return l;
}

} // comms
