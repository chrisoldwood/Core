////////////////////////////////////////////////////////////////////////////////
//! \file   TestException.cpp
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

TEST_SET(Exception)
{

TEST_CASE(Exception, compilationFails)
{
//	Core::Exception dc;											// Not default constructible.
//	Core::Exception cc(Core::NullPtrException(TXT("Test")));	// Not copy constructible.

//	Core::NotImplException e;									// Not default constructible.
	Core::NotImplException e1(TXT("Test"));

//	Core::debugWrite("%s", e1.what());							// std::exception::what() inaccessible.
}
TEST_CASE_END

TEST_CASE(Exception, constructible)
{
	Core::NotImplException e(TXT("Test"));
	Core::NullPtrException npe(TXT("Test"));
	Core::InvalidArgException iae(TXT("Test"));
	Core::BadLogicException ble(TXT("Test"));
	Core::ParseException pe(TXT("Test"));
	Core::ConfigurationException ce(TXT("Test"));
	Core::RuntimeException re(TXT("Test"));

	Core::NotImplException copy(e);
}
TEST_CASE_END

TEST_CASE(Exception, twhatSucceeds)
{
	Core::NotImplException e(TXT("Test"));
	Core::NotImplException copy(e);

	TEST_TRUE(tstrcmp(e.twhat(), TXT("Test")) == 0);
	TEST_TRUE(tstrcmp(copy.twhat(), TXT("Test")) == 0);
}
TEST_CASE_END

TEST_CASE(Exception, whatAsserts)
{
	Core::NotImplException e(TXT("Test"));

#ifdef CORE_CRTDBG_ENABLED
	int oldMode = _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
	const std::exception& std_e = e;
	TEST_TRUE(strcmp(std_e.what(), "Test") == 0);
	_CrtSetReportMode(_CRT_ASSERT, oldMode);
#endif
}
TEST_CASE_END

}
TEST_SET_END
