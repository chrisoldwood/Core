////////////////////////////////////////////////////////////////////////////////
//! \file   AnsiWide.cpp
//! \brief  Ansi/Wide string conversion functions.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AnsiWide.hpp"

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from ANSI to Wide.

void AnsiToWide(const char* pszBegin, const char* pszEnd, wchar_t* pszDst)
{
	std::use_facet< std::ctype<wchar_t> >(std::locale()).widen(pszBegin, pszEnd, pszDst);
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from ANSI to Wide.

std::wstring AnsiToWide(const char* pszBegin, const char* pszEnd)
{
	std::wstring str;

	size_t nLength = pszEnd - pszBegin;

	// Allocate the return value.
	str.resize(nLength);

	wchar_t* pszDst = &(*str.begin());

	// Do the conversion.
	AnsiToWide(pszBegin, pszEnd, pszDst);

	return str;
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from Wide to ANSI.

void WideToAnsi(const wchar_t* pszBegin, const wchar_t* pszEnd, char* pszDst)
{
	std::use_facet< std::ctype<wchar_t> >(std::locale()).narrow(pszBegin, pszEnd, '?', pszDst);
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from Wide to ANSI.

std::string WideToAnsi(const wchar_t* pszBegin, const wchar_t* pszEnd)
{
	std::string str;

	size_t nLength = pszEnd - pszBegin;

	// Allocate the return value.
	str.resize(nLength);

	char* pszDst = &(*str.begin());

	// Do the conversion.
	WideToAnsi(pszBegin, pszEnd, pszDst);

	return str;
}

////////////////////////////////////////////////////////////////////////////////
//! Construct from an ANSI string.

Core::ANSI2WIDE::ANSI2WIDE(const char* psz)
{
	size_t nLength = strlen(psz);

	m_psz = new wchar_t[nLength+1];

	AnsiToWide(psz, psz+nLength, m_psz);

	m_psz[nLength] = L'\0';
}

////////////////////////////////////////////////////////////////////////////////
//! Construct from an ANSI string.

Core::ANSI2WIDE::ANSI2WIDE(const std::string& str)
{
	size_t      nLength = str.length();
	const char* psz     = (str.empty()) ? nullptr : &str[0];

	m_psz = new wchar_t[nLength+1];

	AnsiToWide(psz, psz+nLength, m_psz);

	m_psz[nLength] = L'\0';
}

////////////////////////////////////////////////////////////////////////////////
//! Construct from a wide string.

Core::WIDE2ANSI::WIDE2ANSI(const wchar_t* psz)
{
	size_t nLength = wcslen(psz);

	m_psz = new char[nLength+1];

	WideToAnsi(psz, psz+nLength, m_psz);

	m_psz[nLength] = '\0';
}

////////////////////////////////////////////////////////////////////////////////
//! Construct from a wide string.

Core::WIDE2ANSI::WIDE2ANSI(const std::wstring& str)
{
	size_t         nLength = str.length();
	const wchar_t* psz     = (str.empty()) ? nullptr : &str[0];

	m_psz = new char[nLength+1];

	WideToAnsi(psz, psz+nLength, m_psz);

	m_psz[nLength] = '\0';
}

//namespace Core
}
