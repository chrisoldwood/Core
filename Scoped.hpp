////////////////////////////////////////////////////////////////////////////////
//! \file   Scoped.hpp
//! \brief  The Scoped class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_SCOPED_HPP
#define CORE_SCOPED_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "BadLogicException.hpp"

namespace Core
{

// Forward declarations.
template <typename T>
class Scoped;

template <typename T>
T* attachTo(Scoped<T>& guard);

////////////////////////////////////////////////////////////////////////////////
//! A class for temporarily managing the lifetime of resources that require a
//! custom destroy function.

template <typename T>
class Scoped /*: private NotCopyable*/
{
public:
	//! The type of the function used to destroy the resource.
	typedef void (*Deleter)(T);

public:
	//! Construction from a destroy function and null value.
	Scoped(Deleter deleter, T null = 0);

	//! Construction from a resource, its destroy function and null value.
	Scoped(T resource, Deleter deleter, T null = 0);

	//! Destructor.
	~Scoped();

	//
	// Properties.
	//

	//! Get the managed resource.
	T get() const;

	//
	// Methods.
	//

	//! Query if we are not owning a resource.
	bool empty() const;

	//! Destroy the resource.
	void reset();

	//! Acquire ownership of a resource.
	void attach(T resource);

	//! Release ownership of the resource.
	T detach();

private:
	//
	// Members.
	//
	T		m_resource;		//!< The value to manage.
	Deleter	m_deleter;		//!< The function used to destroy the resource.
	T		m_null;			//!< The resource specific NULL value.

	//
	// Friends.
	//

	//! Allow attachment via an output parameter.
	friend T* attachTo<>(Scoped<T>& guard);

private:
	// NotCopyable.
	Scoped(const Scoped&);
	Scoped& operator=(const Scoped&);
};

////////////////////////////////////////////////////////////////////////////////
//! Construction from a destroy function and null value. This ctor is used with
//! the attachTo() free function to set-up the deleter before attaching the
//! resource.

template <typename T>
inline Scoped<T>::Scoped(Deleter deleter, T null)
	: m_resource(null)
	, m_deleter(deleter)
	, m_null(null)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Construction from a resource, its destroy function and null value.

template <typename T>
inline Scoped<T>::Scoped(T resource, Deleter deleter, T null)
	: m_resource(resource)
	, m_deleter(deleter)
	, m_null(null)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

template <typename T>
inline Scoped<T>::~Scoped()
{
	reset();
}

////////////////////////////////////////////////////////////////////////////////
//! Get the managed resource.

template <typename T>
inline T Scoped<T>::get() const
{
	return m_resource;
}

////////////////////////////////////////////////////////////////////////////////
//! Query if we are not owning a resource.

template <typename T>
inline bool Scoped<T>::empty() const
{
	return (m_resource == m_null);
}

////////////////////////////////////////////////////////////////////////////////
//! Destroy the resource.

template <typename T>
inline void Scoped<T>::reset()
{
	if (m_resource != m_null)
	{
		ASSERT(m_deleter != nullptr);

		m_deleter(m_resource);

		m_resource = m_null;
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Acquire ownership of a resource. If a resource is already held it is
//! destroyed first.

template <typename T>
inline void Scoped<T>::attach(T resource)
{
	reset();

	m_resource = resource;
}

////////////////////////////////////////////////////////////////////////////////
//! Release ownership of the resource.

template <typename T>
inline T Scoped<T>::detach()
{
	T resource = m_resource;

	m_resource = m_null;

	return resource;
}

////////////////////////////////////////////////////////////////////////////////
//! Helper function to gain access to the internal member so that it can be
//! passed as an output parameter, without overloading the & operator.
//! e.g. LoadTypeLib(..., attachTo(p)).

template <typename T>
T* attachTo(Scoped<T>& guard)
{
	if (!guard.empty())
		throw BadLogicException(TXT("Cannot attach to a non-empty smart pointer"));

	return &guard.m_resource;
}

//namespace Core
}

#endif // CORE_SCOPED_HPP
