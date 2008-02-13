////////////////////////////////////////////////////////////////////////////////
//! \file   Types.hpp
//! \brief  Common data types, defines and macros.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_TYPES_HPP
#define CORE_TYPES_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// Standard types.

typedef unsigned char		uchar;		//!< An unsigned character.
typedef unsigned short		ushort;		//!< An unsigned short integer.
typedef unsigned int		uint;		//!< An unsigned integer.
typedef unsigned long		ulong;		//!< An unsigned long integer.

////////////////////////////////////////////////////////////////////////////////
// Size specific types.

typedef unsigned char		byte;		//!< 8 bit unsigned integer.
typedef unsigned short		word;		//!< 16 bit unsigned integer.
typedef unsigned long		dword;		//!< 32 bit unsigned integer.

typedef signed char			int8;		//!< 8 bit signed integer.
typedef signed short		int16;		//!< 16 bit signed integer.
typedef signed long			int32;		//!< 32 bit signed integer.
typedef signed __int64		int64;		//!< 64 bit signed integer.

typedef unsigned char		uint8;		//!< 8 bit unsigned integer.
typedef unsigned short		uint16;		//!< 16 bit unsigned integer.
typedef unsigned long		uint32;		//!< 32 bit unsigned integer.
typedef unsigned __int64	uint64;		//!< 64 bit unsigned integer.

////////////////////////////////////////////////////////////////////////////////
// Common macros.

#ifndef ARRAY_SIZE
//! Calculate the number of elements in a simple array.
#define	ARRAY_SIZE(array)	(sizeof(array)/sizeof((array)[0]))
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
// Common constants.

//! A general purpose equivalent to basic_string::npos
static const size_t npos = static_cast<size_t>(-1);

//namespace Core
}

#endif // CORE_TYPES_HPP
