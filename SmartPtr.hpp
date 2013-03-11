////////////////////////////////////////////////////////////////////////////////
//! \file   SmartPtr.hpp
//! \brief  The SmartPtr class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_SMARTPTR_HPP
#define CORE_SMARTPTR_HPP

#include "NullPtrException.hpp"

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

#ifdef __GNUG__
// base class 'X' has a non-virtual destructor
#pragma GCC diagnostic ignored "-Weffc++"
#endif

////////////////////////////////////////////////////////////////////////////////
//! The base class for all smart-pointer types. This class provides the common
//! smart-pointer storage and methods for accessing it. All behaviour relating
//! to ownership must be provided by the derived classes.

template <typename T>
class SmartPtr /*: private NotCopyable*/
{
public:
	//
	// Operators.
	//

	//! Pointer dereference operator.
	const T& operator*() const;

	//! Pointer dereference operator.
	T& operator*();

	//! Pointer-to-member operator.
	const T* operator->() const;

	//! Pointer-to-member operator.
	T* operator->();

	//! Not operator.
    bool operator!() const;

	//
	// Methods.
	//

	//! Access owned pointer.
	T* get() const;

	//! Access owned pointer as a reference.
	T& getRef() const;

	//! Query if we are not owning a pointer.
	bool empty() const;

protected:
	//! Default constructor.
	SmartPtr();

	//! Construction from a raw pointer.
	explicit SmartPtr(T* ptr);

	//! Destructor.
	~SmartPtr();

	//
	// Members.
	//
	T*	m_ptr;		//!< The pointer being shadowed.

private:
	// NotCopyable.
	SmartPtr(const SmartPtr&);
	SmartPtr& operator=(const SmartPtr&);
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

template <typename T>
inline SmartPtr<T>::SmartPtr()
	: m_ptr(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Construction from a raw pointer.

template <typename T>
inline SmartPtr<T>::SmartPtr(T* ptr)
	: m_ptr(ptr)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

template <typename T>
inline SmartPtr<T>::~SmartPtr()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Pointer dereference operator. Returns the currently owned pointer.

template <typename T>
inline const T& SmartPtr<T>::operator*() const
{
	if (m_ptr == nullptr)
		throw NullPtrException();

	return *m_ptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Pointer dereference operator. Returns the currently owned pointer.

template <typename T>
inline T& SmartPtr<T>::operator*()
{
	if (m_ptr == nullptr)
		throw NullPtrException();

	return *m_ptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Pointer-to-member operator. Returns the currently owned pointer.

template <typename T>
inline const T* SmartPtr<T>::operator->() const
{
	if (m_ptr == nullptr)
		throw NullPtrException();

	return m_ptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Pointer-to-member operator. Returns the currently owned pointer.

template <typename T>
inline T* SmartPtr<T>::operator->()
{
	if (m_ptr == nullptr)
		throw NullPtrException();

	return m_ptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Not operator. Tests for a NULL pointer.

template <typename T>
bool SmartPtr<T>::operator!() const
{
	return (m_ptr == nullptr);
}

////////////////////////////////////////////////////////////////////////////////
//! Access owned pointer. Returns the currently owned pointer.

template <typename T>
inline T* SmartPtr<T>::get() const
{
	return m_ptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Access owned pointer as a reference.

template <typename T>
inline T& SmartPtr<T>::getRef() const
{
	if (m_ptr == nullptr)
		throw NullPtrException();

	return *m_ptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Query if we are not owning a pointer.

template <typename T>
inline bool SmartPtr<T>::empty() const
{
	return (m_ptr == nullptr);
}

////////////////////////////////////////////////////////////////////////////////
//! SmartPtr<T> equality operator. Compare two SmartPtr's for equality.

template <typename T, typename U>
inline bool operator==(const SmartPtr<T>& lhs, const SmartPtr<U>& rhs)
{
	return (lhs.get() == rhs.get());
}

////////////////////////////////////////////////////////////////////////////////
//! SmartPtr<T> inequality operator. Compare two SmartPtr's for inequality.

template <typename T, typename U>
inline bool operator!=(const SmartPtr<T>& lhs, const SmartPtr<U>& rhs)
{
	return (lhs.get() != rhs.get());
}

//namespace Core
}

#endif // CORE_SMARTPTR_HPP
