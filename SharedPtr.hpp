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
#include "SmartPtr.hpp"

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
	explicit SharedPtr(T* ptr);

	//! Copy constructor.
	SharedPtr(const SharedPtr<T>& sharedPtr);

	//! Copy constructor for sub-types of T.
	template <typename U>
	SharedPtr(const SharedPtr<U>& sharedPtr);

	//! Destructor.
	~SharedPtr();

	//
	// Operators.
	//

	//! Assignment operator.
	SharedPtr& operator=(const SharedPtr& sharedPtr);

	//! Assignment operator for sub-types of T.
	template <typename U>
	SharedPtr& operator=(const SharedPtr<U>& sharedPtr);

	//
	// Methods.
	//

	//! Change pointer ownership.
	void reset(T* ptr = nullptr);

private:
	//
	// Members.
	//
	long*	m_refCount;		//!< The pointer reference count.

	//! Private constructor for use by cast functions.
	SharedPtr(T* ptr, long* refCount);

	//
	// Friends.
	//

	//! Allow member access for SharedPtrs of sub-types.
	template<typename U>
	friend class SharedPtr;

	//! Allow member access for the static_cast like function.
	template<typename P, typename U>
	friend SharedPtr<P> static_ptr_cast(const SharedPtr<U>& sharedPtr);

	//! Allow member access for the dynamic_cast like function.
	template<typename P, typename U>
	friend SharedPtr<P> dynamic_ptr_cast(const SharedPtr<U>& sharedPtr);
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor. Sets pointer and reference count to NULL.

template <typename T>
inline SharedPtr<T>::SharedPtr()
	: m_refCount(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Construction from a raw pointer. Takes ownership of a new pointer.

template <typename T>
inline SharedPtr<T>::SharedPtr(T* ptr)
	: m_refCount(nullptr)
{
	reset(ptr);
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor. Takes shared ownership of another pointer.

template<typename T>
inline SharedPtr<T>::SharedPtr(const SharedPtr<T>& sharedPtr)
	: SmartPtr<T>(sharedPtr.m_ptr)
	, m_refCount(sharedPtr.m_refCount)
{
	if (m_refCount != nullptr)
		Core::atomicIncrement(*m_refCount);
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor for sub-types of T. Takes shared ownership of another
//! pointer that must be a sub-type of T or be "more" const than T.

template <typename T>
template <typename U>
inline SharedPtr<T>::SharedPtr(const SharedPtr<U>& sharedPtr)
	: SmartPtr<T>(sharedPtr.m_ptr)
	, m_refCount(sharedPtr.m_refCount)
{
	if (m_refCount != nullptr)
		Core::atomicIncrement(*m_refCount);
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor. Frees the pointer if the last reference.

template <typename T>
inline SharedPtr<T>::~SharedPtr()
{
	reset(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
//! Assignment operator. Frees the current pointer if the last reference and
//! takes shared ownership of another pointer.

template <typename T>
inline SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& sharedPtr)
{
	// Ignore self-assignment.
	if (this->m_ptr != sharedPtr.m_ptr)
	{
		// Final reference?
		if ((m_refCount != nullptr) && (Core::atomicDecrement(*m_refCount) == 0))
		{
			delete this->m_ptr;
			delete this->m_refCount;
		}

		this->m_ptr = sharedPtr.m_ptr;
		this->m_refCount = sharedPtr.m_refCount;

		// Share ownership.
		if (m_refCount != nullptr)
			Core::atomicIncrement(*m_refCount);
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//! Assignment operator for sub-types of T. Frees the current pointer if the
//! last reference and takes shared ownership of another pointer that must be a
//! sub-type of T.

template <typename T>
template <typename U>
inline SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<U>& sharedPtr)
{
	// Ignore self-assignment.
	if (this->m_ptr != sharedPtr.m_ptr)
	{
		// Final reference?
		if ((m_refCount != nullptr) && (Core::atomicDecrement(*m_refCount) == 0))
		{
			delete this->m_ptr;
			delete this->m_refCount;
		}

		this->m_ptr = sharedPtr.m_ptr;
		this->m_refCount = sharedPtr.m_refCount;

		// Share ownership.
		if (m_refCount != nullptr)
			Core::atomicIncrement(*m_refCount);
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//! Change pointer ownership. Frees the current pointer if the last reference
//! and takes shared ownership of another pointer, if provided.

template <typename T>
inline void SharedPtr<T>::reset(T* ptr)
{
	T*    tmpPtr = nullptr;
	long* tmpCnt = nullptr;

	// Allocate new resources up front.
	if (ptr != nullptr)
	{
		tmpPtr = ptr;
		tmpCnt = new long(1);
	}

	// Release current resources, if final reference.
	if ((m_refCount != nullptr) && (Core::atomicDecrement(*m_refCount) == 0))
	{
		delete this->m_ptr;
		delete this->m_refCount;
	}

	// Update state.
	this->m_ptr = tmpPtr;
	this->m_refCount = tmpCnt;
}

////////////////////////////////////////////////////////////////////////////////
//! Private constructor for use by cast functions.

template <typename T>
inline SharedPtr<T>::SharedPtr(T* ptr, long* refCount)
	: SmartPtr<T>(ptr)
	, m_refCount(refCount)
{
	if (m_refCount != nullptr)
		Core::atomicIncrement(*m_refCount);
}

////////////////////////////////////////////////////////////////////////////////
//! A variant of static_cast<> that can be used to create a SharedPtr of the
//! derived ptr type from the base ptr type.

template<typename P, typename U>
inline SharedPtr<P> static_ptr_cast(const SharedPtr<U>& sharedPtr)
{
	return SharedPtr<P>(static_cast<P*>(sharedPtr.m_ptr), sharedPtr.m_refCount);
}

////////////////////////////////////////////////////////////////////////////////
//! A variant of dynamic_cast<> that can be used to create a SharedPtr of the
//! derived ptr type from the base ptr type.

template<typename P, typename U>
inline SharedPtr<P> dynamic_ptr_cast(const SharedPtr<U>& sharedPtr)
{
	P*    tmpPtr = dynamic_cast<P*>(sharedPtr.m_ptr);
	long* tmpCnt = sharedPtr.m_refCount;

	if (tmpPtr == nullptr)
		tmpCnt = nullptr;

	return SharedPtr<P>(tmpPtr, tmpCnt);
}

//namespace Core
}

#endif // CORE_SHAREDPTR_HPP
