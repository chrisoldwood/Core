////////////////////////////////////////////////////////////////////////////////
//! \file   ArrayPtr.hpp
//! \brief  The ArrayPtr class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_ARRAYPTR_HPP
#define CORE_ARRAYPTR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

// Forward declarations.
template <typename T>
class ArrayPtr;

template <typename T>
T** AttachTo(ArrayPtr<T>& ptr);

////////////////////////////////////////////////////////////////////////////////
//! An array smart-pointer for use within a limited scope.

template <typename T>
class ArrayPtr : public SmartPtr<T>
{
public:
	//! Default constructor.
	ArrayPtr();

	//! Construction from a raw pointer.
	explicit ArrayPtr(T* pPointer);

	//! Destructor.
	~ArrayPtr();
	
	//
	// Operators.
	//

	//! Index operator.
	T& operator[](size_t nIndex);

	//! Index operator.
	const T& operator[](size_t nIndex) const;

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
	friend T** AttachTo<>(ArrayPtr<T>& ptr);
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

template <typename T>
inline ArrayPtr<T>::ArrayPtr()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Construction from a raw pointer. Takes ownership of a new pointer.

template <typename T>
inline ArrayPtr<T>::ArrayPtr(T* pPointer)
	: SmartPtr<T>(pPointer)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

template <typename T>
inline ArrayPtr<T>::~ArrayPtr()
{
	Reset(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
//! Index operator.

template <typename T>
inline T& ArrayPtr<T>::operator[](size_t nIndex)
{
	if (m_pPointer == nullptr)
		throw NullPtrException();

	return m_pPointer[nIndex];
}

////////////////////////////////////////////////////////////////////////////////
//! Index operator.

template <typename T>
inline const T& ArrayPtr<T>::operator[](size_t nIndex) const
{
	if (m_pPointer == nullptr)
		throw NullPtrException();

	return m_pPointer[nIndex];
}

////////////////////////////////////////////////////////////////////////////////
//! Change pointer ownership. Frees the current pointer and takes ownership of
//! another pointer, if provided.

template <typename T>
inline void ArrayPtr<T>::Reset(T* pPointer)
{
	// Release current resource.
	delete m_pPointer;

	// Update state.
	m_pPointer = pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Take ownership of the pointer.

template <typename T>
inline T* ArrayPtr<T>::Detach()
{
	T* pPointer = m_pPointer;
	m_pPointer = nullptr;

	return pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Access the underlying pointer member. This is used by the AttachTo() friend
//! function to access the underlying SmartPtr<T> member variable.

template <typename T>
inline T** ArrayPtr<T>::GetPtrMember()
{
	return &m_pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Helper function to gain access to the internal member so that it can be
//! passed as an output parameter, without overloading the & operator.
//! e.g. LoadTypeLib(..., AttachTo(p)).

template <typename T>
inline T** AttachTo(ArrayPtr<T>& ptr)
{
	return ptr.GetPtrMember();
}

//namespace Core
}

#endif // CORE_ARRAYPTR_HPP
