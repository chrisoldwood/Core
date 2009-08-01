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

//! The Test Set run function.
typedef void (*TestSetFn)();

////////////////////////////////////////////////////////////////////////////////
// The unit test functions.

// Parse the command line.
void parseCmdLine(int argc, tchar* argv[], TestCases& cases);

// Register a test set runner function.
bool registerTestSet(const tchar* name, TestSetFn runner);

// Run the self-registering test sets.
void runTestSets(const TestCases& cases);

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
#define TEST_SUITE(c, v)	Core::TestCases cases;				\
							Core::parseCmdLine(c, v, cases);	\
							Core::enableLeakReporting(true);	\
							try {

//! Test suite reporting and cleanup.
#define TEST_SUITE_END			Core::setTestRunFinalStatus(true);										\
							}																			\
							catch(const Core::Exception& e) {											\
								Core::debugWrite(TXT("Unhandled Exception: %s\n"), e.twhat());			\
								if (::IsDebuggerPresent()) ::DebugBreak();								\
								tcout << TXT("Unhandled Exception: ") << e.twhat() << std::endl; 		\
							} catch(...) {																\
								Core::debugWrite(TXT("Unhandled Exception: %s\n"), TXT("UNKNOWN"));		\
								if (::IsDebuggerPresent()) ::DebugBreak();								\
								tcout << TXT("Unhandled Exception: UNKNOWN") << std::endl;				\
							}																			\
							Core::writeTestsSummary();													\
							return Core::getTestProcessResult();

//! Run the self-registering test sets
#define TEST_SUITE_RUN()	Core::runTestSets(cases);

//! Handle the entire setup, execution and reporting of the test suite.
#define TEST_SUITE_MAIN(c, v)	TEST_SUITE(argc, argv)		\
								{							\
									TEST_SUITE_RUN();		\
								}							\
								TEST_SUITE_END
	
//! Define a set of test cases.
#define TEST_SET(t)			static void t();												\
							static bool registered = Core::registerTestSet(TXT(#t), t);		\
							static void t() {

//! End the test set definition.
#define TEST_SET_END		}

//! Execute a set of test cases.
#define TEST_SET_RUN(t)		if ( (cases.empty()) || (cases.find(Core::createLower(TXT(#t))) != cases.end()) ) {	\
								extern void t();																\
								t();																			\
							}

//! Define a test case.
#define TEST_CASE(s, t)		{																			\
							tcout << TXT(#s) << TXT(" -> ") << TXT(#t) << std::endl;					\
							try {

//! End the test case definition.
#define TEST_CASE_END		}																			\
							catch(const Core::Exception& e) {											\
								Core::debugWrite(TXT("Unhandled Exception: %s\n"), e.twhat());			\
								if (::IsDebuggerPresent()) ::DebugBreak();								\
								tcout << TXT("Unhandled Exception: ") << e.twhat() << std::endl; 		\
							} catch(...) {																\
								Core::debugWrite(TXT("Unhandled Exception: %s\n"), TXT("UNKNOWN"));		\
								if (::IsDebuggerPresent()) ::DebugBreak();								\
								tcout << TXT("Unhandled Exception: UNKNOWN") << std::endl;				\
							}																			\
							}

//namespace Core
}

#endif // CORE_UNITTEST_HPP
