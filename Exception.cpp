////////////////////////////////////////////////////////////////////////////////
//! \file   Exception.cpp
//! \brief  The Exception class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Exception.hpp"
#include "AnsiWide.hpp"

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

Exception::Exception()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Construction from a string containg the details.

Exception::Exception(const std::tstring& strDetails)
	: m_strDetails(strDetails)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor.

Exception::Exception(const Exception& rhs)
	: m_strDetails(rhs.m_strDetails)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

Exception::~Exception()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Get the exception details.

const tchar* Exception::What() const
{
	return m_strDetails.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//! Get the ANSI only exception details. This method is marked private to avoid
//! it being called by accident instead of the build agnostic method What().
//! However it could still be invoked through the base class, so we still need
//! to generate an ANSI version on demand.

const char* Exception::what() const
{
#ifdef ANSI_BUILD
	return m_strDetails.c_str();
#else
	ASSERT_FALSE();

	// Generate ANSI string on demand.
	if (m_strAnsiDetails.empty())
		m_strAnsiDetails = Core::WideToAnsi(m_strDetails);

	return m_strAnsiDetails.c_str();
#endif
}

//namespace Core
}
