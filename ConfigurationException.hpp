////////////////////////////////////////////////////////////////////////////////
//! \file   ConfigurationException.hpp
//! \brief  The ConfigurationException class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_CONFIGURATIONEXCEPTION_HPP
#define CORE_CONFIGURATIONEXCEPTION_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! The exception type thrown for configuration errors.

class ConfigurationException : public Core::Exception
{
public:
	//! Construction from a string containing the details.
	ConfigurationException(const tstring& details);

	//! Destructor.
	virtual ~ConfigurationException() throw();
};

////////////////////////////////////////////////////////////////////////////////
//! Construction from a string containing the details.

inline ConfigurationException::ConfigurationException(const tstring& details)
	: Exception(details)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline ConfigurationException::~ConfigurationException() throw()
{
}

//namespace Core
}

#endif // CORE_CONFIGURATIONEXCEPTION_HPP
