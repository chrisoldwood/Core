////////////////////////////////////////////////////////////////////////////////
//! \file   Functional.hpp
//! \brief  Extensions to the standard \<functional\> functions.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_FUNCTIONAL_HPP
#define CORE_FUNCTIONAL_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <functional>

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Wrapper for the deprecated std::unary_function.

#if __cplusplus < 201103L

template<typename Arg, typename Result>
struct unary_function : std::unary_function<Arg, Result>
{ };

#else

template<typename Arg, typename Result>
struct unary_function
{
	typedef Arg argument_type;
	typedef Result result_type;
};

#endif

//namespace Core
}

#endif // CORE_FUNCTIONAL_HPP
