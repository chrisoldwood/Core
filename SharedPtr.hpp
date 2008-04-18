////////////////////////////////////////////////////////////////////////////////
//! \file   SharedPtr.hpp
//! \brief  The SharedPtr template class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_SHAREDPTR_HPP
#define CORE_SHAREDPTR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Interlocked.hpp"

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! A reference counted smart pointer.

template <typename T>
class SharedPtr : public SmartPtr<T>
{
public:
	//! Default constructor.
	SharedPtr();

	//! Construction from a raw pointer.
	explicit SharedPtr(T* pPointer);

	//! Copy constructor.
	SharedPtr(const SharedPtr<T>& oPointer);

	//! Copy constructor for sub-types of T.
	template <typename U>
	SharedPtr(const SharedPtr<U>& oPointer);

	//! Destructor.
	~SharedPtr();
	
	//
	// Operators.
	//

	//! Assignment operator.
	SharedPtr& operator=(const SharedPtr& oPointer);

	//! Assignment operator for sub-types of T.
	template <typename U>
	SharedPtr& operator=(const SharedPtr<U>& oPointer);

	//
	// Methods.
	//

	//! Change pointer ownership.
	void Reset(T* pPointer = nullptr);

private:
	//
	// Members.
	//
	long*	m_pRefCnt;		//!< The pointer reference count.

	//! Private constructor for use by cast functions.
	SharedPtr(T* pPointer, long* pRefCnt);

	//
	// Friends.
	//

	//! Allow member access for SharedPtrs of sub-types.
	template<typename U>
	friend class SharedPtr;

	//! Allow member access for the static_cast like function.
	template<typename T, typename U>
	friend SharedPtr<T> static_ptr_cast(const SharedPtr<U>& oPointer);

	//! Allow member access for the dynamic_cast like function.
	template<typename T, typename U>
	friend SharedPtr<T> dynamic_ptr_cast(const SharedPtr<U>& oPointer);
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor. Sets pointer and reference count to NULL.

template <typename T>
inline SharedPtr<T>::SharedPtr()
	: m_pRefCnt(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Construction from a raw pointer. Takes ownership of a new pointer.

template <typename T>
inline SharedPtr<T>::SharedPtr(T* pPointer)
	: m_pRefCnt(nullptr)
{
	Reset(pPointer);
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor. Takes shared ownership of another pointer.

template<typename T>
inline SharedPtr<T>::SharedPtr(const SharedPtr<T>& oPointer)
	: SmartPtr<T>(oPointer.m_pPointer)
	, m_pRefCnt(oPointer.m_pRefCnt)
{
	if (m_pRefCnt != nullptr)
		_InterlockedIncrement(m_pRefCnt);
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor for sub-types of T. Takes shared ownership of another
//! pointer that must be a sub-type of T..

template <typename T>
template <typename U>
inline SharedPtr<T>::SharedPtr(const SharedPtr<U>& oPointer)
	: SmartPtr<T>(oPointer.m_pPointer)
	, m_pRefCnt(oPointer.m_pRefCnt)
{
	if (m_pRefCnt != nullptr)
		_InterlockedIncrement(m_pRefCnt);
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor. Frees the pointer if the last reference.

template <typename T>
inline SharedPtr<T>::~SharedPtr()
{
	Reset(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
//! Assignment operator. Frees the current pointer if the last reference and
//! takes shared ownership of another pointer.

template <typename T>
inline SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& oPointer)
{
	// Ignore self-assignment.
	if (m_pPointer != oPointer.m_pPointer)
	{
		// Final reference?
		if ((m_pRefCnt != nullptr) && (_InterlockedDecrement(m_pRefCnt) == 0))
		{
			delete m_pPointer;
			delete m_pRefCnt;
		}

		m_pPointer = oPointer.m_pPointer;
		m_pRefCnt  = oPointer.m_pRefCnt;

		// Share ownership.
		if (m_pRefCnt != nullptr)
			_InterlockedIncrement(m_pRefCnt);
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//! Assignment operator for sub-types of T. Frees the current pointer if the
//! last reference and takes shared ownership of another pointer that must be a
//! sub-type of T.

template <typename T>
template <typename U>
inline SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<U>& oPointer)
{
	// Ignore self-assignment.
	if (m_pPointer != oPointer.m_pPointer)
	{
		// Final reference?
		if ((m_pRefCnt != nullptr) && (_InterlockedDecrement(m_pRefCnt) == 0))
		{
			delete m_pPointer;
			delete m_pRefCnt;
		}

		m_pPointer = oPointer.m_pPointer;
		m_pRefCnt  = oPointer.m_pRefCnt;

		// Share ownership.
		if (m_pRefCnt != nullptr)
			_InterlockedIncrement(m_pRefCnt);
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//! Change pointer ownership. Frees the current pointer if the last reference
//! and takes shared ownership of another pointer, if provided.

template <typename T>
inline void SharedPtr<T>::Reset(T* pPointer)
{
	T*    pTmpPtr = nullptr;
	long* pTmpCnt = nullptr;

	// Allocate new resources up front.
	if (pPointer != nullptr)
	{
		pTmpPtr = pPointer;
		pTmpCnt = new long(1);
	}

	// Release current resources, if final reference.
	if ((m_pRefCnt != nullptr) && (_InterlockedDecrement(m_pRefCnt) == 0))
	{
		delete m_pPointer;
		delete m_pRefCnt;
	}

	// Update state.
	m_pPointer = pTmpPtr;
	m_pRefCnt  = pTmpCnt;
}

////////////////////////////////////////////////////////////////////////////////
//! Private constructor for use by cast functions.

template <typename T>
inline SharedPtr<T>::SharedPtr(T* pPointer, long* pRefCnt)
	: SmartPtr<T>(pPointer)
	, m_pRefCnt(pRefCnt)
{
	if (m_pRefCnt != nullptr)
		_InterlockedIncrement(m_pRefCnt);
}

////////////////////////////////////////////////////////////////////////////////
//! A variant of static_cast<> that can be used to create a SharedPtr of the
//! derived ptr type from the base ptr type.

template<typename T, typename U>
inline SharedPtr<T> static_ptr_cast(const SharedPtr<U>& oPointer)
{
	return SharedPtr<T>(static_cast<T*>(oPointer.m_pPointer), oPointer.m_pRefCnt);
}

////////////////////////////////////////////////////////////////////////////////
//! A variant of dynamic_cast<> that can be used to create a SharedPtr of the
//! derived ptr type from the base ptr type.

template<typename T, typename U>
inline SharedPtr<T> dynamic_ptr_cast(const SharedPtr<U>& oPointer)
{
	T*    pTmpPtr = dynamic_cast<T*>(oPointer.m_pPointer);
	long* pTmpCnt = oPointer.m_pRefCnt;

	if (pTmpPtr == nullptr)
		pTmpCnt = nullptr;

	return SharedPtr<T>(pTmpPtr, pTmpCnt);
}

//namespace Core
}

#endif // CORE_SHAREDPTR_HPP
