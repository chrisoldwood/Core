////////////////////////////////////////////////////////////////////////////////
//! \file   ParseException.hpp
//! \brief  The ParseException class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef XML_PARSEEXCEPTION_HPP
#define XML_PARSEEXCEPTION_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace XML
{

////////////////////////////////////////////////////////////////////////////////
//! The type of exception thrown during parsing of an XML document.

class ParseException : public Core::Exception
{
public:
	//! Default constructor.
	ParseException(const tstring& strDetails);

	//! Destructor.
	virtual ~ParseException();
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

inline ParseException::ParseException(const tstring& strDetails)
	: Exception(strDetails)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline ParseException::~ParseException()
{
}

//namespace XML
}

#endif // XML_PARSEEXCEPTION_HPP
