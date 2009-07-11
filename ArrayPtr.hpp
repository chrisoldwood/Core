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
T** attachTo(ArrayPtr<T>& ptr);

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
	void reset(T* pPointer = nullptr);

	//! Take ownership of the pointer.
	T* detach();

private:
	//! Access the underlying pointer member.
	T** getPtrMember();

	//
	// Friends.
	//

	//! Allow attachment via an output parameter.
	friend T** attachTo<>(ArrayPtr<T>& ptr);
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
	reset(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
//! Index operator.

template <typename T>
inline T& ArrayPtr<T>::operator[](size_t nIndex)
{
	if (this->m_pPointer == nullptr)
		throw NullPtrException();

	return this->m_pPointer[nIndex];
}

////////////////////////////////////////////////////////////////////////////////
//! Index operator.

template <typename T>
inline const T& ArrayPtr<T>::operator[](size_t nIndex) const
{
	if (this->m_pPointer == nullptr)
		throw NullPtrException();

	return this->m_pPointer[nIndex];
}

////////////////////////////////////////////////////////////////////////////////
//! Change pointer ownership. Frees the current pointer and takes ownership of
//! another pointer, if provided.

template <typename T>
inline void ArrayPtr<T>::reset(T* pPointer)
{
	// Release current resource.
	delete[] this->m_pPointer;

	// Update state.
	this->m_pPointer = pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Take ownership of the pointer.

template <typename T>
inline T* ArrayPtr<T>::detach()
{
	T* pPointer = this->m_pPointer;

	this->m_pPointer = nullptr;

	return pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Access the underlying pointer member. This is used by the AttachTo() friend
//! function to access the underlying SmartPtr<T> member variable.

template <typename T>
inline T** ArrayPtr<T>::getPtrMember()
{
	return &this->m_pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Helper function to gain access to the internal member so that it can be
//! passed as an output parameter, without overloading the & operator.
//! e.g. LoadTypeLib(..., AttachTo(p)).

template <typename T>
inline T** attachTo(ArrayPtr<T>& ptr)
{
	ASSERT(*ptr.getPtrMember() == nullptr);

	return ptr.getPtrMember();
}

//namespace Core
}

#endif // CORE_ARRAYPTR_HPP
