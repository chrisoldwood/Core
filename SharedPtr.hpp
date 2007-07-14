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

	//! Destructor.
	~SharedPtr();
	
	//
	// Operators.
	//

	//! Assignment operator.
	SharedPtr& operator=(const SharedPtr<T>& oPointer);

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

template <typename T>
inline SharedPtr<T>::SharedPtr(const SharedPtr<T>& oPointer)
	: SmartPtr<T>(oPointer.m_pPointer)
	, m_pRefCnt(oPointer.m_pRefCnt)
{
	if (m_pRefCnt != nullptr)
		::InterlockedIncrement(m_pRefCnt);
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
inline SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& oPointer)
{
	// Check for self-assignment.
	if (this != &oPointer)
	{
		// Final reference?
		if ((m_pRefCnt != nullptr) && (::InterlockedDecrement(m_pRefCnt) == 0))
		{
			delete m_pPointer;
			delete m_pRefCnt;
		}

		m_pPointer = oPointer.m_pPointer;
		m_pRefCnt  = oPointer.m_pRefCnt;

		// Share ownership.
		if (m_pRefCnt != nullptr)
			::InterlockedIncrement(m_pRefCnt);
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//! Change pointer ownership. Frees the current pointer if the last reference
//! and takes shared ownership of another pointer, if provided.

template <typename T>
inline void SharedPtr<T>::Reset(T* pPointer)
{
	// Final reference?
	if ((m_pRefCnt != nullptr) && (::InterlockedDecrement(m_pRefCnt) == 0))
	{
		delete m_pPointer;
		delete m_pRefCnt;
	}

	// Take ownership.
	if (pPointer != nullptr)
	{
		m_pPointer = pPointer;
		m_pRefCnt  = new long(1);
	}
	// Reset members.
	else
	{
		m_pPointer = nullptr;
		m_pRefCnt  = nullptr;
	}
}

////////////////////////////////////////////////////////////////////////////////
//! SharedPtr<T> equality operator. Compare two SharedPtr's for equality.

template <typename T>
inline bool operator==(const SharedPtr<T>& oLHS, const SharedPtr<T>& oRHS)
{
	return (oLHS.Get() == oRHS.Get());
}

////////////////////////////////////////////////////////////////////////////////
//! SharedPtr<T> inequality operator. Compare two SharedPtr's for inequality.

template <typename T>
inline bool operator!=(const SharedPtr<T>& oLHS, const SharedPtr<T>& oRHS)
{
	return (oLHS.Get() != oRHS.Get());
}

//namespace Core
}

#endif // CORE_SHAREDPTR_HPP
