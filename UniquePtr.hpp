////////////////////////////////////////////////////////////////////////////////
//! \file   UniquePtr.hpp
//! \brief  The UniquePtr class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_UNIQUEPTR_HPP
#define CORE_UNIQUEPTR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

// Forward declarations.
template <typename T>
class UniquePtr;

template <typename T>
T** AttachTo(UniquePtr<T>& ptr);

////////////////////////////////////////////////////////////////////////////////
//! A smart-pointer for use within a limited scope.

template <typename T>
class UniquePtr : public SmartPtr<T>
{
public:
	//! Default constructor.
	UniquePtr();

	//! Construction from a raw pointer.
	explicit UniquePtr(T* pPointer);

	//! Destructor.
	~UniquePtr();
	
	//
	// Methods.
	//

	//! Change pointer ownership.
	void Reset(T* pPointer = nullptr);

	//! Take ownership of the pointer.
	T* Detach();
	
private:
	//! Access the underlying pointer member.
	T** GetPtrMember();

	//
	// Friends.
	//

	//! Allow attachment via an output parameter.
	friend T** AttachTo<>(UniquePtr<T>& ptr);
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

template <typename T>
inline UniquePtr<T>::UniquePtr()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Construction from a raw pointer. Takes ownership of a new pointer.

template <typename T>
inline UniquePtr<T>::UniquePtr(T* pPointer)
	: SmartPtr<T>(pPointer)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

template <typename T>
inline UniquePtr<T>::~UniquePtr()
{
	Reset(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
//! Change pointer ownership. Frees the current pointer and takes ownership of
//! another pointer, if provided.

template <typename T>
inline void UniquePtr<T>::Reset(T* pPointer)
{
	// Release current resource.
	delete m_pPointer;

	// Update state.
	m_pPointer = pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Take ownership of the pointer.

template <typename T>
inline T* UniquePtr<T>::Detach()
{
	T* pPointer = m_pPointer;
	m_pPointer = nullptr;

	return pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Access the underlying pointer member. This is used by the AttachTo() friend
//! function to access the underlying SmartPtr<T> member variable.

template <typename T>
inline T** UniquePtr<T>::GetPtrMember()
{
	return &m_pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Helper function to gain access to the internal member so that it can be
//! passed as an output parameter, without overloading the & operator.
//! e.g. LoadTypeLib(..., AttachTo(p)).

template <typename T>
inline T** AttachTo(UniquePtr<T>& ptr)
{
	return ptr.GetPtrMember();
}

//namespace Core
}

#endif // CORE_UNIQUEPTR_HPP
