////////////////////////////////////////////////////////////////////////////////
//! \file   tfstream.hpp
//! \brief  Build independent fstream header.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_TFSTREAM_HPP
#define CORE_TFSTREAM_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "tiosfwd.hpp"

#ifdef _MSC_VER
#pragma push_macro("new")
#undef new
#endif

// Standard header redefines 'new'.
#include <fstream>

#ifdef _MSC_VER
#pragma pop_macro("new")
#endif

#endif // CORE_TFSTREAM_HPP
