////////////////////////////////////////////////////////////////////////////////
//! \file   AnsiWide.cpp
//! \brief  Ansi/Wide string conversion functions.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AnsiWide.hpp"
#include <locale>

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from ANSI to Wide.

void ansiToWide(const char* pszBegin, const char* pszEnd, wchar_t* pszDst)
{
	std::use_facet< std::ctype<wchar_t> >(std::locale()).widen(pszBegin, pszEnd, pszDst);
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from ANSI to Wide.

std::wstring ansiToWide(const char* pszBegin, const char* pszEnd)
{
	std::wstring str;

	size_t nLength = pszEnd - pszBegin;

	// Allocate the return value.
	str.resize(nLength);

	wchar_t* pszDst = const_cast<wchar_t*>(str.data());

	// Do the conversion.
	ansiToWide(pszBegin, pszEnd, pszDst);

	return str;
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from Wide to ANSI.

void wideToAnsi(const wchar_t* pszBegin, const wchar_t* pszEnd, char* pszDst)
{
	std::use_facet< std::ctype<wchar_t> >(std::locale()).narrow(pszBegin, pszEnd, '?', pszDst);
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from Wide to ANSI.

std::string wideToAnsi(const wchar_t* pszBegin, const wchar_t* pszEnd)
{
	std::string str;

	size_t nLength = pszEnd - pszBegin;

	// Allocate the return value.
	str.resize(nLength);

	char* pszDst = const_cast<char*>(str.data());

	// Do the conversion.
	wideToAnsi(pszBegin, pszEnd, pszDst);

	return str;
}

////////////////////////////////////////////////////////////////////////////////
//! Construct from an ANSI string.

Core::AnsiToWide::AnsiToWide(const char* psz)
	: m_psz()
{
	size_t nLength = strlen(psz);

	m_psz = new wchar_t[nLength+1];

	ansiToWide(psz, psz+nLength, m_psz);

	m_psz[nLength] = L'\0';
}

////////////////////////////////////////////////////////////////////////////////
//! Construct from an ANSI string.

Core::AnsiToWide::AnsiToWide(const std::string& str)
	: m_psz()
{
	size_t      nLength = str.length();
	const char* psz     = (str.empty()) ? nullptr : &str[0];

	m_psz = new wchar_t[nLength+1];

	ansiToWide(psz, psz+nLength, m_psz);

	m_psz[nLength] = L'\0';
}

////////////////////////////////////////////////////////////////////////////////
//! Construct from a wide string.

Core::WideToAnsi::WideToAnsi(const wchar_t* psz)
	: m_psz()
{
	size_t nLength = wcslen(psz);

	m_psz = new char[nLength+1];

	wideToAnsi(psz, psz+nLength, m_psz);

	m_psz[nLength] = '\0';
}

////////////////////////////////////////////////////////////////////////////////
//! Construct from a wide string.

Core::WideToAnsi::WideToAnsi(const std::wstring& str)
	: m_psz()
{
	size_t         nLength = str.length();
	const wchar_t* psz     = (str.empty()) ? nullptr : &str[0];

	m_psz = new char[nLength+1];

	wideToAnsi(psz, psz+nLength, m_psz);

	m_psz[nLength] = '\0';
}

//namespace Core
}
