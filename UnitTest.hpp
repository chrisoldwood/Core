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
#include <cstdlib>

#if (defined(_MSC_VER)) || (defined(__BORLANDC__)) || ( defined(__GNUC__) && (!defined(_APISETDEBUG_) && !defined(_WINBASE_H)) )
// Avoid bringing in <windows.h>.
extern "C" __declspec(dllimport) void __stdcall DebugBreak();
extern "C" __declspec(dllimport) int  __stdcall IsDebuggerPresent();
#endif

namespace Core
{

//! A collection of test suite names.
typedef std::set<tstring> TestSetFilters;

//! The Test Set run function type.
typedef void (*TestSetFn)();

//! The test case SetUp function type.
typedef void (*TestCaseSetUpFn)();

//! The test case TearDown function type.
typedef void (*TestCaseTearDownFn)();

////////////////////////////////////////////////////////////////////////////////
// The unit test functions.

// Parse the command line.
bool parseCmdLine(int argc, tchar* argv[], TestSetFilters& filters);

// Register a test set runner function.
bool registerTestSet(const tchar* name, TestSetFn runner);

// Run the self-registering test sets.
bool runTestSets(const TestSetFilters& filters);

// Start the test set.
void onStartTestSet(const tchar* name);

// End the test set.
void onEndTestSet();

// Define the test case set-up function.
void defineTestCaseSetup(TestCaseSetUpFn setup);

// Define the test case tear-down function.
void defineTestCaseTearDown(TestCaseTearDownFn teardown);

// Start a new test case.
bool onStartTestCase(const tchar* name);

// End a test case.
void onEndTestCase();

// Process the result of the test assert.
void processAssertResult(const char* file, size_t line, const tchar* expression, bool passed);

// Process an unexpected exception running a test case.
void processTestException(const char* file, size_t line, const tchar* error);

// Process an unexpected exception during set-up or tear-down.
void processSetupTeardownException(const tchar* function, const tchar* error);

// Set how the test run completed.
void setTestRunFinalStatus(bool successful);

// Write the summary of the test results to stdout.
void writeTestsSummary();

// Get the test process result code.
int getTestProcessResult();

////////////////////////////////////////////////////////////////////////////////
// The unit test macros.

//! Test that the expression result is true.
#define TEST_TRUE(x)	try {																		\
							if (x)	Core::processAssertResult(__FILE__, __LINE__, TXT(#x), true);	\
							else	Core::processAssertResult(__FILE__, __LINE__, TXT(#x), false);	\
						} catch(const Core::Exception& e) {											\
							Core::debugWrite(TXT("Unhandled Exception: %s\n"), e.twhat());			\
							Core::processAssertResult(__FILE__, __LINE__, TXT(#x), false);			\
						} catch(...) {																\
							Core::debugWrite(TXT("Unhandled Exception: %s\n"), TXT("UNKNOWN"));		\
							Core::processAssertResult(__FILE__, __LINE__, TXT(#x), false);			\
						}

//! Test that the expression result is false.
#define TEST_FALSE(x)	try {																		\
							if (x)	Core::processAssertResult(__FILE__, __LINE__, TXT(#x), false);	\
							else	Core::processAssertResult(__FILE__, __LINE__, TXT(#x), true);	\
						} catch(const Core::Exception& e) {											\
							Core::debugWrite(TXT("Unhandled Exception: %s\n"), e.twhat());			\
							Core::processAssertResult(__FILE__, __LINE__, TXT(#x), false);			\
						} catch(...) {																\
							Core::debugWrite(TXT("Unhandled Exception: %s\n"), TXT("UNKNOWN"));		\
							Core::processAssertResult(__FILE__, __LINE__, TXT(#x), false);			\
						}
//! Test that the expression causes an exception.
#define TEST_THROWS(x)	try {																	\
							(x);																\
							Core::processAssertResult(__FILE__, __LINE__, TXT(#x), false);		\
						} catch(const Core::Exception& e) {										\
							Core::debugWrite(TXT("Thrown: %s\n"), e.twhat());					\
							Core::processAssertResult(__FILE__, __LINE__, TXT(#x), true);		\
						} catch(const std::exception& e) {										\
							Core::debugWrite(TXT("Thrown: %hs\n"), e.what());					\
							Core::processAssertResult(__FILE__, __LINE__, TXT(#x), true);		\
						} catch(...) {															\
							Core::debugWrite(TXT("Unhandled Exception: %s\n"), TXT("UNKNOWN"));	\
							Core::processAssertResult(__FILE__, __LINE__, TXT(#x), false);		\
						}

//! Mark that the test has passed for the specified reason.
#define TEST_PASSED(r)	Core::processAssertResult(__FILE__, __LINE__, TXT(r), true);

//! Mark that the test has failed for the specified reason. The message is a string literal.
#define TEST_FAILED(r)	Core::processAssertResult(__FILE__, __LINE__, TXT(r), false);

//! Mark that the test has failed for the specified reason. The message is a string value.
#define TEST_FAILED_STR(r)	Core::processAssertResult(__FILE__, __LINE__, r, false);

//! Test suite preparation.
#define TEST_SUITE(c, v)	Core::TestSetFilters filters;			\
							if (!Core::parseCmdLine(c, v, filters))	\
								return EXIT_FAILURE;				\
							Core::enableLeakReporting(true);		\
							try {

//! Test suite reporting and clean-up.
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
#define TEST_SUITE_RUN()	if (!Core::runTestSets(filters))	\
								return EXIT_FAILURE;

//! Handle the entire set-up, execution and reporting of the test suite.
#define TEST_SUITE_MAIN(c, v)	TEST_SUITE(argc, argv)		\
								{							\
									TEST_SUITE_RUN();		\
								}							\
								TEST_SUITE_END

//! Define a set of test cases.
#define TEST_SET(t)			static void _##t##_TEST_SET();												\
							static bool registered = Core::registerTestSet(TXT(#t), _##t##_TEST_SET);	\
							static void _##t##_TEST_SET()												\
							{																			\
							Core::onStartTestSet(TXT(#t));

//! End the test set definition.
#define TEST_SET_END		Core::onEndTestSet();														\
							registered = true;															\
							}

//! Define the test case set-up function.
#define TEST_CASE_SETUP()		struct TestCaseSetup {			\
									static void fn()

//! End the test case set-up function definition.
#define TEST_CASE_SETUP_END		};								\
								Core::defineTestCaseSetup(TestCaseSetup::fn);

//! Define the test case tear-down function.
#define TEST_CASE_TEARDOWN()	struct TestCaseTearDown {			\
									static void fn()

//! End the test case tear-down function definition.
#define TEST_CASE_TEARDOWN_END	};								\
								Core::defineTestCaseTearDown(TestCaseTearDown::fn);

//! Define a test case.
#define TEST_CASE(t)		{																			\
							if (Core::onStartTestCase(TXT(t)))	{										\
								try {

//! End the test case definition.
#define TEST_CASE_END			}																		\
								catch(const Core::Exception& e) {										\
									Core::debugWrite(TXT("Thrown: %s\n"), e.twhat());					\
									Core::processTestException(__FILE__, __LINE__, e.twhat());			\
								} catch(...) {															\
									Core::debugWrite(TXT("Unhandled Exception: %s\n"), TXT("UNKNOWN"));	\
									Core::processTestException(__FILE__, __LINE__, TXT("UNKNOWN"));		\
								}																		\
							}																			\
							Core::onEndTestCase();														\
							}

//namespace Core
}

#endif // CORE_UNITTEST_HPP
