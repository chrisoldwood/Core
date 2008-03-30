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

void AnsiToWide(const char* pszBegin, const char* pszEnd, wchar_t* pszDst);

////////////////////////////////////////////////////////////////////////////////
// Convert a string from ANSI to Wide.

std::wstring AnsiToWide(const char* pszBegin, const char* pszEnd);

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from ANSI to Wide.

inline std::wstring AnsiToWide(const char* psz)
{
	return AnsiToWide(psz, psz + strlen(psz));
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from ANSI to Wide.

inline std::wstring AnsiToWide(const std::string& str)
{
	const char* pszBegin = &(*str.begin());
	const char* pszEnd   = &(*str.end());

	return AnsiToWide(pszBegin, pszEnd);
}

////////////////////////////////////////////////////////////////////////////////
// Convert a string from Wide to ANSI.

void WideToAnsi(const wchar_t* pszBegin, const wchar_t* pszEnd, char* pszDst);

////////////////////////////////////////////////////////////////////////////////
// Convert a string from Wide to ANSI.

std::string WideToAnsi(const wchar_t* pszBegin, const wchar_t* pszEnd);

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from Wide to ANSI.

inline std::string WideToAnsi(const wchar_t* psz)
{
	return WideToAnsi(psz, psz + wcslen(psz));
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from Wide to ANSI.

inline std::string WideToAnsi(const std::wstring& str)
{
	const wchar_t* pszBegin = &(*str.begin());
	const wchar_t* pszEnd   = &(*str.end());

	return WideToAnsi(pszBegin, pszEnd);
}

////////////////////////////////////////////////////////////////////////////////
//! The class used to do the conversion from ANSI to Wide via the X2Y() macros.
//  NB: The sole member must be a c-style string to work with var_args functions.

class ANSI2WIDE
{
public:
	//! Construct from an ANSI string.
	ANSI2WIDE(const char* psz);

	//! Construct from an ANSI string.
	explicit ANSI2WIDE(const std::string& str);

	//! Destructor
	~ANSI2WIDE();

	//! Conversion operator for a wide string.
	operator const wchar_t*() const;

private:
	//
	// Members.
	//
	wchar_t*	m_psz;	//! The converted string.
};

////////////////////////////////////////////////////////////////////////////////
//! Destructor

inline ANSI2WIDE::~ANSI2WIDE()
{
	delete[] m_psz;
}

////////////////////////////////////////////////////////////////////////////////
//! Conversion operator for a wide string.

inline ANSI2WIDE::operator const wchar_t*() const
{
	return m_psz;
}

////////////////////////////////////////////////////////////////////////////////
//! The class used to do the conversion from Wide to ANSI via the X2Y() macros.
//  NB: The sole member must be a c-style string to work with var_args functions.

class WIDE2ANSI
{
public:
	//! Construct from a wide string.
	WIDE2ANSI(const wchar_t* psz);

	//! Construct from a wide string.
	explicit WIDE2ANSI(const std::wstring& str);

	//! Destructor.
	~WIDE2ANSI();

	//! Conversion operator for an ANSI string.
	operator const char*() const;

private:
	//
	// Members.
	//
	char*	m_psz;	//! The converted string.
};

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline WIDE2ANSI::~WIDE2ANSI()
{
	delete[] m_psz;
}

////////////////////////////////////////////////////////////////////////////////
//! Conversion operator for an ANSI string.

inline WIDE2ANSI::operator const char*() const
{
	return m_psz;
}

////////////////////////////////////////////////////////////////////////////////
// Compatibility macros and classes to replace use of <atlconv.h>

//! Convert an ANSI string to Wide.
#define A2W(psz)	static_cast<const wchar_t*>(Core::ANSI2WIDE(psz))

//! Convert a Wide string to ANSI.
#define W2A(psz)	static_cast<const char*>(Core::WIDE2ANSI(psz))

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
