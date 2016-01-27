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

#include "BadLogicException.hpp"

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
	explicit ArrayPtr(T* ptr);

	//! Destructor.
	~ArrayPtr();

	//
	// Operators.
	//

	//! Index operator.
	T& operator[](size_t index);

	//! Index operator.
	const T& operator[](size_t index) const;

	//
	// Methods.
	//

	//! Change pointer ownership.
	void reset(T* ptr = nullptr);

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
inline ArrayPtr<T>::ArrayPtr(T* ptr)
	: SmartPtr<T>(ptr)
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
inline T& ArrayPtr<T>::operator[](size_t index)
{
	if (this->m_ptr == nullptr)
		throw NullPtrException();

	return this->m_ptr[index];
}

////////////////////////////////////////////////////////////////////////////////
//! Index operator.

template <typename T>
inline const T& ArrayPtr<T>::operator[](size_t index) const
{
	if (this->m_ptr == nullptr)
		throw NullPtrException();

	return this->m_ptr[index];
}

////////////////////////////////////////////////////////////////////////////////
//! Change pointer ownership. Frees the current pointer and takes ownership of
//! another pointer, if provided.

template <typename T>
inline void ArrayPtr<T>::reset(T* ptr)
{
	// Release current resource.
	delete[] this->m_ptr;

	// Update state.
	this->m_ptr = ptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Take ownership of the pointer.

template <typename T>
inline T* ArrayPtr<T>::detach()
{
	T* ptr = this->m_ptr;

	this->m_ptr = nullptr;

	return ptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Access the underlying pointer member. This is used by the attachTo() friend
//! function to access the underlying SmartPtr<T> member variable.

template <typename T>
inline T** ArrayPtr<T>::getPtrMember()
{
	return &this->m_ptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Helper function to gain access to the internal member so that it can be
//! passed as an output parameter, without overloading the & operator.
//! e.g. LoadTypeLib(..., attachTo(p)).

template <typename T>
inline T** attachTo(ArrayPtr<T>& ptr)
{
	if (!ptr.empty())
		throw BadLogicException(TXT("Cannot attach to a non-empty smart pointer"));

	return ptr.getPtrMember();
}

//namespace Core
}

#endif // CORE_ARRAYPTR_HPP
