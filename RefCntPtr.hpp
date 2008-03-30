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

namespace Core
{

// Forward declarations.
template <typename T>
class RefCntPtr;

template <typename T>
T** AttachTo(RefCntPtr<T>& ptr);

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
	explicit RefCntPtr(T* pPointer, bool bAddRef = false);

	//! Copy constructor.
	RefCntPtr(const RefCntPtr& oPtr);

	//! Copy constructor for sub-types of T.
	template <typename U>
	RefCntPtr(const RefCntPtr<U>& oPtr);

	//! Destructor.
	~RefCntPtr();

	//
	// Operators.
	//

	//! Assignment operator.
	RefCntPtr& operator=(const RefCntPtr& oPtr);

	//! Assignment operator for sub-types of T.
	template <typename U>
	RefCntPtr& operator=(const RefCntPtr<U>& oPtr);

	//
	// Methods.
	//

	//! Change pointer ownership.
	void Reset(T* pPointer = nullptr, bool bAddRef = false);
	
	//! Take ownership of the pointer.
	T* Detach();
	
private:
	//! Access the underlying pointer member.
	T** GetPtrMember();

	//
	// Friends.
	//

	//! Allow member access for RefCntPtrs of sub-types.
	template<typename U>
	friend class RefCntPtr;

	//! Allow attachment via an output parameter.
	friend T** AttachTo<>(RefCntPtr<T>& ptr);

	//! Allow member access for the static_cast like function.
	template<typename T, typename U>
	friend RefCntPtr<T> static_ptr_cast(RefCntPtr<U>& oPointer);

	//! Allow member access for the dynamic_cast like function.
	template<typename T, typename U>
	friend RefCntPtr<T> dynamic_ptr_cast(RefCntPtr<U>& oPointer);
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
inline RefCntPtr<T>::RefCntPtr(T* pPointer, bool bAddRef)
	: SmartPtr<T>(pPointer)
{
	if ((m_pPointer != nullptr) && bAddRef)
		m_pPointer->IncRefCount();
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor.

template <typename T>
inline RefCntPtr<T>::RefCntPtr(const RefCntPtr& oPtr)
	: SmartPtr<T>(oPtr.m_pPointer)
{
	if (m_pPointer != nullptr)
		m_pPointer->IncRefCount();
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor for sub-types of T. Takes shared ownership of another
//! pointer that must be a sub-type of T..

template <typename T>
template <typename U>
inline RefCntPtr<T>::RefCntPtr(const RefCntPtr<U>& oPtr)
	: SmartPtr<T>(oPtr.m_pPointer)
{
	if (m_pPointer != nullptr)
		m_pPointer->IncRefCount();
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

template <typename T>
inline RefCntPtr<T>::~RefCntPtr()
{
	Reset();
}

////////////////////////////////////////////////////////////////////////////////
//! Assignment operator.

template <typename T>
inline RefCntPtr<T>& RefCntPtr<T>::operator=(const RefCntPtr& oPtr)
{
	// Ignore self-assignment.
	if (this->m_pPointer != oPtr.m_pPointer)
		Reset(oPtr.m_pPointer, true);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//! Assignment operator for sub-types of T. Releases teh current resource and
//! takes shared ownership of another pointer that must be a sub-type of T.

template <typename T>
template <typename U>
inline RefCntPtr<T>& RefCntPtr<T>::operator=(const RefCntPtr<U>& oPtr)
{
	// Ignore self-assignment.
	if (m_pPointer != oPtr.m_pPointer)
		Reset(oPtr.m_pPointer, true);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//! Change pointer ownership.

template <typename T>
inline void RefCntPtr<T>::Reset(T* pPointer, bool bAddRef)
{
	// Release the current resource.
	if (m_pPointer != nullptr)
	{
		m_pPointer->DecRefCount();
		m_pPointer = nullptr;
	}

	// Attach the new resource, if specified.
	m_pPointer = pPointer;

	if ((m_pPointer != nullptr) && bAddRef)
		m_pPointer->IncRefCount();
}

////////////////////////////////////////////////////////////////////////////////
//! Take ownership of the pointer.

template <typename T>
inline T* RefCntPtr<T>::Detach()
{
	T* pPointer = m_pPointer;
	m_pPointer = nullptr;

	return pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Access the underlying pointer member. This is used by the AttachTo() friend
//! function to access the underlying SmartPtr<T> member variable.

template <typename T>
inline T** RefCntPtr<T>::GetPtrMember()
{
	return &m_pPointer;
}

////////////////////////////////////////////////////////////////////////////////
//! Helper function to gain access to the internal member so that it can be
//! passed as an output parameter, without overloading the & operator.
//! e.g. LoadTypeLib(..., AttachTo(p)).

template <typename T>
inline T** AttachTo(RefCntPtr<T>& ptr)
{
	return ptr.GetPtrMember();
}

////////////////////////////////////////////////////////////////////////////////
//! A variant of static_cast<> that can be used to create a RefCntPtr of the
//! derived ptr type from the base ptr type.

template<typename T, typename U>
inline RefCntPtr<T> static_ptr_cast(RefCntPtr<U>& oPtr)
{
	return RefCntPtr<T>(static_cast<T*>(oPtr.m_pPointer), true);
}

////////////////////////////////////////////////////////////////////////////////
//! A variant of dynamic_cast<> that can be used to create a RefCntPtr of the
//! derived ptr type from the base ptr type.

template<typename T, typename U>
inline RefCntPtr<T> dynamic_ptr_cast(RefCntPtr<U>& oPtr)
{
	return RefCntPtr<T>(dynamic_cast<T*>(oPtr.m_pPointer), true);
}

//namespace Core
}

#endif // CORE_REFCNTPTR_HPP
