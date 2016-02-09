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

class RefCounted /*: private NotCopyable*/
{
public:
	//
	// Properties.
	//

	//! Get the reference count.
	long refCount() const;

	//
	// Methods.
	//

	//! Increase the reference count by one.
	long incRefCount();

	//! Decrease the reference count by one
	long decRefCount();

protected:
	//! Default constructor.
	RefCounted();

	//! Destructor.
	virtual ~RefCounted();

private:
	//
	// Members.
	//
	long	m_refCount;	//!< The reference count.

	// NotCopyable.
	RefCounted(const RefCounted&);
	RefCounted& operator=(const RefCounted&);
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

inline RefCounted::RefCounted()
	: m_refCount(1)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline RefCounted::~RefCounted()
{
	ASSERT(m_refCount == 0);
}

////////////////////////////////////////////////////////////////////////////////
//! Get the reference count.

inline long RefCounted::refCount() const
{
	return m_refCount;
}

////////////////////////////////////////////////////////////////////////////////
//! Increase the reference count by one.

inline long RefCounted::incRefCount()
{
	ASSERT(m_refCount > 0);

	long newCount = Core::atomicIncrement(m_refCount);

	return newCount;
}

////////////////////////////////////////////////////////////////////////////////
//! Decrease the reference count by one

inline long RefCounted::decRefCount()
{
	ASSERT(m_refCount > 0);

	long newCount = Core::atomicDecrement(m_refCount);

	if (newCount == 0)
		delete this;

	return newCount;
}

//namespace Core
}

#endif // CORE_REFCOUNTED_HPP
