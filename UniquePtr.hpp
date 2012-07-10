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

#include "BadLogicException.hpp"

namespace Core
{

// Forward declarations.
template <typename T>
class UniquePtr;

template <typename T>
T** attachTo(UniquePtr<T>& ptr);

////////////////////////////////////////////////////////////////////////////////
//! A smart-pointer for use within a limited scope.

template <typename T>
class UniquePtr : public SmartPtr<T>
{
public:
	//! Default constructor.
	UniquePtr();

	//! Construction from a raw pointer.
	explicit UniquePtr(T* ptr);

	//! Destructor.
	~UniquePtr();
	
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
	friend T** attachTo<>(UniquePtr<T>& ptr);
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
inline UniquePtr<T>::UniquePtr(T* ptr)
	: SmartPtr<T>(ptr)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

template <typename T>
inline UniquePtr<T>::~UniquePtr()
{
	reset(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
//! Change pointer ownership. Frees the current pointer and takes ownership of
//! another pointer, if provided.

template <typename T>
inline void UniquePtr<T>::reset(T* ptr)
{
	// Release current resource.
	delete this->m_ptr;

	// Update state.
	this->m_ptr = ptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Take ownership of the pointer.

template <typename T>
inline T* UniquePtr<T>::detach()
{
	T* ptr = this->m_ptr;

	this->m_ptr = nullptr;

	return ptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Access the underlying pointer member. This is used by the attachTo() friend
//! function to access the underlying SmartPtr<T> member variable.

template <typename T>
inline T** UniquePtr<T>::getPtrMember()
{
	return &this->m_ptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Helper function to gain access to the internal member so that it can be
//! passed as an output parameter, without overloading the & operator.
//! e.g. LoadTypeLib(..., attachTo(p)).

template <typename T>
inline T** attachTo(UniquePtr<T>& ptr)
{
	if (!ptr.empty())
		throw BadLogicException(TXT("Cannot attach to a non-empty smart pointer"));

	return ptr.getPtrMember();
}

//namespace Core
}

#endif // CORE_UNIQUEPTR_HPP
