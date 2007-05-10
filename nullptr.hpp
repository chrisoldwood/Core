////////////////////////////////////////////////////////////////////////////////
//! \file   nullptr.hpp
//! \brief  The nullptr class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_NULLPTR_HPP
#define CORE_NULLPTR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

// VC++ 7.x
#if _MSC_VER > 1200

////////////////////////////////////////////////////////////////////////////////
//! A class to simulate the proposed nullptr keyword. This was adapted from the
//! May 2004 CUJ article by Herb Sutter.

const class nullptr_t
{
public:
	//! Allow conversion to any type of null non-member pointer.
	template<class T>
	operator T*() const
	{ return 0; }

	//! Allow conversion to any type of null member pointer.
	template<class C, class T>
	operator T C::*() const
	{ return 0; }

private:
	//! Don't allow address to be taken.
	void operator&() const;

} nullptr = {};

// VC++ 6.x
#else

// Pre VC++ 6.x can't cope with the template.
#define nullptr 0					

#endif

#endif // CORE_NULLPTR_HPP
