////////////////////////////////////////////////////////////////////////////////
//! \file   UnitTest.cpp
//! \brief  Functions for writing simple unit tests.
//! \author Chris Oldwood

#include "Common.hpp"
#include "UnitTest.hpp"
#include "StringUtils.hpp"
#include "tiostream.hpp"
#include "Algorithm.hpp"
#include "CmdLineException.hpp"
#include "CmdLineParser.hpp"
#include <algorithm>
#include <map>
#include <stdlib.h>

namespace Core
{

//! A collection of test sets.
typedef std::map<tstring, TestSetFn> TestSets;
//! A collection of test case names.
typedef std::vector<tstring> TestCaseNames;
//! A collection of per-test assert messages.
typedef std::vector<tstring> TestCaseAsserts;
//! A collection of test suite assert messages.
typedef std::vector<TestCaseAsserts> TestSuiteAsserts;

//! The overall state of the test run.
static bool s_successful = false;
//! The number of tests that passed.
static uint s_numPassed = 0;
//! The number of tests that failed.
static uint s_numFailed = 0;
//! The number of tests that were indeterminate.
static uint s_numUnknown = 0;
//! Is verbose mode enabled?
static bool s_verbose = false;
//! Is quiet mode enabled?
static bool s_quiet = false;
//! Invoke DebugBreak on failure?
static bool s_debug = false;

//! The test runner command line switch IDs.
enum SwitchID
{
	HELP	= 0,	//!< Show command line usage.
	VERBOSE	= 1,	//!< Verbose output of each test case.
	QUIET	= 2,	//!< Minimal output of each test case.
	DEBUG	= 3,	//!< Invoke DebugBreak() when a test fails.
};

//! Test case result state.
enum TestResult
{
	UNKNOWN		= 1,	//! Test result undetermined.
	SUCCEEDED	= 2,	//! Test definitely succeeded.
	FAILED		= 3,	//! Test definitely failed.
};

//! Current test case status.
static TestResult s_currentResult = UNKNOWN;
//! The name of the executing test set.
static tstring s_currentTestSet;
//! The name of the executing test case.
static tstring s_currentTestCase;
//! The set of asserts for the executing test case.
static TestCaseAsserts s_currentTestCaseAsserts;
//! The list of failed test cases for the set.
static TestCaseNames s_failures;
//! The list of asserts for the failed test cases.
static TestSuiteAsserts s_failuresAsserts;
//! The test case SetUp function.
static TestCaseSetUpFn s_setup;
//! The test case TearDown function.
static TestCaseTearDownFn s_teardown;
//! The list of test cases executed.
static TestCaseNames s_executed;

////////////////////////////////////////////////////////////////////////////////
//! Parse the command line. This extracts the list of test cases to run.

static Core::CmdLineSwitch s_switches[] =
{
	{ HELP,		TXT("?"),	nullptr,		Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,	nullptr,	TXT("Display command line usage")		},
	{ HELP,		TXT("h"),	TXT("help"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,	nullptr,	TXT("Display command line usage")		},
	{ VERBOSE,	TXT("v"),	TXT("verbose"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,	nullptr,	TXT("Verbose output of each test case")	},
	{ QUIET,	TXT("q"),	TXT("quiet"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,	nullptr,	TXT("Minimal output of each test case")	},
	{ DEBUG,	TXT("d"),	TXT("debug"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,	nullptr,	TXT("Invoke DebugBreak() on failure")	},
};
static size_t s_switchCount = ARRAY_SIZE(s_switches);

////////////////////////////////////////////////////////////////////////////////
//! Display the program options syntax.

static void showUsage(const tchar* process, const Core::CmdLineParser& parser)
{
	tcout << std::endl;
	tcout << TXT("USAGE: ") << process << (" [options...] [test_case, ...]") << std::endl;
	tcout << std::endl;

	tcout << TXT("Options:-") << std::endl;
	tcout << std::endl;
	tcout << parser.formatSwitches(Core::CmdLineParser::UNIX);
}

////////////////////////////////////////////////////////////////////////////////
//! Parse the command line. This extracts the list of test cases to run.

bool parseCmdLine(int argc, tchar* argv[], TestSetFilters& filters)
{
	// Parse the command line.
	Core::CmdLineParser parser(s_switches, s_switches+s_switchCount);

	try
	{
		parser.parse(argc, argv);

		if (parser.isSwitchSet(VERBOSE) && parser.isSwitchSet(QUIET))
			throw CmdLineException(TXT("--quiet and --verbose are mutually exclusive"));
	}
	catch (const CmdLineException& e)
	{
		tcerr << TXT("ERROR: ") << e.twhat() << std::endl;
		showUsage(argv[0], parser);
		return false;
	}

	// Help requested?
	if (parser.isSwitchSet(HELP))
	{
		showUsage(argv[0], parser);
		return false;
	}

	// Process command line settings.
	s_verbose = parser.isSwitchSet(VERBOSE);
	s_quiet = parser.isSwitchSet(QUIET);
	s_debug = parser.isSwitchSet(DEBUG);

	// Build the test case list.
	Core::CmdLineParser::UnnamedArgs::const_iterator it = parser.getUnnamedArgs().begin();
	Core::CmdLineParser::UnnamedArgs::const_iterator end = parser.getUnnamedArgs().end();

	for (;it != end; ++it)
		filters.insert(Core::createLower(*it));

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the collection of test sets. This is accessed via a special function
//! to workaround the indeterminate initialisation order of the self-registering
//! test sets and this collection.

static TestSets& getTestSetCollection()
{
	static TestSets s_testTests;
	return s_testTests;
}

////////////////////////////////////////////////////////////////////////////////
//! Register a test set runner function.

bool registerTestSet(const tchar* name, TestSetFn runner)
{
	ASSERT(tstrlen(name) != 0);
	ASSERT(runner != nullptr);

	TestSets& sets = getTestSetCollection();

	ASSERT(sets.find(name) == sets.end());

	sets.insert(std::make_pair(name, runner));

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Comparison functor for matching test sets against by test set name.

struct TestSetComparator
{
	tstring m_value;

	TestSetComparator(const tstring& value)
		: m_value(value)
	{ }

	bool operator()(const std::pair<tstring, TestSetFn>& rhs)
	{
		return (tstricmp(m_value.c_str(), rhs.first.c_str()) == 0);
	}
};

////////////////////////////////////////////////////////////////////////////////
//! Run the self-registering test sets.

bool runTestSets(const TestSetFilters& filters)
{
	TestSets& testSets = getTestSetCollection();

	if (!filters.empty())
	{
		for (TestSetFilters::const_iterator it = filters.begin(); it != filters.end(); ++it)
		{
			const tstring& name = *it;

			if ((std::find_if(testSets.begin(), testSets.end(), TestSetComparator(name)) == testSets.end()))
			{
				tcerr << TXT("ERROR: Unknown test set '") << name << TXT("'") << std::endl;
				return false;
			}
		}
	}

	tcout << std::endl;

	for (TestSets::const_iterator it = testSets.begin(); it != testSets.end(); ++it)
	{
		tstring name = createLower(it->first);

		if ( (filters.empty()) || (filters.find(name) != filters.end()) )
			it->second();
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Start the test set.

void onStartTestSet(const tchar* name)
{
	ASSERT(tstrlen(name) != 0);
	ASSERT(s_currentTestCase.empty());
	ASSERT(s_currentTestCaseAsserts.empty());

	if (!s_quiet)
	{
		tcout << name << std::endl;

		if (!s_verbose)
			tcout << TXT(" ");
	}

	s_currentTestSet = name;
	s_failures.clear();
	s_failuresAsserts.clear();
	s_executed.clear();
	s_setup = nullptr;
	s_teardown = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
//! End the test set.

void onEndTestSet()
{
	ASSERT(s_currentTestCase.empty());
	ASSERT(s_currentTestCaseAsserts.empty());

	if (!s_quiet && !s_verbose)
	{
		tcout << std::endl;
	}

	if (!s_failures.empty())
	{
		const tstring failureCount = (s_quiet) ? Core::fmt(TXT(" %s: %Iu failure(s)"), s_currentTestSet.c_str(), s_failures.size())
											   : Core::fmt(TXT(" %Iu failure(s)"), s_failures.size());

		tcout << std::endl;
		tcout << failureCount << std::endl;

		for (size_t i = 0; i != s_failures.size(); ++i)
		{
			debugWrite(TXT(" > %s\n"), s_failures[i].c_str());
			tcout << TXT(" > ") << s_failures[i] << std::endl;

			for (size_t j = 0; j != s_failuresAsserts[i].size(); ++j)
			{
				debugWrite(TXT("  %s\n"), s_failuresAsserts[i][j].c_str());
				tcout << TXT("  ") << s_failuresAsserts[i][j] << std::endl;
			}
		}
	}

	if (!s_quiet)
	{
		tcout << std::endl;
	}

	s_currentTestSet.clear();
	s_failures.clear();
	s_failuresAsserts.clear();
	s_executed.clear();
	s_setup = nullptr;
	s_teardown = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Define the test case setup function.

void defineTestCaseSetup(TestCaseSetUpFn setup)
{
	s_setup = setup;
}

////////////////////////////////////////////////////////////////////////////////
//! Define the test case tear-down function.

void defineTestCaseTearDown(TestCaseTearDownFn teardown)
{
	s_teardown = teardown;
}

////////////////////////////////////////////////////////////////////////////////
//! Start a new test case.

bool onStartTestCase(const tchar* name)
{
	ASSERT(tstrlen(name) != 0);
	ASSERT(s_currentTestCase.empty());
	ASSERT(s_currentTestCaseAsserts.empty());
	ASSERT(!exists<tstring>(s_executed, name));

	if (s_verbose)
		tcout << TXT(" > ") << name << std::endl;

	s_currentResult = UNKNOWN;
	s_currentTestCase = name;

	if (s_setup != nullptr)
	{
		try
		{
			s_setup();
		}
		catch(const Core::Exception& e)
		{
			processSetupTeardownException(TXT("SetUp"), e.twhat());
			return false;
		}
		catch (...)
		{
			processSetupTeardownException(TXT("SetUp"), TXT("UNKNOWN"));
			return false;
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! End a test case.

void onEndTestCase()
{
	ASSERT(!s_currentTestCase.empty());

	if (s_teardown != nullptr)
	{
		try
		{
			s_teardown();
		}
		catch(const Core::Exception& e)
		{
			processSetupTeardownException(TXT("TearDown"), e.twhat());
		}
		catch (...)
		{
			processSetupTeardownException(TXT("TearDown"), TXT("UNKNOWN"));
		}
	}

	s_executed.push_back(s_currentTestCase);

	// Update statistics.
	if (s_currentResult == SUCCEEDED)
	{
		++s_numPassed;

		if (!s_verbose)
			tcout << TXT(".");
	}
	else if (s_currentResult == FAILED)
	{
		++s_numFailed;

		s_failures.push_back(s_currentTestCase);
		s_failuresAsserts.push_back(s_currentTestCaseAsserts);

		if (!s_verbose)
			tcout << TXT("F");
	}
	else
	{
		ASSERT_FALSE();
		ASSERT(s_currentResult == UNKNOWN);

		++s_numUnknown;

		if (!s_verbose)
			tcout << TXT("?");
	}

	s_currentResult = UNKNOWN;
	s_currentTestCase.clear();
	s_currentTestCaseAsserts.clear();
}

////////////////////////////////////////////////////////////////////////////////
//! Get just the file-name from the path.

static const char* getFileName(const char* path)
{
	const char* filename = strrchr(path, '\\');

	if (filename != nullptr)
		++filename;
	else
		filename = path;

	return filename;
}

#if (__GNUC__ >= 8) // GCC 8+
// error: format '%hs' expects argument of type 'short int*', but argument 3 has type 'const char*' [-Werror=format=]
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#endif

////////////////////////////////////////////////////////////////////////////////
//! Write the assert result to stdout.

void processAssertResult(const char* file, size_t line, const tchar* expression, bool passed)
{
	if (s_currentResult == UNKNOWN)
	{
		s_currentResult = (passed) ? SUCCEEDED : FAILED;
	}
	else if (!passed)
	{
		s_currentResult = FAILED;
	}

	const tchar*  result = (passed) ? TXT("Passed") : TXT("FAILED");
	const char*   filename = getFileName(file);
	const tstring assert = Core::fmt(TXT(" %s [%hs, %3Iu] %s"), result, filename, line, expression);

	s_currentTestCaseAsserts.push_back(assert);

	if (s_verbose)
	{
		tcout << assert << std::endl;
	}

	if (!passed && s_debug)
		::DebugBreak();
}

////////////////////////////////////////////////////////////////////////////////
//! Process an unexpected exception running a test case.

void processTestException(const char* file, size_t line, const tchar* error)
{
	s_currentResult = FAILED;

	const tchar*  result = TXT("FAILED");
	const char*   filename = getFileName(file);
	const tstring assert = Core::fmt(TXT(" %s [%hs, %3Iu] Threw: %s"), result, filename, line, error);

	s_currentTestCaseAsserts.push_back(assert);

	if (s_verbose)
	{
		tcout << assert << std::endl;
	}

	if (s_debug)
		::DebugBreak();
}

#if (__GNUC__ >= 8) // GCC 8+
#pragma GCC diagnostic pop
#endif

////////////////////////////////////////////////////////////////////////////////
//! Process an unexpected exception during set-up or tear-down.

void processSetupTeardownException(const tchar* function, const tchar* error)
{
	s_currentResult = FAILED;

	if (s_verbose)
	{
		Core::debugWrite(TXT("Unhandled Exception: %s\n"), error);

		tcout << Core::fmt(TXT(" %s [%s] Unhandled Exception: %s"), TXT("FAILED"), function, error) << std::endl;
	}

	if (s_debug)
		::DebugBreak();
}

////////////////////////////////////////////////////////////////////////////////
//! Set how the test run completed.

void setTestRunFinalStatus(bool successful)
{
	s_successful = successful;
}

////////////////////////////////////////////////////////////////////////////////
//! Write the summary of the test results to the debugger stream and stdout.

void writeTestsSummary()
{
	if (s_quiet)
	{
		tcout << std::endl << std::endl;
	}

	tstring str = Core::fmt(TXT("Test Results: %u Passed %u Failed %u Unknown"),
							s_numPassed, s_numFailed, s_numUnknown);

	if (!s_successful)
		str += TXT(" [RUN TERMINATED ABORMALLY]");

	debugWrite(TXT("%s\n"), str.c_str());
	std::tcout << str << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the test process result code.

int getTestProcessResult()
{
	return (s_successful && (s_numFailed == 0)) ? EXIT_SUCCESS : EXIT_FAILURE;
}

//namespace Core
}
