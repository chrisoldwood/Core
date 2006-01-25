/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TYPES.HPP
** COMPONENT:	Core C++ Library
** DESCRIPTION:	Common data types, defines and macros.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef TYPES_HPP
#define TYPES_HPP

#if _MSC_VER > 1000
#pragma once
#endif

/******************************************************************************
**
** Standard types.
**
*******************************************************************************
*/

typedef unsigned char		uchar;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned long		ulong;

/******************************************************************************
**
** Size specific types.
**
*******************************************************************************
*/

typedef unsigned char		byte;
typedef unsigned short		word;
typedef unsigned long		dword;

typedef signed char			int8;
typedef signed short		int16;
typedef signed long			int32;
typedef signed __int64		int64;

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned long		uint32;
typedef unsigned __int64	uint64;

#endif //TYPES_HPP
