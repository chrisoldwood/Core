////////////////////////////////////////////////////////////////////////////////
//! \file   UnitTest.cpp
//! \brief  Functions for writing simple unit tests.
//! \author Chris Oldwood

#include "Common.hpp"
#include "UnitTest.hpp"
#include "StringUtils.hpp"
#include "tiostream.hpp"
#include "CmdLineException.hpp"
#include "CmdLineParser.hpp"
#include <stdlib.h>
#include <map>

namespace Core
{

//! A collection of test sets.
typedef std::map<tstring, TestSetFn> TestSets;
//! A collection of test case names.
typedef std::vector<tstring> TestCaseNames;

//! The overall state of the test run.
static bool s_successful = false;
//! The number of tests that passed.
static uint s_numPassed = 0;
//! The number of tests that failed.
static uint s_numFailed = 0;
//! The number of tests that were indeterminate.
static uint s_numUnknown = 0;
//! Is quiet mode enabled?
static bool s_verbose = false;

//! The test runner command line switch IDs.
enum SwitchID
{
	HELP	= 0,	//!< Show command line usage.
	VERBOSE	= 1,	//!< Verbose output of each test case.
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
//! The list of failed test cases for the set.
static TestCaseNames s_failures;

////////////////////////////////////////////////////////////////////////////////
//! Parse the command line. This extracts the list of test cases to run.

static Core::CmdLineSwitch s_switches[] =
{
	{ HELP,		TXT("?"),	NULL,			Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,	NULL,	TXT("Display command line usage")		},
	{ HELP,		TXT("h"),	TXT("help"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,	NULL,	TXT("Display command line usage")		},
	{ VERBOSE,	TXT("v"),	TXT("verbose"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,	NULL,	TXT("Verbose output of each test case")	},
};
static size_t s_switchCount = ARRAY_SIZE(s_switches);

////////////////////////////////////////////////////////////////////////////////
//! Display the program options syntax.

void showUsage(const tchar* process, const Core::CmdLineParser& parser)
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

bool parseCmdLine(int argc, tchar* argv[], TestCases& cases)
{
	// Parse the command line.
	Core::CmdLineParser parser(s_switches, s_switches+s_switchCount);

	try
	{
		parser.parse(argc, argv);
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

	// Build the test case list.
	Core::CmdLineParser::UnnamedArgs::const_iterator it = parser.getUnnamedArgs().begin();
	Core::CmdLineParser::UnnamedArgs::const_iterator end = parser.getUnnamedArgs().end();

	for (;it != end; ++it)
		cases.insert(Core::createLower(*it));

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
	getTestSetCollection().insert(std::make_pair(name, runner));

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Run the self-registering test sets.

void runTestSets(const TestCases& cases)
{
	tcout << std::endl;

	TestSets& testSets = getTestSetCollection();

	for (TestSets::const_iterator it = testSets.begin(); it != testSets.end(); ++it)
	{
		tstring name = createLower(it->first);

		if ( (cases.empty()) || (cases.find(name) != cases.end()) )
			it->second();
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Start the test set.

void onStartTestSet(const tchar* name)
{
	tcout << name << std::endl;

	if (!s_verbose)
		tcout << TXT(" ");

	s_currentTestSet = name;
	s_failures.clear();
}

////////////////////////////////////////////////////////////////////////////////
//! End the test set.

void onEndTestSet()
{
	if (!s_verbose)
		tcout << std::endl;

	if (!s_failures.empty())
	{
		tcout << std::endl;
		tcout << TXT(" ") << s_failures.size() << TXT(" failure(s)") << std::endl;

		TestCaseNames::const_iterator it = s_failures.begin();
		TestCaseNames::const_iterator end = s_failures.end();

		for (; it != end; ++it)
		{
			tcout << TXT(" > ") << *it << std::endl;
		}
	}

	tcout << std::endl;

#ifdef _DEBUG
	s_currentTestSet.clear();
	s_failures.clear();
#endif
}

////////////////////////////////////////////////////////////////////////////////
//! Start a new test case.

void onStartTestCase(const tchar* name)
{
	s_currentResult = UNKNOWN;
	s_currentTestCase = name;
}

////////////////////////////////////////////////////////////////////////////////
//! End a test case.

void onEndTestCase()
{
	// Update stats.
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

		if (!s_verbose)
			tcout << TXT("F");
	}
	else
	{
		ASSERT(s_currentResult == UNKNOWN);

		++s_numUnknown;

		if (!s_verbose)
			tcout << TXT("?");
	}

#ifdef _DEBUG
	s_currentResult = UNKNOWN;
	s_currentTestCase.clear();
#endif
}

////////////////////////////////////////////////////////////////////////////////
//! Get just the filename from the path.

const char* getFileName(const char* path)
{
	const char* filename = strrchr(path, '\\');

	if (filename != nullptr)
		++filename;
	else
		filename = filename;

	return filename;
}

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

	if (s_verbose)
	{
		const tchar* result = (passed) ? TXT("Passed") : TXT("FAILED");
		const char*  filename = getFileName(file);

		tcout << Core::fmt(TXT(" %s [%hs, %4u] %s"), result, filename, line, expression) << std::endl;
	}

	// Break into debugger, if present.
	if (!passed && ::IsDebuggerPresent())
		::DebugBreak();
}

////////////////////////////////////////////////////////////////////////////////
//! Process an unexpected exception running a test case.

void processTestException(const char* file, size_t line, const tchar* error)
{
	s_currentResult = FAILED;

	if (s_verbose)
	{
		Core::debugWrite(TXT("Unhandled Exception: %s\n"), error);

		const char* filename = getFileName(file);

		tcout << Core::fmt(TXT(" %s [%hs, %4u] Unhandled Exception: %s"), TXT("FAILED"), filename, line, error) << std::endl;
	}

	if (::IsDebuggerPresent())
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
