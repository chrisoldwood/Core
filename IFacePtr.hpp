////////////////////////////////////////////////////////////////////////////////
//! \file   IFacePtr.hpp
//! \brief  The IFacePtr class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_IFACEPTR_HPP
#define CORE_IFACEPTR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

// Forward declarations.
template <typename T>
class IFacePtr;

template <typename T>
T** AttachTo(IFacePtr<T>& ptr);

////////////////////////////////////////////////////////////////////////////////
//! A smart-pointer type for use with COM interfaces.

template <typename T>
class IFacePtr
{
public:
	//! Default constructor.
	IFacePtr();

	//! Construction from an existing interface pointer.
	explicit IFacePtr(T* pInterface, bool bAddRef = false);

	//! Copy constructor.
	IFacePtr(const IFacePtr<T>& oPtr);

	//! Destructor.
	~IFacePtr();

	//
	// Operators.
	//

	//! Assignment operator.
	IFacePtr& operator=(const IFacePtr<T>& oPtr);

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

	//! Access owned interface.
	T* Get() const;

	//! Access owned pointer as a reference.
	T& GetRef() const;

	//! Release the interface.
	void Release();
	
private:
	//
	// Members.
	//
	T*	m_pInterface;		//!< The interface.

	//! Allow attachment via an output parameter.
	friend T** AttachTo<>(IFacePtr<T>& ptr);
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

template <typename T>
inline IFacePtr<T>::IFacePtr()
	: m_pInterface(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Construction from an existing interface pointer.

template <typename T>
inline IFacePtr<T>::IFacePtr(T* pInterface, bool bAddRef)
	: m_pInterface(pInterface)
{
	if (bAddRef)
	{
		ASSERT(m_pInterface != nullptr);

		m_pInterface->AddRef();
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor.

template <typename T>
inline IFacePtr<T>::IFacePtr(const IFacePtr<T>& oPtr)
	: m_pInterface(oPtr.m_pInterface)
{
	if (m_pInterface != nullptr)
		m_pInterface->AddRef();
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

template <typename T>
inline IFacePtr<T>::~IFacePtr()
{
	Release();
}

////////////////////////////////////////////////////////////////////////////////
//! Assignment operator.

template <typename T>
inline IFacePtr<T>& IFacePtr<T>::operator=(const IFacePtr<T>& oPtr)
{
	// Check for self-assignment.
	if ( (this != &oPtr) && (this->m_pInterface != oPtr.m_pInterface) )
	{
		Release();

		m_pInterface = oPtr.m_pInterface;

		if (m_pInterface != nullptr)
			m_pInterface->AddRef();
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//! Pointer dereference operator. Returns the currently owned interface.

template <typename T>
inline T& IFacePtr<T>::operator*() const
{
	return *m_pInterface;
}

////////////////////////////////////////////////////////////////////////////////
//! Pointer-to-member operator. Returns the currently owned interface.

template <typename T>
inline const T* IFacePtr<T>::operator->() const
{
	return m_pInterface;
}

////////////////////////////////////////////////////////////////////////////////
//! Pointer-to-member operator. Returns the currently owned interface.

template <typename T>
inline T* IFacePtr<T>::operator->()
{
	return m_pInterface;
}

////////////////////////////////////////////////////////////////////////////////
//! Not operator. Tests for a NULL pointer.

template <typename T>
bool IFacePtr<T>::operator!() const
{
	return (m_pInterface == nullptr);
}

////////////////////////////////////////////////////////////////////////////////
//! Access owned interface. Returns the currently owned interface.

template <typename T>
inline T* IFacePtr<T>::Get() const
{
	return m_pInterface;
}

////////////////////////////////////////////////////////////////////////////////
//! Access owned pointer as a reference.

template <typename T>
inline T& IFacePtr<T>::GetRef() const
{
	return *m_pInterface;
}

////////////////////////////////////////////////////////////////////////////////
//! Release the interface.

template <typename T>
inline void IFacePtr<T>::Release()
{
	if (m_pInterface != nullptr)
	{
		m_pInterface->Release();
		m_pInterface = nullptr;
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Helper function to gain access to the internal member so that it can be
//! passed as an output parameter, e.g. LoadTypeLib().

template <typename T>
inline T** AttachTo(IFacePtr<T>& ptr)
{
	return &ptr.m_pInterface;
}

//namespace Core
}

#endif // CORE_IFACEPTR_HPP
