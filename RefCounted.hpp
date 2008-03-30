////////////////////////////////////////////////////////////////////////////////
//! \file   RefCounted.hpp
//! \brief  The RefCounted class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_REFCOUNTED_HPP
#define CORE_REFCOUNTED_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Interlocked.hpp"

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! The class used to provide internal reference counting.

class RefCounted : private NotCopyable
{
public:
	//
	// Properties.
	//

	//! Get the reference count.
	long RefCount() const;

	//
	// Methods.
	//

	//! Increase the reference count by one.
	long IncRefCount();

	//! Decrese the reference count by one
	long DecRefCount();

protected:
	//! Default constructor.
	RefCounted();

	//! Destructor.
	virtual ~RefCounted();
	
private:
	//
	// Members.
	//
	long	m_lRefCount;	//!< The reference count.
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

inline RefCounted::RefCounted()
	: m_lRefCount(1)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline RefCounted::~RefCounted()
{
	ASSERT(m_lRefCount == 0);
}

//! Get the reference count.
inline long RefCounted::RefCount() const
{
	return m_lRefCount;
}

//! Increase the reference count by one.
inline long RefCounted::IncRefCount()
{
	ASSERT(m_lRefCount > 0);

	long lRefCount = _InterlockedIncrement(&m_lRefCount);

	return lRefCount;
}

//! Decrese the reference count by one
inline long RefCounted::DecRefCount()
{
	ASSERT(m_lRefCount > 0);

	long lRefCount = _InterlockedDecrement(&m_lRefCount);

	if (lRefCount == 0)
		delete this;

	return lRefCount;
}

//namespace Core
}

#endif // CORE_REFCOUNTED_HPP
