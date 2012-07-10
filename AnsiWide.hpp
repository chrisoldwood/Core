////////////////////////////////////////////////////////////////////////////////
//! \file   AnsiWide.hpp
//! \brief  Ansi/Wide string conversion functions.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_ANSIWIDE_HPP
#define CORE_ANSIWIDE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
// Convert a string from ANSI to Wide.

void ansiToWide(const char* begin, const char* end, wchar_t* dest);

////////////////////////////////////////////////////////////////////////////////
// Convert a string from ANSI to Wide.

std::wstring ansiToWide(const char* begin, const char* end);

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from ANSI to Wide.

inline std::wstring ansiToWide(const char* string)
{
	return ansiToWide(string, string + strlen(string));
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from ANSI to Wide.

inline std::wstring ansiToWide(const std::string& string)
{
	const char* begin = string.data();
	const char* end   = begin + string.size();

	return ansiToWide(begin, end);
}

////////////////////////////////////////////////////////////////////////////////
// Convert a string from Wide to ANSI.

void wideToAnsi(const wchar_t* begin, const wchar_t* end, char* dest);

////////////////////////////////////////////////////////////////////////////////
// Convert a string from Wide to ANSI.

std::string wideToAnsi(const wchar_t* begin, const wchar_t* end);

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from Wide to ANSI.

inline std::string wideToAnsi(const wchar_t* string)
{
	return wideToAnsi(string, string + wcslen(string));
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from Wide to ANSI.

inline std::string wideToAnsi(const std::wstring& string)
{
	const wchar_t* begin = string.data();
	const wchar_t* end   = begin + string.size();

	return wideToAnsi(begin, end);
}

////////////////////////////////////////////////////////////////////////////////
//! The class used to do the conversion from ANSI to Wide via the X2Y() macros.
//  NB: The sole member must be a c-style string to work with var_args functions.

class AnsiToWide /*: private NotCopyable*/
{
public:
	//! Construct from an ANSI string.
	AnsiToWide(const char* string);

	//! Construct from an ANSI string.
	explicit AnsiToWide(const std::string& string);

	//! Destructor
	~AnsiToWide();

	//! Conversion operator for a wide string.
	operator const wchar_t*() const;

private:
	//
	// Members.
	//
	wchar_t*	m_string;	//! The converted string.

	// NotCopyable.
	AnsiToWide(const AnsiToWide&);
	AnsiToWide& operator=(const AnsiToWide&);
};

////////////////////////////////////////////////////////////////////////////////
//! Destructor

inline AnsiToWide::~AnsiToWide()
{
	delete[] m_string;
}

////////////////////////////////////////////////////////////////////////////////
//! Conversion operator for a wide string.

inline AnsiToWide::operator const wchar_t*() const
{
	return m_string;
}

////////////////////////////////////////////////////////////////////////////////
//! The class used to do the conversion from Wide to ANSI via the X2Y() macros.
//  NB: The sole member must be a c-style string to work with var_args functions.

class WideToAnsi /*: private NotCopyable*/
{
public:
	//! Construct from a wide string.
	WideToAnsi(const wchar_t* string);

	//! Construct from a wide string.
	explicit WideToAnsi(const std::wstring& string);

	//! Destructor.
	~WideToAnsi();

	//! Conversion operator for an ANSI string.
	operator const char*() const;

private:
	//
	// Members.
	//
	char*	m_string;	//! The converted string.

	// NotCopyable.
	WideToAnsi(const WideToAnsi&);
	WideToAnsi& operator=(const WideToAnsi&);
};

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline WideToAnsi::~WideToAnsi()
{
	delete[] m_string;
}

////////////////////////////////////////////////////////////////////////////////
//! Conversion operator for an ANSI string.

inline WideToAnsi::operator const char*() const
{
	return m_string;
}

////////////////////////////////////////////////////////////////////////////////
// Compatibility macros and classes to replace use of <atlconv.h>

//! Helper function for use NOP conversions.
inline const tchar* getCharPtr(const tchar* string)
{
	return string;
}

//! Helper function for use NOP conversions.
inline const tchar* getCharPtr(const tstring& string)
{
	return string.c_str();
}

//! Convert an ANSI string to Wide.
#define A2W(string)	static_cast<const wchar_t*>(Core::AnsiToWide(string))

//! Convert a Wide string to ANSI.
#define W2A(string)	static_cast<const char*>(Core::WideToAnsi(string))

// ANSI build.
#ifdef ANSI_BUILD

//! Convert an ANSI string to a TCHAR string.
#define A2T(string)	(Core::getCharPtr(string))
//! Convert a TCHAR string to ANSI.
#define T2A(string)	(Core::getCharPtr(string))

//! Convert a Wide string to a TCHAR string.
#define W2T(string)	W2A(string)
//! Convert a TCHAR string to Wide.
#define T2W(string)	A2W(string)

// UNICODE build.
#else

//! Convert an ANSI string to a TCHAR string.
#define A2T(string)	A2W(string)
//! Convert a TCHAR string to ANSI.
#define T2A(string)	W2A(string)

//! Convert a Wide string to a TCHAR string.
#define W2T(string)	(Core::getCharPtr(string))
//! Convert a TCHAR string to Wide.
#define T2W(string)	(Core::getCharPtr(string))

#endif

//namespace Core
}

#endif // CORE_ANSIWIDE_HPP
