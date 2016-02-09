////////////////////////////////////////////////////////////////////////////////
//! \file   RefCntPtr.hpp
//! \brief  The RefCntPtr class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_REFCNTPTR_HPP
#define CORE_REFCNTPTR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "RefCounted.hpp"
#include "BadLogicException.hpp"

namespace Core
{

// Forward declarations.
template <typename T>
class RefCntPtr;

template <typename T>
T** attachTo(RefCntPtr<T>& ptr);

////////////////////////////////////////////////////////////////////////////////
//! The smart-pointer type for use with types that contain their own reference
//! counting mechanism, e.g. Core::RefCounted.

template <typename T>
class RefCntPtr : public SmartPtr<T>
{
public:
	//! Default constructor.
	RefCntPtr();

	//! Construction from an existing pointer.
	explicit RefCntPtr(T* ptr, bool addRef = false);

	//! Copy constructor.
	RefCntPtr(const RefCntPtr& refCnfPtr);

	//! Copy constructor for sub-types of T.
	template <typename U>
	RefCntPtr(const RefCntPtr<U>& refCnfPtr);

	//! Destructor.
	~RefCntPtr();

	//
	// Operators.
	//

	//! Assignment operator.
	RefCntPtr& operator=(const RefCntPtr& refCnfPtr);

	//! Assignment operator for sub-types of T.
	template <typename U>
	RefCntPtr& operator=(const RefCntPtr<U>& refCnfPtr);

	//
	// Methods.
	//

	//! Change pointer ownership.
	void reset(T* ptr = nullptr, bool addRef = false);

	//! Take ownership of the pointer.
	T* detach();

private:
	//! Access the underlying pointer member.
	T** getPtrMember();

	//
	// Friends.
	//

	//! Allow member access for RefCntPtrs of sub-types.
	template<typename U>
	friend class RefCntPtr;

	//! Allow attachment via an output parameter.
	friend T** attachTo<>(RefCntPtr<T>& ptr);

	//! Allow member access for the static_cast like function.
	template<typename P, typename U>
	friend RefCntPtr<P> static_ptr_cast(const RefCntPtr<U>& refCnfPtr);

	//! Allow member access for the dynamic_cast like function.
	template<typename P, typename U>
	friend RefCntPtr<P> dynamic_ptr_cast(const RefCntPtr<U>& refCnfPtr);
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

template <typename T>
inline RefCntPtr<T>::RefCntPtr()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Construction from an existing pointer.

template <typename T>
inline RefCntPtr<T>::RefCntPtr(T* ptr, bool addRef)
	: SmartPtr<T>(ptr)
{
	if ((this->m_ptr != nullptr) && addRef)
		this->m_ptr->incRefCount();
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor.

template <typename T>
inline RefCntPtr<T>::RefCntPtr(const RefCntPtr& refCnfPtr)
	: SmartPtr<T>(refCnfPtr.m_ptr)
{
	if (this->m_ptr != nullptr)
		this->m_ptr->incRefCount();
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor for sub-types of T. Takes shared ownership of another
//! pointer that must be a sub-type of T..

template <typename T>
template <typename U>
inline RefCntPtr<T>::RefCntPtr(const RefCntPtr<U>& refCnfPtr)
	: SmartPtr<T>(refCnfPtr.m_ptr)
{
	if (this->m_ptr != nullptr)
		this->m_ptr->incRefCount();
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

template <typename T>
inline RefCntPtr<T>::~RefCntPtr()
{
	reset();
}

////////////////////////////////////////////////////////////////////////////////
//! Assignment operator.

template <typename T>
inline RefCntPtr<T>& RefCntPtr<T>::operator=(const RefCntPtr& refCnfPtr)
{
	// Ignore self-assignment.
	if (this->m_ptr != refCnfPtr.m_ptr)
		reset(refCnfPtr.m_ptr, true);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//! Assignment operator for sub-types of T. Releases the current resource and
//! takes shared ownership of another pointer that must be a sub-type of T.

template <typename T>
template <typename U>
inline RefCntPtr<T>& RefCntPtr<T>::operator=(const RefCntPtr<U>& refCnfPtr)
{
	// Ignore self-assignment.
	if (this->m_ptr != refCnfPtr.m_ptr)
		reset(refCnfPtr.m_ptr, true);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//! Change pointer ownership.

template <typename T>
inline void RefCntPtr<T>::reset(T* ptr, bool addRef)
{
	// Release the current resource.
	if (this->m_ptr != nullptr)
	{
		this->m_ptr->decRefCount();
		this->m_ptr = nullptr;
	}

	// Attach the new resource, if specified.
	this->m_ptr = ptr;

	if ((this->m_ptr != nullptr) && addRef)
		this->m_ptr->incRefCount();
}

////////////////////////////////////////////////////////////////////////////////
//! Take ownership of the pointer.

template <typename T>
inline T* RefCntPtr<T>::detach()
{
	T* ptr = this->m_ptr;

	this->m_ptr = nullptr;

	return ptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Access the underlying pointer member. This is used by the attachTo() friend
//! function to access the underlying SmartPtr<T> member variable.

template <typename T>
inline T** RefCntPtr<T>::getPtrMember()
{
	return &this->m_ptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Helper function to gain access to the internal member so that it can be
//! passed as an output parameter, without overloading the & operator.
//! e.g. LoadTypeLib(..., attachTo(p)).

template <typename T>
T** attachTo(RefCntPtr<T>& ptr)
{
	if (!ptr.empty())
		throw BadLogicException(TXT("Cannot attach to a non-empty smart pointer"));

	return ptr.getPtrMember();
}

////////////////////////////////////////////////////////////////////////////////
//! A variant of static_cast<> that can be used to create a RefCntPtr of the
//! derived ptr type from the base ptr type.

template<typename P, typename U>
inline RefCntPtr<P> static_ptr_cast(const RefCntPtr<U>& refCnfPtr)
{
	return RefCntPtr<P>(static_cast<P*>(refCnfPtr.m_ptr), true);
}

////////////////////////////////////////////////////////////////////////////////
//! A variant of dynamic_cast<> that can be used to create a RefCntPtr of the
//! derived ptr type from the base ptr type.

template<typename P, typename U>
RefCntPtr<P> dynamic_ptr_cast(const RefCntPtr<U>& refCnfPtr)
{
	return RefCntPtr<P>(dynamic_cast<P*>(refCnfPtr.m_ptr), true);
}

//namespace Core
}

#endif // CORE_REFCNTPTR_HPP
