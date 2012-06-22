////////////////////////////////////////////////////////////////////////////////
//! \file   FileSystemException.hpp
//! \brief  The FileSystemException class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_FILESYSTEMEXCEPTION_HPP
#define CORE_FILESYSTEMEXCEPTION_HPP

#include "Exception.hpp"

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! The type of exception thrown when an error involving the file-system is
//! encountered.

class FileSystemException : public Exception
{
public:
	//! Construction from a string containing the details.
	FileSystemException(const tstring& strDetails);

	//! Destructor.
	virtual ~FileSystemException() throw();
};

////////////////////////////////////////////////////////////////////////////////
//! Construction from a string containing the details.

inline FileSystemException::FileSystemException(const tstring& strDetails)
	: Exception(strDetails)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline FileSystemException::~FileSystemException() throw()
{
}

//namespace Core
}

#endif // CORE_FILESYSTEMEXCEPTION_HPP
