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

void ansiToWide(const char* begin, const char* end, wchar_t* dest)
{
	std::use_facet< std::ctype<wchar_t> >(std::locale()).widen(begin, end, dest);
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from ANSI to Wide.

std::wstring ansiToWide(const char* begin, const char* end)
{
	std::wstring string;

	size_t length = end - begin;

	// Allocate the return value.
	string.resize(length);

	wchar_t* dest = const_cast<wchar_t*>(string.data());

	// Do the conversion.
	ansiToWide(begin, end, dest);

	return string;
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from Wide to ANSI.

void wideToAnsi(const wchar_t* begin, const wchar_t* end, char* dest)
{
	std::use_facet< std::ctype<wchar_t> >(std::locale()).narrow(begin, end, '?', dest);
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string from Wide to ANSI.

std::string wideToAnsi(const wchar_t* begin, const wchar_t* end)
{
	std::string str;

	size_t length = end - begin;

	// Allocate the return value.
	str.resize(length);

	char* dest = const_cast<char*>(str.data());

	// Do the conversion.
	wideToAnsi(begin, end, dest);

	return str;
}

////////////////////////////////////////////////////////////////////////////////
//! Construct from an ANSI string.

Core::AnsiToWide::AnsiToWide(const char* string)
	: m_string()
{
	size_t length = strlen(string);

	m_string = new wchar_t[length+1];

	ansiToWide(string, string+length, m_string);

	m_string[length] = L'\0';
}

////////////////////////////////////////////////////////////////////////////////
//! Construct from an ANSI string.

Core::AnsiToWide::AnsiToWide(const std::string& str)
	: m_string()
{
	size_t      length = str.length();
	const char* string = (str.empty()) ? nullptr : &str[0];

	m_string = new wchar_t[length+1];

	ansiToWide(string, string+length, m_string);

	m_string[length] = L'\0';
}

////////////////////////////////////////////////////////////////////////////////
//! Construct from a wide string.

Core::WideToAnsi::WideToAnsi(const wchar_t* string)
	: m_string()
{
	size_t length = wcslen(string);

	m_string = new char[length+1];

	wideToAnsi(string, string+length, m_string);

	m_string[length] = '\0';
}

////////////////////////////////////////////////////////////////////////////////
//! Construct from a wide string.

Core::WideToAnsi::WideToAnsi(const std::wstring& str)
	: m_string()
{
	size_t         length = str.length();
	const wchar_t* string = (str.empty()) ? nullptr : &str[0];

	m_string = new char[length+1];

	wideToAnsi(string, string+length, m_string);

	m_string[length] = '\0';
}

//namespace Core
}
