////////////////////////////////////////////////////////////////////////////////
//! \file   ExceptionTests.cpp
//! \brief  The unit tests for the Exception class and its derivatives.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/NotImplException.hpp>
#include <Core/NullPtrException.hpp>
#include <Core/InvalidArgException.hpp>
#include <Core/BadLogicException.hpp>
#include <Core/ParseException.hpp>
#include <Core/ConfigurationException.hpp>
#include <Core/RuntimeException.hpp>
#include <Core/FileSystemException.hpp>
#include <Core/AnsiWide.hpp>

TEST_SET(Exception)
{

TEST_CASE("compilation should succeed")
{
//	Core::Exception dc;											// Not default constructible.
//	Core::Exception cc(Core::NullPtrException(TXT("Test")));	// Not copy constructible.

//	Core::NotImplException e;									// Not default constructible.
	Core::NotImplException e1(TXT("Test"));

//	Core::debugWrite("%s", e1.what());							// std::exception::what() inaccessible.

	TEST_PASSED("compilation succeeded");
}
TEST_CASE_END

	const tstring message(TXT("test message"));

TEST_CASE("NotImplException is constructed with a message")
{
	Core::NotImplException exception(message);

	TEST_TRUE(tstrstr(exception.twhat(), message.c_str()) != nullptr);
}
TEST_CASE_END

TEST_CASE("NullPtrException is constructed with a message")
{
	Core::NullPtrException exception(message);

	TEST_TRUE(tstrstr(exception.twhat(), message.c_str()) != nullptr);
}
TEST_CASE_END

TEST_CASE("InvalidArgException is constructed with a message")
{
	Core::InvalidArgException exception(message);

	TEST_TRUE(tstrstr(exception.twhat(), message.c_str()) != nullptr);
}
TEST_CASE_END

TEST_CASE("BadLogicException is constructed with a message")
{
	Core::BadLogicException exception(message);

	TEST_TRUE(tstrstr(exception.twhat(), message.c_str()) != nullptr);
}
TEST_CASE_END

TEST_CASE("ParseException is constructed with a message")
{
	Core::ParseException exception(message);

	TEST_TRUE(tstrstr(exception.twhat(), message.c_str()) != nullptr);
}
TEST_CASE_END

TEST_CASE("ConfigurationException is constructed with a message")
{
	Core::ConfigurationException exception(message);

	TEST_TRUE(tstrstr(exception.twhat(), message.c_str()) != nullptr);
}
TEST_CASE_END

TEST_CASE("RuntimeException is constructed with a message")
{
	Core::RuntimeException exception(message);

	TEST_TRUE(tstrstr(exception.twhat(), message.c_str()) != nullptr);
}
TEST_CASE_END

TEST_CASE("FileSystemException is constructed with a message")
{
	Core::FileSystemException exception(message);

	TEST_TRUE(tstrstr(exception.twhat(), message.c_str()) != nullptr);
}
TEST_CASE_END

TEST_CASE("what ASSERTs when invoked")
{
	Core::NotImplException exception(message);

#ifdef CORE_CRTDBG_ENABLED
	int oldMode = _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);

	const std::exception& std_e = exception;

	TEST_TRUE(strstr(std_e.what(), T2A(message)) != nullptr);

	_CrtSetReportMode(_CRT_ASSERT, oldMode);
#else
	TEST_PASSED("Visual C++ specific test");
#endif
}
TEST_CASE_END

}
TEST_SET_END
