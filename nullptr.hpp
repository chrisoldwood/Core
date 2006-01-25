/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		NULLPTR.HPP
** COMPONENT:	Core C++ Library
** DESCRIPTION:	The nullptr class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef NULLPTR_HPP
#define NULLPTR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

/******************************************************************************
**
** Adapted from the May 2004 CUJ article by Herb Sutter.
**
*******************************************************************************
*/

// VC++ 7.x
#if _MSC_VER > 1200

const								// this is a const object...
class
{
public:
	template<class T>				// convertible to any type
	operator T*() const				// of null non-member pointer...
	{ return 0; }

	template<class C, class T>		// or any type of null
	operator T C::*() const			// member pointer...
	{ return 0; }

private:
	void operator&() const;			// whose address can't be taken

} nullptr = {};						// and whose name is nullptr.

// VC++ 6.x
#else

#define nullptr 0

#endif

#endif // NULLPTR_HPP
