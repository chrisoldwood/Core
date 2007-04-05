////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   SharedPtr.hpp
//! \brief  The SharedPtr template class declaration.

// Check for previous inclusion
#ifndef SHAREDPTR_HPP
#define SHAREDPTR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! A reference counted smart pointer.

template <typename T>
class SharedPtr
{
public:
	//! Default constructor.
	SharedPtr();

	//! Construction from a raw pointer.
	SharedPtr(T* pPointer);

	//! Copy constructor.
	SharedPtr(const SharedPtr<T>& oPointer);

	//! Destructor.
	~SharedPtr();
	
	//
	// Operators.
	//

	//! Assignment operator.
	SharedPtr& operator=(const SharedPtr<T>& oPointer);

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

	//! Change pointer ownership.
	void Reset(T* pPointer = nullptr);

private:
	//
	// Members.
	//
	T*		m_pPointer;		//!< The pointer being shared.
	ulong*	m_pRefCnt;		//!< The pointer reference count.
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor. Sets pointer and reference count to NULL.

template <typename T>
inline SharedPtr<T>::SharedPtr()
	: m_pPointer(nullptr)
	, m_pRefCnt(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Construction from a raw pointer. Takes ownership of a new pointer.

template <typename T>
inline SharedPtr<T>::SharedPtr(T* pPointer)
	: m_pPointer(nullptr)
	, m_pRefCnt(nullptr)
{
	Reset(pPointer);
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor. Takes shared ownership of another pointer.

template <typename T>
inline SharedPtr<T>::SharedPtr(const SharedPtr<T>& oPointer)
	: m_pPointer(oPointer.m_pPointer)
	, m_pRefCnt(oPointer.m_pRefCnt)
{
	if (m_pRefCnt != nullptr)
		++(*m_pRefCnt);
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
		if ((m_pRefCnt != nullptr) && (--(*m_pRefCnt) == 0))
		{
			delete m_pPointer;
			delete m_pRefCnt;
		}

		m_pPointer = oPointer.m_pPointer;
		m_pRefCnt  = oPointer.m_pRefCnt;

		// Share ownership.
		if (m_pRefCnt != nullptr)
			++(*m_pRefCnt);
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//! Pointer dereference operator. Returns the currently shared pointer.

template <typename T>
inline T& SharedPtr<T>::operator*() const
{
	return *m_pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Pointer-to-member operator. Returns the currently shared pointer.

template <typename T>
inline const T* SharedPtr<T>::operator->() const
{
	return m_pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Pointer-to-member operator. Returns the currently shared pointer.

template <typename T>
inline T* SharedPtr<T>::operator->()
{
	return m_pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Not operator. Tests for a NULL pointer.

template <typename T>
bool SharedPtr<T>::operator!() const
{
	return (m_pPointer == nullptr);
}

////////////////////////////////////////////////////////////////////////////////
//! Access owned pointer. Returns the currently shared pointer.

template <typename T>
inline T* SharedPtr<T>::Get() const
{
	return m_pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Access owned pointer as a reference.

template <typename T>
inline T& SharedPtr<T>::GetRef() const
{
	return *m_pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Change pointer ownership. Frees the current pointer if the last reference
//! and takes shared ownership of another pointer, if provided.

template <typename T>
inline void SharedPtr<T>::Reset(T* pPointer)
{
	// Final reference?
	if ((m_pRefCnt != nullptr) && (--(*m_pRefCnt) == 0))
	{
		delete m_pPointer;
		delete m_pRefCnt;
	}

	// Take ownership.
	if (pPointer != nullptr)
	{
		m_pPointer = pPointer;
		m_pRefCnt  = new ulong(1);
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

#endif // SHAREDPTR_HPP
