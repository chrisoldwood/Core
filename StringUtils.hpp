////////////////////////////////////////////////////////////////////////////////
//! \file   StringUtils.hpp
//! \brief  .
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_STRINGUTILS_HPP
#define CORE_STRINGUTILS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Format the string ala printf.

std::tstring FmtEx(const tchar* pszFormat, va_list args);

////////////////////////////////////////////////////////////////////////////////
//! Format the string ala printf.

std::tstring Fmt(const tchar* pszFormat, ...);

//namespace Core
}

#endif // CORE_STRINGUTILS_HPP
