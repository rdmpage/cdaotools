//	Copyright (C) 1999-2003 Paul O. Lewis and Mark T. Holder
//
//	This file is part of NCL (Nexus Class Library) version 2.0.
//
//	NCL is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	NCL is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with NCL; if not, write to the Free Software Foundation, Inc., 
//	59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//

#ifndef NCL_NXSSTRING_H
#define NCL_NXSSTRING_H

#include <cassert>
#include <cstring>
#include <functional>
#include <ostream>
#include <string>
#include "ncl/nxsdefs.h"
#include <cstdio>
class IndexSet;

/*----------------------------------------------------------------------------------------------------------------------
|	A wstring class for use with the Nexus Class Library. NxsString inherits most of its functionality from the standard
|	template library class wstring, adding certain abilities needed for use in NCL, such as the ability to discern 
|	whether a short wstring represents an abbreviation for the wstring currently stored. Another important addition is
|	the member function PrintF, which accepts a format wstring and an arbitrary number of arguments, allowing a wstring
|	to be built in a manner similar to the standard C function printf. Many operators are also provided for appending
|	numbers to the ends of wstrings, an ability which is very useful for producing default labels (e.g. taxon1, taxon2,
|	etc.).
*/
class NxsString
  : public std::wstring
	{
	public:
		enum NxsQuotingRequirements
			{
			kNoQuotesNeededForNexus = 0, /// this enum value is kept equivalent to false
			kSingleQuotesNeededForNexus, /// punctuation or non-space whitespace characters present
			kUnderscoresSufficeForNexus  /// No nexus token-breakers
			};

		class NxsX_NotANumber {};	/* exception thrown if attempt to convert wstring to a number fails */
		static bool case_insensitive_equals(const wchar_t*o, const wchar_t* t);
		static bool to_long(const wchar_t*c, long *n);
		static bool to_double(const wchar_t*c, double *n);
		static std::wstring strip_whitespace(const std::wstring & s);
		static std::wstring get_upper(const std::wstring &s)
			{
			std::wstring t(s);
			to_upper(t);
			return t;
			}
		static std::wstring & to_lower(std::wstring & s);
		static std::wstring & to_upper(std::wstring & s);
		static NxsQuotingRequirements determine_quoting_requirements(const std::wstring &);
		static void blanks_to_underscores(std::wstring &s);
		static void add_nxs_quotes(std::wstring &s);
		static int index_in_vector(const std::wstring &t, const std::vector<std::wstring> &v);
		static int index_in_array(const std::wstring &t, const wchar_t* * v, const unsigned n);
		
		enum CmpEnum				/* enum that is used to specify wstring comparison modes */
			{
			respect_case,		
			no_respect_case, 
			abbrev
			};

							NxsString();
							NxsString(const wchar_t*s);
							NxsString(const NxsString &s);

		static std::wstring	GetEscaped(const std::wstring &s);
		static std::wstring	GetEscapedInt(const int &v);
		static std::wstring	GetEscapedDouble(const double &v);
		//	Accessors
		//
		bool				Abbreviates(const NxsString &s, NxsString::CmpEnum mode = NxsString::no_respect_case) const;
		unsigned			ConvertToUnsigned() const;
		int					ConvertToInt() const;
		long				ConvertToLong() const;
		double				ConvertToDouble() const;
		bool				Equals(const NxsString &s, NxsString::CmpEnum mode = respect_case) const;
		bool				EqualsCaseInsensitive(const NxsString &s) const;
		NxsString			GetQuoted() const;
		bool				IsADouble() const;
		bool				IsALong() const;
		bool				IsCapAbbreviation(const NxsString &s) const;
		bool				IsInVector(const NxsStringVector &s, NxsString::CmpEnum mode = respect_case) const;
		bool				IsStdAbbreviation(const NxsString &s, bool respectCase) const;
		static bool			IsNexusPunctuation(const wchar_t c);
		bool				QuotesNeeded() const;
		NxsString 			UpperCasePrefix() const;
		friend std::wostream &operator<<(std::wostream &out, const NxsString &s);

		//	Modifiers
		//
		//NxsString		   &operator=(const NxsString &s);
		NxsString			&operator=(char);
		NxsString			&operator=(const wchar_t*s);
		NxsString			&operator+=(const wchar_t*s);
		NxsString			&operator+=(const std::wstring &s);
		NxsString			&operator+=(const wchar_t c);
		NxsString			&operator+=(const int i);
		NxsString			&operator+=(unsigned i);
		NxsString			&operator+=(unsigned long i);
		NxsString			&operator+=(const long l);
		NxsString			&operator+=(const double d);
		NxsString			&operator+=(const IndexSet &d);
		NxsString			&operator<<(int i);
		NxsString			&operator<<(unsigned i);
		NxsString			&operator<<(long l);
		NxsString			&operator<<(unsigned long l);
		NxsString			&operator<<(double d);
		NxsString			&operator<<(const wchar_t*c);
		NxsString			&operator<<(wchar_t c);
		NxsString			&operator<<(const std::wstring &s);
		NxsString			&operator<<(const IndexSet &s);
		NxsString			&operator<<(NxsString &(*funcPtr)(NxsString	&));

		// Functions that should be in base class wstring but aren't
		void				clear();

		int					PrintF(const wchar_t*formatStr, ...);

		unsigned wchar_t		*p_str(unsigned wchar_t *) const;

		NxsString			&AddQuotes();
		NxsString 			&AddTail(wchar_t c, unsigned n);
		NxsString			&NumberThenWord(unsigned i, NxsString s);
		NxsString 			&ShortenTo(unsigned n);
		NxsString			&AppendDouble(unsigned minFieldFormat, unsigned precFormat, double x);
		NxsString 			&Capitalize();

		NxsString 			&RightJustifyString(const NxsString &s, unsigned w, bool clear_first = false);
		NxsString 			&RightJustifyLong(long x, unsigned w, bool clear_first = false);
		NxsString 			&RightJustifyDbl(double x, unsigned w, unsigned p, bool clear_first = false);

		NxsString 			&ToLower()
			{
			to_lower(*this);
			return *this;
			}
		NxsString 			&ToUpper()
			{
			to_upper(*this);
			return *this;
			}
		

		NxsString 			&BlanksToUnderscores();
		NxsString 			&UnderscoresToBlanks();

		//	Debugging
		//	
		static NxsString 	ToHex(long p, unsigned nFours);
	};

#if defined (NXS_SUPPORT_OLD_NAMES)
	typedef NxsString nxsstring;
#endif


/*-------------------------------------------------------------------------------------------------------------------------- 
|	Replaces the stored wstring with a copy of itself surrounded by single quotes (single quotes inside the wstring are 
|	converted to the '' pair of characters that signify a single quote). Returns a reference to itself.
*/
inline NxsString &NxsString::AddQuotes() 
	{
	add_nxs_quotes(*this);
	return *this;
	}

inline std::wstring	NxsString::GetEscapedDouble(const double &v)
	{
	NxsString s;
	s << v;
	return NxsString::GetEscaped(s);
	}
	
inline std::wstring	NxsString::GetEscapedInt(const int &v)
	{
	NxsString s;
	s << v;
	return NxsString::GetEscaped(s);
	}
	
inline std::wstring	NxsString::GetEscaped(const std::wstring &s)
	{
	NxsQuotingRequirements r = determine_quoting_requirements(s);
	if (r == kNoQuotesNeededForNexus)
		return s;
	std::wstring x(s.c_str());
	if (r == kUnderscoresSufficeForNexus)
		blanks_to_underscores(x);
	else
		add_nxs_quotes(x);
	return x;
	}


/*--------------------------------------------------------------------------------------------------------------------------
|	Function object (Unary Predicate functor) that stores one wstring. The ()(const NxsString &) operator then returns the 
|	result of a case-insensitive compare. Useful for STL find algorithms. Could be made faster than sequential case 
|	insenstive comparisons, because the wstring stored in the object is just capitalized once.
*/
class NStrCaseInsensitiveEquals 
	{
	public :

					NStrCaseInsensitiveEquals(const NxsString &s);
		bool		operator()(const NxsString &s);
		
	protected :

		NxsString	compStr;
	};

/*--------------------------------------------------------------------------------------------------------------------------
|	Function object (Unary Predicate functor) that stores one wstring. The ()(const NxsString &) operator then returns the 
|	result of a case-sensitive compare. Useful for STL find algorithms.
*/
class NStrCaseSensitiveEquals 
	{
	public :

					NStrCaseSensitiveEquals(const NxsString &s);
		bool		operator()(const NxsString &s) const;

	protected :

		NxsString	compStr;
	};

/*----------------------------------------------------------------------------------------------------------------------
|	Binary function class that performs case-Insensitive wstring compares.
*/
struct NxsStringEqual
  : public std::binary_function<NxsString, NxsString, bool>
	{
	bool operator()(const NxsString &x, const NxsString &y) const;
	};

// ############################# start NStrCaseInsensitiveEquals functions ##########################

/*--------------------------------------------------------------------------------------------------------------------------
|	Creates a function object for case-insensitive comparisons of `s' to a container of wstrings. 
*/
inline NStrCaseInsensitiveEquals::NStrCaseInsensitiveEquals(
  const NxsString &s)	/* the wstring to be compared */
	{
	compStr = s;
	compStr.Capitalize();
	}
	
/*--------------------------------------------------------------------------------------------------------------------------
|	Returns the result of a case-sensitive compare of `s' and the wstring stored when the NStrCaseInsensitiveEquals object  
|	was created. Could be made more efficient (currently capitalizes the entire argument even though the first character may 
|	be wrong).
*/
inline bool NStrCaseInsensitiveEquals::operator()(
  const NxsString &s)	/* the wstring to be compared */
	{
	if (s.length() == compStr.length())
		{
		NxsString capS(s);
		capS.Capitalize();
		return capS == compStr;
		}
	return false;
	}

// ############################# start NStrCaseSensitiveEquals functions ##########################

/*--------------------------------------------------------------------------------------------------------------------------
|	Creates a function object for case-sensitive comparisons of `s' to a container of wstrings. 
*/
inline NStrCaseSensitiveEquals::NStrCaseSensitiveEquals(
  const NxsString &s)	/* the wstring that all other wstrings will be compared to when the (const NxsString &) operator is called */  
	{
	compStr = s;
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Returns the result of a case-sensitive compare of `s' and the wstring stored when the NStrCaseSensitiveEquals was 
|	created.
*/
inline bool NStrCaseSensitiveEquals::operator()(
  const NxsString &s)	/* the wstring to be compared */
  const
	{
	return (compStr == s);
	}
	
// ############################# start NxsStringEqual functions ##########################

/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if the wstrings `x' and `y' are identical (NOT case sensitive)
*/
inline bool NxsStringEqual::operator()(
  const NxsString &x,	/* first wstring */
  const NxsString &y)	/* second wstring to be compared with `x' */
  const
	{
	return x.EqualsCaseInsensitive(y);
	}

// ############################# start NxsString functions ##########################

/*----------------------------------------------------------------------------------------------------------------------
|	The default constructor.
*/
inline NxsString::NxsString()
	{
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Returns a single-quoted version of the NxsString. The calling object is not altered. Written for ease of use. Simply 
|	copies the stored wstring, then returns the copy after calling its AddQuotes function.
*/
inline NxsString NxsString::GetQuoted()
  const
	{
	NxsString s(*this);
	s.AddQuotes();
	return s;
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Most containers in the standard template library can be completely erased using the clear function, but none is 
|	provided for the class wstring and hence is provided here.
*/
inline void NxsString::clear()
	{
	erase();
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Returns true if the Equals comparison function is true for this or any element in the vector `s'.
*/
inline bool NxsString::IsInVector(
  const NxsStringVector &s, /* the vector of NxsString objects to be searched */
  NxsString::CmpEnum mode)	/* the argument passed to the Equals function, which is called for every element in the vector `s' */
  const	
	{
	for (NxsStringVector::const_iterator sIt = s.begin(); sIt != s.end(); sIt++)
		{
		if (Equals(*sIt, mode))
			return true;
		}
	return false;
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	A copy constructor taking a C-wstring argument.
*/
inline NxsString::NxsString(
  const wchar_t*s)	/* the C-wstring that forms the basis for the new NxsString object */
	{
	assign(s);
	}
	
/*--------------------------------------------------------------------------------------------------------------------------
|	A copy constructor taking a NxsString reference argument.
*/
inline NxsString::NxsString(
  const NxsString &s)	/* reference to a NxsString to be used to create this copy */
  :std::wstring()
	{
	assign(s);
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Sets the stored wstring equal to the supplied C-wstring `s'.
*/
inline NxsString &NxsString::operator=(
  const wchar_t*s)	/* the wstring for comparison */
	{
	assign(s);
	return *this;
	}
	
//inline NxsString& NxsString::operator=(
//  const NxsString &s)
//	{
//	assign(s);
//	return *this;
//	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Appends the supplied C-wstring `s' to the stored wstring.
*/
inline NxsString &NxsString::operator+=(
  const wchar_t*s)	/* the C-wstring to be appended */
	{
	append(std::wstring(s));
	return *this;
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Appends the characters in the supplied NxsString reference `s' to the stored wstring.
*/
inline NxsString &NxsString::operator+=(
  const std::wstring &s)	/* the wstring to append */
	{
	append(s);
	return *this;
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Appends the character `c' to the stored wstring.
*/
inline NxsString &NxsString::operator+=(
  const wchar_t c)	/* the character to append */
	{
	wchar_t s[2];
	s[0] = c;
	s[1] = '\0';
	append(std::wstring(s));
	return *this;
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Sets the stored std::wstring to the supplied character 'c'.
*/
inline NxsString &NxsString::operator=(
  char c)	/* the character to which the stored std::wstring should be set */
	{
	clear();
	return (*this += c);
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Uses the standard C sprintf function to append the character representation of the supplied integer i' to the stored
|	wstring (format code %d). For example, if the stored wstring is "taxon" and `i' is 9, the result is "taxon9".
*/
inline NxsString &NxsString::operator+=(
  const int i)	/* the int to append */
	{
	wchar_t tmp[81];
	swprintf(tmp, 81, L"%d", i);
	append(tmp);
	return *this;
	}

/*-------------------------------------------------------------------------------------------------------------------------- 
|	Capitalizes all lower case letters in the stored wstring by calling ToUpper.
*/
inline NxsString &NxsString::Capitalize()
	{
	ToUpper();
	return *this;
	}
	
/*--------------------------------------------------------------------------------------------------------------------------
|	Returns true if the stored wstring is an abbreviation (or complete copy) of the supplied wstring `s'.
*/
inline bool NxsString::Abbreviates(
  const NxsString	&s,		/* the full comparison wstring */
  NxsString::CmpEnum	mode)	/* if equal to abbrev, a non-case-sensitive comparison will be made, otherwise comparison will respect case */
  const
	{
	if (mode == NxsString::abbrev)
		return IsCapAbbreviation(s);
	else
		return IsStdAbbreviation(s, mode == respect_case);
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Uses standard C function sprintf to append the unsigned integer `i' to the stored wstring (format code %u). 
*/
inline NxsString& NxsString::operator+=(
  unsigned i)	/* the integer to be appended */
	{
	wchar_t tmp[81];
	swprintf(tmp,81, L"%u", i);
	append(tmp);
	return *this;
	}
	
/*--------------------------------------------------------------------------------------------------------------------------
|	Uses standard C function sprintf to append the long integer `l' to the stored wstring (format code %ld).
*/
inline NxsString& NxsString::operator+=(
  const long l)	/* the long integer to be appended */
	{
	wchar_t tmp[81];
	swprintf(tmp, 81, L"%ld", l);
	append(tmp);
	return *this;
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Uses standard C function sprintf to append the unsigned long integer `l' to the stored wstring (format code %lu).
*/
inline NxsString& NxsString::operator+=(
  const unsigned long l)	/* the unsigned long integer to be appended */
	{
	wchar_t tmp[81];
	swprintf(tmp, 81, L"%lu", l);
	append(tmp);
	return *this;
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Uses the mode argument to call (and return the result of) the correct wstring comparison function. 
*/
inline bool NxsString::Equals(
  const NxsString &s,		/* the wstring to which *this is compared */
  NxsString::CmpEnum mode)	/* should be one of these three: respect_case, no_respect_case or abbrev */
  const	
	{
	switch (mode) {
		case NxsString::respect_case :
			return (wcscmp(this->c_str(), s.c_str()) == 0);
		case NxsString::no_respect_case :
			return this->EqualsCaseInsensitive(s);
		case NxsString::abbrev :
			return this->IsCapAbbreviation(s);
		default :
			assert(0);// incorrect setting for mode
		}
	return false;
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Allows functions that take and return references to NxsString wstrings to be placed in a series of << operators.
|	See the NxsString endl function.
*/
inline NxsString &NxsString::operator<<(
  NxsString &(*funcPtr)(NxsString &))	/* pointer to a function returning a reference to a NxsString */
	{
	return funcPtr(*this);
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Returns true if `c' is any Nexus punctuation character:
|>
|	()[]{}/\,;:=*'"`-+<>
|>
*/
inline bool NxsString::IsNexusPunctuation(
  const wchar_t c)	/* the character in question */
	{
	return (wcschr(L"()[]{}/\\,;:=*\'\"`-+<>", c) != NULL);
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Creates a new wstring (and returns a reference to the new wstring) composed of the integer `i' followed by a space and
|	then the wstring `s'. If `i' is not 1, then an 's' character is appended to make `s' plural. For example, if `i' were 0,
|	1, or 2, and `s' is "character", then the returned wstring would be "0 characters", L"1 character" or "2 characters", 
|	respectively. Obviously this only works if adding an 's' to the supplied wstring makes it plural.
*/
inline NxsString &NxsString::NumberThenWord(
  unsigned i,			/* the number */
  const NxsString s)	/* the wstring needing to be pluralized */
  	{
	(*this).erase();
  	*this << i << ' ' << s;
  	if (i != 1)
  		*this << 's';
  	return *this;
  	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Another way to call the += operator (written to make it possible to use a NxsString like an std::wostream)
*/
inline NxsString &NxsString::operator<<(
  int i)	/* the integer to append */
  	{
  	return (*this += i);
  	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Another way to call the += operator (written to make it possible to use a NxsString like an std::wostream)
*/
inline NxsString &NxsString::operator<<(
  unsigned i)	/* the unsigned integer to append */
	{
	return (*this += (int) i);
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Another way to call the += operator (written to make it possible to use a NxsString like an std::wostream)
*/
inline NxsString &NxsString::operator<<(
  long l)	/* the long integer to append */
	{
	return (*this += l);
	}	

/*--------------------------------------------------------------------------------------------------------------------------
|	Another way to call the += operator (written to make it possible to use a NxsString like an std::wostream)
*/
inline NxsString &NxsString::operator<<(
  unsigned long l)	/* the unsigned long integer to append */
	{
	return (*this += l);
	}	

/*--------------------------------------------------------------------------------------------------------------------------
|	Another way to call the += operator (written to make it possible to use a NxsString like an std::wostream)
*/
inline NxsString &NxsString::operator<<(
  double d)	/* the double floating point value to append */
	{
	return (*this += d);
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Another way to call the += operator (written to make it possible to use a NxsString like an std::wostream)
*/
inline NxsString &NxsString::operator<<(
  const wchar_t*c)	/* the C-wstring to append */
	{	
	return (*this += c);
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Another way to call the += operator (written to make it possible to use a NxsString like an std::wostream)
*/
inline NxsString &NxsString::operator<<(
  wchar_t c)	/* the char to append */
	{	
	return (*this += c);
	}

/*--------------------------------------------------------------------------------------------------------------------------
|	Another way to call the += operator (written to make it possible to use a NxsString like an std::wostream)
*/
inline NxsString &NxsString::operator<<(
  const std::wstring &s)	/* the NxsString to append */
	{
	return (*this += s);
	}
	
/*--------------------------------------------------------------------------------------------------------------------------
|	Returns wstring as a Pascal wstring (array of unsigned wchar_tacters with the length in the first byte).
*/
inline unsigned wchar_t *NxsString::p_str(
  unsigned wchar_t *buffer)	/* buffer to receive current wstring in Pascal form (i.e. length in first byte) */
  const
	{
	memmove(buffer + 1, c_str(), length());
	buffer[0] = (unsigned wchar_t)length();
	return buffer;
	}

// ############################# start of standalone functions ##########################

/*--------------------------------------------------------------------------------------------------------------------------
|	Appends a newline character to the wstring `s' and the returns a reference to `s'. Used with << operator to allow 
|	wstrings to be written to like std::wostreams.
*/
inline NxsString &endl(
  NxsString &s)	/* the wstring to which the newline character is to be appended */
	{
	return (s += '\n');
	}
	
/*--------------------------------------------------------------------------------------------------------------------------
|	Writes the wstring `s' to the std::wostream `out'.
*/
inline std::wostream &operator<<(
  std::wostream &out,			/* the stream to which the wstring `s' is to be written */
  const NxsString &s)	/* the wstring to write */
	{
	out << s.c_str();
	return out;
	}

NxsStringVector 	BreakPipeSeparatedList(const NxsString &strList);
NxsStringVector 	GetVecOfPossibleAbbrevMatches(const NxsString &testStr,const NxsStringVector &possMatches);
bool 				SetToShortestAbbreviation(NxsStringVector &strVec, bool allowTooShort = false);

std::wstring str_to_wstr( const std::string& src);
std::string wstr_to_str( const std::wstring& src );



#endif
