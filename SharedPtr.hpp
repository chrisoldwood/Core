/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SHAREDPTR.HPP
** COMPONENT:	Core C++ Library
** DESCRIPTION:	The SharedPtr class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SHAREDPTR_HPP
#define SHAREDPTR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

/******************************************************************************
** 
** A reference counted smart pointer class.
**
*******************************************************************************
*/

template <typename T>
class SharedPtr
{
public:
	//
	// Constructors/Destructor.
	//
	SharedPtr();
	SharedPtr(T* pPointer);
	SharedPtr(const SharedPtr<T>& oPointer);
	~SharedPtr();
	
	//
	// Operators.
	//
	SharedPtr& operator=(const SharedPtr<T>& oPointer);
	T&         operator*() const;
	const T*   operator->() const;
	T*         operator->();
    bool       operator!() const;

	//
	// Methods.
	//
	T*   Get() const;
	void Reset(T* pPointer = nullptr);

protected:
	//
	// Members.
	//
	T*		m_pPointer;		// The pointer being shared.
	ulong*	m_pRefCnt;		// The reference count.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

/******************************************************************************
** Default constructor.
*/

template <typename T>
inline SharedPtr<T>::SharedPtr()
	: m_pPointer(nullptr)
	, m_pRefCnt(nullptr)
{
}

/******************************************************************************
** Construction from a new pointer.
*/

template <typename T>
inline SharedPtr<T>::SharedPtr(T* pPointer)
	: m_pPointer(nullptr)
	, m_pRefCnt(nullptr)
{
	Reset(pPointer);
}

/******************************************************************************
** Copy constructor.
*/

template <typename T>
inline SharedPtr<T>::SharedPtr(const SharedPtr<T>& oPointer)
	: m_pPointer(oPointer.m_pPointer)
	, m_pRefCnt(oPointer.m_pRefCnt)
{
	if (m_pRefCnt != nullptr)
		++(*m_pRefCnt);
}

/******************************************************************************
** Destructor.
*/

template <typename T>
inline SharedPtr<T>::~SharedPtr()
{
	Reset(nullptr);
}

/******************************************************************************
** Assignment operator.
*/

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

/******************************************************************************
** Access the underlying pointer.
*/

template <typename T>
inline T& SharedPtr<T>::operator*() const
{
	return *m_pPointer;
}

/******************************************************************************
** Access the underlying pointer.
*/

template <typename T>
inline const T* SharedPtr<T>::operator->() const
{
	return m_pPointer;
}

/******************************************************************************
** Access the underlying pointer.
*/

template <typename T>
inline T* SharedPtr<T>::operator->()
{
	return m_pPointer;
}

/******************************************************************************
** ! Operator.
*/

template <typename T>
bool SharedPtr<T>::operator!() const
{
	return (m_pPointer == nullptr);
}

/******************************************************************************
** Get the underlying shared pointer.
*/

template <typename T>
inline T* SharedPtr<T>::Get() const
{
	return m_pPointer;
}

/******************************************************************************
** Replace the existing pointer.
*/

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

/******************************************************************************
** Global == Operator.
*/

template <typename T>
inline bool operator==(const SharedPtr<T>& oLHS, const SharedPtr<T>& oRHS)
{
	return (oLHS.Get() == oRHS.Get());
}

/******************************************************************************
** Global != Operator.
*/

template <typename T>
inline bool operator!=(const SharedPtr<T>& oLHS, const SharedPtr<T>& oRHS)
{
	return (oLHS.Get() != oRHS.Get());
}

//namespace Core
}

#endif // SHAREDPTR_HPP
