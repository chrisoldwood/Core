////////////////////////////////////////////////////////////////////////////////
//! \file   CmdLineParserTests.cpp
//! \brief  The unit tests for the CmdLineParser class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/CmdLineParser.hpp>
#include <Core/CmdLineException.hpp>

#ifdef __GNUG__
// deprecated conversion from string constant to 'tchar*'
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

TEST_SET(CmdLineParser)
{
	enum SwitchID
	{
		SHORT_ONLY,
		LONG_ONLY,
		SHORT_LONG,
		USAGE,
		FLAG,
		SINGLE,
		MULTIPLE,
		INTERNAL,
	};

	static Core::CmdLineSwitch s_aoSwitches[] =
	{
		{ SHORT_ONLY,	TXT("short"),	NULL,			Core::CmdLineSwitch::MANY,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Short switch")		},
		{ LONG_ONLY,	NULL,			TXT("long"),	Core::CmdLineSwitch::MANY,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Long switch")		},
		{ SHORT_LONG,	TXT("b"),		TXT("both"),	Core::CmdLineSwitch::MANY,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Both types")		},
		{ USAGE,		TXT("?"),		NULL,			Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Usage")			},
		{ FLAG,			TXT("f"),		TXT("flag"),	Core::CmdLineSwitch::MANY,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Flag type")		},
		{ SINGLE,		TXT("s"),		TXT("single"),	Core::CmdLineSwitch::MANY,	Core::CmdLineSwitch::SINGLE,	TXT("param"),	TXT("Single value")		},
		{ MULTIPLE,		TXT("m"),		TXT("multi"),	Core::CmdLineSwitch::MANY,	Core::CmdLineSwitch::MULTIPLE,	TXT("params"),	TXT("Multiple values")	},
		{ INTERNAL,		TXT("@"),		NULL,			Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		NULL,			NULL					},
	};
	static size_t s_nCount = ARRAY_SIZE(s_aoSwitches);

TEST_CASE("initial state contains no arguments")
{
	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	TEST_TRUE(parser.getNamedArgs().empty());
	TEST_TRUE(parser.getUnnamedArgs().empty());
	TEST_FALSE(parser.isSwitchSet(FLAG));
}
TEST_CASE_END

TEST_CASE("invalid switch names cause an exception to be thrown")
{
	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	TEST_THROWS(parser.parse(0, nullptr));

	static tchar* argv[] = { TXT("program.exe"), TXT("-invalid_switch") };
	static int argc = ARRAY_SIZE(argv);

	TEST_THROWS(parser.parse(argc, argv));
}
TEST_CASE_END

TEST_CASE("default flags allows both windows and unix format switches and unamed arguments")
{
	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	static tchar* argv[] = { TXT("program.exe"), TXT("-short"), TXT("/short"), TXT("--long"), TXT("/long"), TXT("-b"), TXT("--both"), TXT("/both"), TXT("unnamed") };
	static int argc = ARRAY_SIZE(argv);

	parser.parse(argc, argv);

	TEST_TRUE(parser.getNamedArgs().size() == 3);
	TEST_TRUE(parser.getUnnamedArgs().size() == 1);
	TEST_TRUE(parser.isSwitchSet(SHORT_ONLY) && parser.isSwitchSet(LONG_ONLY) && parser.isSwitchSet(SHORT_LONG));
	TEST_TRUE(!parser.isSwitchSet(FLAG));
}
TEST_CASE_END

TEST_CASE("single use flag switches throw an exception when used multiple times")
{
	static tchar* argv[] = { TXT("program.exe"), TXT("unamed"), TXT("-?"), TXT("-?") };
	static int argc = ARRAY_SIZE(argv);

	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	TEST_THROWS(parser.parse(argc, argv));
}
TEST_CASE_END

TEST_CASE("strings beginning with a dash are treated as values when windows only format specified")
{
	static tchar* argv[] = { TXT("program.exe"), TXT("unamed"), TXT("-?"), TXT("-?") };
	static int argc = ARRAY_SIZE(argv);

	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	parser.parse(argc, argv, Core::CmdLineParser::ALLOW_WIN_FORMAT | Core::CmdLineParser::ALLOW_UNNAMED);

	TEST_TRUE(parser.getUnnamedArgs().size() == 3);
}
TEST_CASE_END

TEST_CASE("strings beginning with a slash are treated as values when unix only format specified")
{
	static tchar* argv[] = { TXT("program.exe"), TXT("unamed"), TXT("/?"), };
	static int argc = ARRAY_SIZE(argv);

	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	parser.parse(argc, argv, Core::CmdLineParser::ALLOW_UNIX_FORMAT | Core::CmdLineParser::ALLOW_UNNAMED);

	TEST_TRUE(parser.getUnnamedArgs().size() == 2);
}
TEST_CASE_END

TEST_CASE("flag based switches that include a value throw an exception")
{
	static tchar* argv[] = { TXT("program.exe"), TXT("/flag:value") };
	static int argc = ARRAY_SIZE(argv);

	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	TEST_THROWS(parser.parse(argc, argv));
}
TEST_CASE_END

TEST_CASE("value based switches without a value throw an exception")
{
	static tchar* argv[] = { TXT("program.exe"), TXT("/single") };
	static int argc = ARRAY_SIZE(argv);

	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	TEST_THROWS(parser.parse(argc, argv));
}
TEST_CASE_END

TEST_CASE("windows style switches can include the value when separated by a colon")
{
	static tchar* argv[] = { TXT("program.exe"), TXT("/s:value1"), TXT("/s"), TXT("value2"), TXT("/single:value3"), TXT("/single"), TXT("value4") };
	static int argc = ARRAY_SIZE(argv);

	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	parser.parse(argc, argv, Core::CmdLineParser::ALLOW_WIN_FORMAT);

	const Core::CmdLineParser::NamedArgs&          mapArgs = parser.getNamedArgs();
	Core::CmdLineParser::NamedArgs::const_iterator it  = mapArgs.find(SINGLE);
	const Core::CmdLineParser::StringVector&       vec = it->second;

	TEST_TRUE((vec.size() == 4) && (vec[0] == TXT("value1")) && (vec[1] == TXT("value2")) && (vec[2] == TXT("value3")) && (vec[3] == TXT("value4")) );
}
TEST_CASE_END

TEST_CASE("unix style switches can include the value when separated by an equals sign")
{
	static tchar* argv[] = { TXT("program.exe"), TXT("-s=value4"), TXT("-s"), TXT("value3"), TXT("--single=value2"), TXT("--single"), TXT("value1") };
	static int argc = ARRAY_SIZE(argv);

	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	parser.parse(argc, argv, Core::CmdLineParser::ALLOW_UNIX_FORMAT);

	const Core::CmdLineParser::NamedArgs&          mapArgs = parser.getNamedArgs();
	Core::CmdLineParser::NamedArgs::const_iterator it  = mapArgs.find(SINGLE);
	const Core::CmdLineParser::StringVector&       vec = it->second;

	TEST_TRUE((vec.size() == 4) && (vec[0] == TXT("value4")) && (vec[1] == TXT("value3")) && (vec[2] == TXT("value2")) && (vec[3] == TXT("value1")) );
}
TEST_CASE_END

TEST_CASE("multiple value lists are terminated by another switch")
{
	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	const Core::CmdLineParser::NamedArgs& mapArgs = parser.getNamedArgs();

	static tchar* argv[] = { TXT("program.exe"), TXT("-m"), TXT("value1"), TXT("value2"), TXT("--multi"), TXT("value3"), TXT("value4"), TXT("--flag") };
	static int argc = ARRAY_SIZE(argv);

	parser.parse(argc, argv);

	Core::CmdLineParser::NamedArgs::const_iterator it  = mapArgs.find(MULTIPLE);
	const Core::CmdLineParser::StringVector&       vec = it->second;

	TEST_TRUE((vec.size() == 4) && (vec[0] == TXT("value1")) && (vec[1] == TXT("value2")) && (vec[2] == TXT("value3")) && (vec[3] == TXT("value4")) );
	TEST_TRUE(mapArgs.find(FLAG) != mapArgs.end());
	TEST_TRUE(parser.getUnnamedArgs().empty());
}
TEST_CASE_END

TEST_CASE("multiple value lists are terminated by the end of the command line")
{
	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	const Core::CmdLineParser::NamedArgs& mapArgs = parser.getNamedArgs();

	static tchar* argv[] = { TXT("program.exe"), TXT("--multi"), TXT("value") };
	static int argc = ARRAY_SIZE(argv);

	parser.parse(argc, argv, Core::CmdLineParser::ALLOW_UNIX_FORMAT); // !ALLOW_UNNAMED.

	Core::CmdLineParser::NamedArgs::const_iterator it  = mapArgs.find(MULTIPLE);
	const Core::CmdLineParser::StringVector&       vec = it->second;

	TEST_TRUE((vec.size() == 1) && (vec[0] == TXT("value")));
	TEST_TRUE(parser.getUnnamedArgs().empty());
	TEST_TRUE(parser.getSwitchValue(MULTIPLE) == TXT("value"));
}
TEST_CASE_END

TEST_CASE("reusing the parser resets the state")
{
	static tchar* argv[] = { TXT("program.exe"), TXT("-short"), TXT("unnamed") };
	static int argc = ARRAY_SIZE(argv);

	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	parser.parse(argc, argv);

	TEST_FALSE(parser.getNamedArgs().empty());
	TEST_FALSE(parser.getUnnamedArgs().empty());

	static tchar* argv2[] = { TXT("program.exe") };
	static int argc2 = ARRAY_SIZE(argv2);

	parser.parse(argc2, argv2);

	TEST_TRUE(parser.getNamedArgs().empty());
	TEST_TRUE(parser.getUnnamedArgs().empty());
}
TEST_CASE_END

TEST_CASE("formatting switch list in unix style uses dashes")
{
	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	const tchar* expected =
	TXT("-short                     Short switch\n")
	TXT("--long                     Long switch\n")
	TXT("-b | --both                Both types\n")
	TXT("-?                         Usage\n")
	TXT("-f | --flag                Flag type\n")
	TXT("-s | --single <param>      Single value\n")
	TXT("-m | --multi <params> ...  Multiple values\n");

	tstring actual = parser.formatSwitches(Core::CmdLineParser::UNIX);

//	TRACE1(TXT("\n%s\n"), strUsage1.c_str());
	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("formatting switch list in windows style uses slashes")
{
	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	const tchar* expected =
	TXT("/short                    Short switch\n")
	TXT("/long                     Long switch\n")
	TXT("/b | /both                Both types\n")
	TXT("/?                        Usage\n")
	TXT("/f | /flag                Flag type\n")
	TXT("/s | /single <param>      Single value\n")
	TXT("/m | /multi <params> ...  Multiple values\n");

	tstring actual = parser.formatSwitches(Core::CmdLineParser::WINDOWS);

//	TRACE1(TXT("\n%s\n"), strUsage2.c_str());
	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("switches longer than the short name will be rejected")
{
	static tchar* argv[] = { TXT("program.exe"), TXT("-fl") };
	static int argc = ARRAY_SIZE(argv);

	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	TEST_THROWS(parser.parse(argc, argv));
}
TEST_CASE_END

TEST_CASE("switches shorter than the long name will be rejected")
{
	static tchar* argv[] = { TXT("program.exe"), TXT("--fl") };
	static int argc = ARRAY_SIZE(argv);

	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	TEST_THROWS(parser.parse(argc, argv));
}
TEST_CASE_END

TEST_CASE("switches longer than the long name will be rejected")
{
	static tchar* argv[] = { TXT("program.exe"), TXT("--flaggg") };
	static int argc = ARRAY_SIZE(argv);

	Core::CmdLineParser parser(s_aoSwitches, s_aoSwitches+s_nCount);

	TEST_THROWS(parser.parse(argc, argv));
}
TEST_CASE_END

}
TEST_SET_END
