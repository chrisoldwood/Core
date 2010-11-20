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

	//! Decrese the reference count by one
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
	long	m_lRefCount;	//!< The reference count.

	// NotCopyable.
	RefCounted(const RefCounted&);
	RefCounted& operator=(const RefCounted&);
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

////////////////////////////////////////////////////////////////////////////////
//! Get the reference count.

inline long RefCounted::refCount() const
{
	return m_lRefCount;
}

////////////////////////////////////////////////////////////////////////////////
//! Increase the reference count by one.

inline long RefCounted::incRefCount()
{
	ASSERT(m_lRefCount > 0);

	long lRefCount = Core::atomicIncrement(m_lRefCount);

	return lRefCount;
}

////////////////////////////////////////////////////////////////////////////////
//! Decrese the reference count by one

inline long RefCounted::decRefCount()
{
	ASSERT(m_lRefCount > 0);

	long lRefCount = Core::atomicDecrement(m_lRefCount);

	if (lRefCount == 0)
		delete this;

	return lRefCount;
}

//namespace Core
}

#endif // CORE_REFCOUNTED_HPP
