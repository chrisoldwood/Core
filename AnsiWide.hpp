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

void ansiToWide(const char* pszBegin, const char* pszEnd, wchar_t* pszDst);

////////////////////////////////////////////////////////////////////////////////
// Convert a string from ANSI to Wide.

std::wstring ansiToWide(const char* pszBegin, const char* pszEnd);

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from ANSI to Wide.

inline std::wstring ansiToWide(const char* psz)
{
	return ansiToWide(psz, psz + strlen(psz));
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from ANSI to Wide.

inline std::wstring ansiToWide(const std::string& str)
{
	const char* pszBegin = str.data();
	const char* pszEnd   = pszBegin + str.size();

	return ansiToWide(pszBegin, pszEnd);
}

////////////////////////////////////////////////////////////////////////////////
// Convert a string from Wide to ANSI.

void wideToAnsi(const wchar_t* pszBegin, const wchar_t* pszEnd, char* pszDst);

////////////////////////////////////////////////////////////////////////////////
// Convert a string from Wide to ANSI.

std::string wideToAnsi(const wchar_t* pszBegin, const wchar_t* pszEnd);

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from Wide to ANSI.

inline std::string wideToAnsi(const wchar_t* psz)
{
	return wideToAnsi(psz, psz + wcslen(psz));
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from Wide to ANSI.

inline std::string wideToAnsi(const std::wstring& str)
{
	const wchar_t* pszBegin = str.data();
	const wchar_t* pszEnd   = pszBegin + str.size();

	return wideToAnsi(pszBegin, pszEnd);
}

////////////////////////////////////////////////////////////////////////////////
//! The class used to do the conversion from ANSI to Wide via the X2Y() macros.
//  NB: The sole member must be a c-style string to work with var_args functions.

class AnsiToWide /*: private NotCopyable*/
{
public:
	//! Construct from an ANSI string.
	AnsiToWide(const char* psz);

	//! Construct from an ANSI string.
	explicit AnsiToWide(const std::string& str);

	//! Destructor
	~AnsiToWide();

	//! Conversion operator for a wide string.
	operator const wchar_t*() const;

private:
	//
	// Members.
	//
	wchar_t*	m_psz;	//! The converted string.

	// NotCopyable.
	AnsiToWide(const AnsiToWide&);
	AnsiToWide& operator=(const AnsiToWide&);
};

////////////////////////////////////////////////////////////////////////////////
//! Destructor

inline AnsiToWide::~AnsiToWide()
{
	delete[] m_psz;
}

////////////////////////////////////////////////////////////////////////////////
//! Conversion operator for a wide string.

inline AnsiToWide::operator const wchar_t*() const
{
	return m_psz;
}

////////////////////////////////////////////////////////////////////////////////
//! The class used to do the conversion from Wide to ANSI via the X2Y() macros.
//  NB: The sole member must be a c-style string to work with var_args functions.

class WideToAnsi /*: private NotCopyable*/
{
public:
	//! Construct from a wide string.
	WideToAnsi(const wchar_t* psz);

	//! Construct from a wide string.
	explicit WideToAnsi(const std::wstring& str);

	//! Destructor.
	~WideToAnsi();

	//! Conversion operator for an ANSI string.
	operator const char*() const;

private:
	//
	// Members.
	//
	char*	m_psz;	//! The converted string.

	// NotCopyable.
	WideToAnsi(const WideToAnsi&);
	WideToAnsi& operator=(const WideToAnsi&);
};

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline WideToAnsi::~WideToAnsi()
{
	delete[] m_psz;
}

////////////////////////////////////////////////////////////////////////////////
//! Conversion operator for an ANSI string.

inline WideToAnsi::operator const char*() const
{
	return m_psz;
}

////////////////////////////////////////////////////////////////////////////////
// Compatibility macros and classes to replace use of <atlconv.h>

//! Convert an ANSI string to Wide.
#define A2W(psz)	static_cast<const wchar_t*>(Core::AnsiToWide(psz))

//! Convert a Wide string to ANSI.
#define W2A(psz)	static_cast<const char*>(Core::WideToAnsi(psz))

// ANSI build.
#ifdef ANSI_BUILD

//! Convert an ANSI string to a TCHAR string.
#define A2T(psz)	(psz)
//! Convert a TCHAR string to ANSI.
#define T2A(psz)	(psz)

//! Convert a Wide string to a TCHAR string.
#define W2T(psz)	W2A(psz)
//! Convert a TCHAR string to Wide.
#define T2W(psz)	A2W(psz)

// UNICODE build.
#else

//! Convert an ANSI string to a TCHAR string.
#define A2T(psz)	A2W(psz)
//! Convert a TCHAR string to ANSI.
#define T2A(psz)	W2A(psz)

//! Convert a Wide string to a TCHAR string.
#define W2T(psz)	(psz)
//! Convert a TCHAR string to Wide.
#define T2W(psz)	(psz)

#endif

//namespace Core
}

#endif // CORE_ANSIWIDE_HPP
