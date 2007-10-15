////////////////////////////////////////////////////////////////////////////////
//! \file   SmartPtr.hpp
//! \brief  The SmartPtr class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_SMARTPTR_HPP
#define CORE_SMARTPTR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! The base class for all smart-pointer types. This class provides the common
//! smart-pointer storage and methods for accessing it. All behaviour relating
//! to ownership must be provided by the derived classes.

template <typename T>
class SmartPtr : private NotCopyable
{
public:
	//
	// Operators.
	//

	//! Pointer dereference operator.
	T& operator*() const;

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
	T* Get() const;

	//! Access owned pointer as a reference.
	T& GetRef() const;

protected:
	//! Default constructor.
	SmartPtr();

	//! Construction from a raw pointer.
	explicit SmartPtr(T* pPointer);

	//! Destructor.
	~SmartPtr();
	
	//
	// Members.
	//
	T*	m_pPointer;		//!< The pointer being shadowed.
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

template <typename T>
inline SmartPtr<T>::SmartPtr()
	: m_pPointer(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Construction from a raw pointer.

template <typename T>
inline SmartPtr<T>::SmartPtr(T* pPointer)
	: m_pPointer(pPointer)
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
inline T& SmartPtr<T>::operator*() const
{
	ASSERT(m_pPointer != nullptr);

	return *m_pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Pointer-to-member operator. Returns the currently owned pointer.

template <typename T>
inline const T* SmartPtr<T>::operator->() const
{
	ASSERT(m_pPointer != nullptr);

	return m_pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Pointer-to-member operator. Returns the currently owned pointer.

template <typename T>
inline T* SmartPtr<T>::operator->()
{
	ASSERT(m_pPointer != nullptr);

	return m_pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Not operator. Tests for a NULL pointer.

template <typename T>
bool SmartPtr<T>::operator!() const
{
	return (m_pPointer == nullptr);
}

////////////////////////////////////////////////////////////////////////////////
//! Access owned pointer. Returns the currently owned pointer.

template <typename T>
inline T* SmartPtr<T>::Get() const
{
	return m_pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Access owned pointer as a reference.

template <typename T>
inline T& SmartPtr<T>::GetRef() const
{
	return *m_pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! SmartPtr<T> equality operator. Compare two SmartPtr's for equality.

template <typename T>
inline bool operator==(const SmartPtr<T>& oLHS, const SmartPtr<T>& oRHS)
{
	return (oLHS.Get() == oRHS.Get());
}

////////////////////////////////////////////////////////////////////////////////
//! SmartPtr<T> inequality operator. Compare two SmartPtr's for inequality.

template <typename T>
inline bool operator!=(const SmartPtr<T>& oLHS, const SmartPtr<T>& oRHS)
{
	return (oLHS.Get() != oRHS.Get());
}

//namespace Core
}

#endif // CORE_SMARTPTR_HPP
