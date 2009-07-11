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

#include <set>
#include "tiostream.hpp"
#include "StringUtils.hpp"

// Avoid bringing in <windows.h>.
extern "C" __declspec(dllimport) void __stdcall DebugBreak();
extern "C" __declspec(dllimport) int  __stdcall IsDebuggerPresent();

namespace Core
{

//! A collection of test suite names.
typedef std::set<tstring> TestCases;

////////////////////////////////////////////////////////////////////////////////
// The unit test functions.

// Parse the command line.
void parseCmdLine(int argc, tchar* argv[], TestCases& cases);

// Write the test result to stdout.
void writeTestResult(const char* pszFile, size_t nLine, const tchar* pszExpression, bool bPassed);

// Set how the test run completed.
void setTestRunFinalStatus(bool bSuccess);

// Write the summary of the test results to stdout.
void writeTestsSummary();

// Get the test process result code.
int getTestProcessResult();

////////////////////////////////////////////////////////////////////////////////
// The unit test macros.

//! Test that the expression result is true.
#define TEST_TRUE(x)	try {																	\
							if (x)	Core::writeTestResult(__FILE__, __LINE__, TXT(#x), true);	\
							else	Core::writeTestResult(__FILE__, __LINE__, TXT(#x), false);	\
						} catch(const Core::Exception& e) {										\
							Core::debugWrite(TXT("Unhandled Exception: %s\n"), e.twhat());		\
							Core::writeTestResult(__FILE__, __LINE__, TXT(#x), false);			\
						} catch(...) {															\
							Core::debugWrite(TXT("Unhandled Exception: %s\n"), TXT("UNKNOWN"));	\
							Core::writeTestResult(__FILE__, __LINE__, TXT(#x), false);			\
						}

//! Test that the expression result is false.
#define TEST_FALSE(x)	try {																	\
							if (x)	Core::writeTestResult(__FILE__, __LINE__, TXT(#x), false);	\
							else	Core::writeTestResult(__FILE__, __LINE__, TXT(#x), true);	\
						} catch(const Core::Exception& e) {										\
							Core::debugWrite(TXT("Unhandled Exception: %s\n"), e.twhat());		\
							Core::writeTestResult(__FILE__, __LINE__, TXT(#x), false);			\
						} catch(...) {															\
							Core::debugWrite(TXT("Unhandled Exception: %s\n"), TXT("UNKNOWN"));	\
							Core::writeTestResult(__FILE__, __LINE__, TXT(#x), false);			\
						}
//! Test that the expression casuses an exception.
#define TEST_THROWS(x)	try {																	\
							(x);																\
							Core::writeTestResult(__FILE__, __LINE__, TXT(#x), false);			\
						} catch(const Core::Exception& e) {										\
							Core::debugWrite(TXT("Thrown: %s\n"), e.twhat());					\
							Core::writeTestResult(__FILE__, __LINE__, TXT(#x), true);			\
						} catch(const std::exception& e) {										\
							Core::debugWrite(TXT("Thrown: %hs\n"), e.what());					\
							Core::writeTestResult(__FILE__, __LINE__, TXT(#x), true);			\
						} catch(...) {															\
							Core::debugWrite(TXT("Unhandled Exception: %s\n"), TXT("UNKNOWN"));	\
							Core::writeTestResult(__FILE__, __LINE__, TXT(#x), false);			\
						}

//! Test suite preparation.
#define TEST_SUITE_BEGIN(c, v)	Core::TestCases cases;				\
								Core::parseCmdLine(c, v, cases);	\
								Core::enableLeakReporting(true);	\
								try

//! Define a test case.
#define TEST_CASE(t)			if ( (cases.empty()) || (cases.find(Core::createLower(TXT(#t))) != cases.end()) )	\
								{ t(); }

//! Test suite reporting and cleanup.
#define TEST_SUITE_END		catch(const Core::Exception& e) {											\
								Core::debugWrite(TXT("Unhandled Exception: %s\n"), e.twhat());			\
								if (::IsDebuggerPresent()) ::DebugBreak();								\
								tcout << TXT("Unhandled Exception: ") << e.twhat() << std::endl; 	\
							} catch(...) {																\
								Core::debugWrite(TXT("Unhandled Exception: %s\n"), TXT("UNKNOWN"));		\
								if (::IsDebuggerPresent()) ::DebugBreak();								\
								tcout << TXT("Unhandled Exception: UNKNOWN") << std::endl;			\
							}																			\
							Core::writeTestsSummary();													\
							return Core::getTestProcessResult();

//namespace Core
}

#endif // CORE_UNITTEST_HPP
