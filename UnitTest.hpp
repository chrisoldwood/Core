////////////////////////////////////////////////////////////////////////////////
//! \file   UnitTest.hpp
//! \brief  Macros and functions for writing simple unit tests.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_UNITTEST_HPP
#define CORE_UNITTEST_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
// The unit test functions.

// Write the test result to stdout.
void WriteTestResult(const char* pszFile, size_t nLine, const tchar* pszExpression, bool bPassed);

// Set how the test run completed.
void SetTestRunFinalStatus(bool bSuccess);

// Write the summary of the test results to stdout.
void WriteTestsSummary();

// Get the test process result code.
int GetTestProcessResult();

////////////////////////////////////////////////////////////////////////////////
// The unit test macros.

//! Test that the expression result is true.
#define TEST_TRUE(x)	try {																	\
							if (x)	Core::WriteTestResult(__FILE__, __LINE__, TXT(#x), true);	\
							else	Core::WriteTestResult(__FILE__, __LINE__, TXT(#x), false);	\
						} catch(...) {															\
							Core::WriteTestResult(__FILE__, __LINE__, TXT(#x), false);			\
						}

//! Test that the expression result is false.
#define TEST_FALSE(x)	try {																	\
							if (x)	Core::WriteTestResult(__FILE__, __LINE__, TXT(#x), false);	\
							else	Core::WriteTestResult(__FILE__, __LINE__, TXT(#x), true);	\
						} catch(...) {															\
							Core::WriteTestResult(__FILE__, __LINE__, TXT(#x), false);			\
						}
//! Test that the expression casuses an exception.
#define TEST_THROWS(x)	try {																	\
							(x);																\
							Core::WriteTestResult(__FILE__, __LINE__, TXT(#x), false);			\
						} catch(const std::exception& /*e*/) {									\
							Core::WriteTestResult(__FILE__, __LINE__, TXT(#x), true);			\
						} catch(...) {															\
							Core::WriteTestResult(__FILE__, __LINE__, TXT(#x), false);			\
						}

//! Test suite preparation.
#define TEST_SUITE_BEGIN	Core::EnableLeakReporting(true);	\
							try

//! Test suite reporting and cleanup.
#define TEST_SUITE_END		catch (const Core::Exception& e)											\
							{	std::tcout << TXT("Unhandled exception: ") << e.What() << std::endl; }	\
							catch (...)																	\
							{	std::tcout << TXT("Unhandled exception: UNKNOWN") << std::endl; }		\
							Core::WriteTestsSummary();													\
							return Core::GetTestProcessResult();

//namespace Core
}

#endif // CORE_UNITTEST_HPP
