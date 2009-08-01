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

//namespace Core
}

#endif // CORE_NOTCOPYABLE_HPP
