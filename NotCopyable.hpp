////////////////////////////////////////////////////////////////////////////////
//! \file   NotCopyable.hpp
//! \brief  The NotCopyable class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_NOTCOPYABLE_HPP
#define CORE_NOTCOPYABLE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Base class used to disable copying of the derived class. This class should
//! be privately inherited from.

class NotCopyable
{
protected:
	// Enable construction.
	NotCopyable() {};

private:
	//! Disable copy constructor.
	NotCopyable(const NotCopyable&);

	//! Disable assignment.
	NotCopyable& operator=(const NotCopyable&);
};

////////////////////////////////////////////////////////////////////////////////
//! The NotCopyable class does not work well with the GCC -Weff-c++ switch so
//! the alternative is to define them directly in the class itself. However
//! GCC 3.4 needs the copy ctor to be publicly accessible.

#if ((__GNUC__ == 3) && (__GNUC_MINOR__ == 4)) // GCC 3.4.x

#define CORE_NOT_COPYABLE(className)		\
public:										\
	className(const className&);			\
private:									\
	className& operator=(const className&);

#else

#define CORE_NOT_COPYABLE(className)		\
private:									\
	className(const className&);			\
	className& operator=(const className&);

#endif

//namespace Core
}

#endif // CORE_NOTCOPYABLE_HPP
