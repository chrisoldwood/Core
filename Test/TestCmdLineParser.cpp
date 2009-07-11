////////////////////////////////////////////////////////////////////////////////
//! \file   TestCmdLineParser.cpp
//! \brief  The unit tests for the CmdLineParser class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/CmdLineParser.hpp>
#include <Core/CmdLineException.hpp>

#ifdef __GNUG__
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the CmdLineParser class.

void testCmdLineParser()
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

{
	Core::CmdLineParser oParser(s_aoSwitches, s_aoSwitches+s_nCount);

	TEST_THROWS(oParser.parse(0, nullptr));

	static tchar* argv[] = { TXT("program.exe"), TXT("-invalid_switch") };
	static int argc = ARRAY_SIZE(argv);

	TEST_THROWS(oParser.parse(argc, argv));
}

{
	Core::CmdLineParser oParser(s_aoSwitches, s_aoSwitches+s_nCount);

	TEST_TRUE(oParser.getNamedArgs().empty() && oParser.getUnnamedArgs().empty());

	static tchar* argv[] = { TXT("program.exe"), TXT("-short"), TXT("/short"), TXT("--long"), TXT("/long"), TXT("-b"), TXT("--both"), TXT("/both"), TXT("unnamed") };
	static int argc = ARRAY_SIZE(argv);

	oParser.parse(argc, argv);

	TEST_TRUE(oParser.getNamedArgs().size() == 3);
	TEST_TRUE(oParser.getUnnamedArgs().size() == 1);
	TEST_TRUE(oParser.isSwitchSet(SHORT_ONLY) && oParser.isSwitchSet(LONG_ONLY) && oParser.isSwitchSet(SHORT_LONG));
	TEST_TRUE(!oParser.isSwitchSet(FLAG));

	static tchar* argv2[] = { TXT("program.exe") };
	static int argc2 = ARRAY_SIZE(argv2);

	oParser.parse(argc2, argv2);

	TEST_TRUE(oParser.getNamedArgs().empty() && oParser.getUnnamedArgs().empty());
}

{
	static tchar* argv[] = { TXT("program.exe"), TXT("unamed"), TXT("-?"), TXT("-?") };
	static int argc = ARRAY_SIZE(argv);

	Core::CmdLineParser oParser(s_aoSwitches, s_aoSwitches+s_nCount);

	TEST_THROWS(oParser.parse(argc, argv, Core::CmdLineParser::ALLOW_ANY_FORMAT));

	TEST_THROWS(oParser.parse(argc, argv));

	oParser.parse(argc, argv, Core::CmdLineParser::ALLOW_WIN_FORMAT | Core::CmdLineParser::ALLOW_UNNAMED);

	TEST_TRUE(oParser.getUnnamedArgs().size() == 3);

	static tchar* argv2[] = { TXT("program.exe"), TXT("unamed"), TXT("/?"), };
	static int argc2 = ARRAY_SIZE(argv2);

	oParser.parse(argc2, argv2, Core::CmdLineParser::ALLOW_UNIX_FORMAT | Core::CmdLineParser::ALLOW_UNNAMED);

	TEST_TRUE(oParser.getUnnamedArgs().size() == 2);
}

{
	static tchar* argv[] = { TXT("program.exe"), TXT("/flag:value") };
	static int argc = ARRAY_SIZE(argv);

	Core::CmdLineParser oParser(s_aoSwitches, s_aoSwitches+s_nCount);

	TEST_THROWS(oParser.parse(argc, argv));
}

{
	Core::CmdLineParser oParser(s_aoSwitches, s_aoSwitches+s_nCount);

	const Core::CmdLineParser::NamedArgs& mapArgs = oParser.getNamedArgs();

	static tchar* argv1[] = { TXT("program.exe"), TXT("/s:value1"), TXT("/s"), TXT("value2"), TXT("/single:value3"), TXT("/single"), TXT("value4") };
	static int argc1 = ARRAY_SIZE(argv1);

	oParser.parse(argc1, argv1, Core::CmdLineParser::ALLOW_WIN_FORMAT);

	Core::CmdLineParser::NamedArgs::const_iterator it1  = mapArgs.find(SINGLE);
	const Core::CmdLineParser::StringVector&       vec1 = it1->second;

	TEST_TRUE((vec1.size() == 4) && (vec1[0] == TXT("value1")) && (vec1[1] == TXT("value2")) && (vec1[2] == TXT("value3")) && (vec1[3] == TXT("value4")) );

	static tchar* argv2[] = { TXT("program.exe"), TXT("-s=value4"), TXT("-s"), TXT("value3"), TXT("--single=value2"), TXT("--single"), TXT("value1") };
	static int argc2 = ARRAY_SIZE(argv2);

	oParser.parse(argc2, argv2, Core::CmdLineParser::ALLOW_UNIX_FORMAT);

	Core::CmdLineParser::NamedArgs::const_iterator it2  = mapArgs.find(SINGLE);
	const Core::CmdLineParser::StringVector&       vec2 = it2->second;

	TEST_TRUE((vec2.size() == 4) && (vec2[0] == TXT("value4")) && (vec2[1] == TXT("value3")) && (vec2[2] == TXT("value2")) && (vec2[3] == TXT("value1")) );
}

{
	Core::CmdLineParser oParser(s_aoSwitches, s_aoSwitches+s_nCount);

	const Core::CmdLineParser::NamedArgs& mapArgs = oParser.getNamedArgs();

	static tchar* argv[] = { TXT("program.exe"), TXT("-m"), TXT("value1"), TXT("value2"), TXT("--multi"), TXT("value3"), TXT("value4"), TXT("--flag") };
	static int argc = ARRAY_SIZE(argv);

	oParser.parse(argc, argv);

	Core::CmdLineParser::NamedArgs::const_iterator it  = mapArgs.find(MULTIPLE);
	const Core::CmdLineParser::StringVector&       vec = it->second;

	TEST_TRUE((vec.size() == 4) && (vec[0] == TXT("value1")) && (vec[1] == TXT("value2")) && (vec[2] == TXT("value3")) && (vec[3] == TXT("value4")) );
	TEST_TRUE(mapArgs.find(FLAG) != mapArgs.end());
	TEST_TRUE(oParser.getUnnamedArgs().empty());
}

{
	Core::CmdLineParser oParser(s_aoSwitches, s_aoSwitches+s_nCount);

	const Core::CmdLineParser::NamedArgs& mapArgs = oParser.getNamedArgs();

	static tchar* argv[] = { TXT("program.exe"), TXT("--multi"), TXT("value") };
	static int argc = ARRAY_SIZE(argv);

	oParser.parse(argc, argv, Core::CmdLineParser::ALLOW_UNIX_FORMAT); // !ALLOW_UNNAMED.

	Core::CmdLineParser::NamedArgs::const_iterator it  = mapArgs.find(MULTIPLE);
	const Core::CmdLineParser::StringVector&       vec = it->second;

	TEST_TRUE((vec.size() == 1) && (vec[0] == TXT("value")));
	TEST_TRUE(oParser.getUnnamedArgs().empty());
}

{
	Core::CmdLineParser oParser(s_aoSwitches, s_aoSwitches+s_nCount);

	const tchar* psz1 =
	TXT("-short                     Short switch\n")
	TXT("--long                     Long switch\n")
	TXT("-b | --both                Both types\n")
	TXT("-?                         Usage\n")
	TXT("-f | --flag                Flag type\n")
	TXT("-s | --single <param>      Single value\n")
	TXT("-m | --multi <params> ...  Multiple values\n");

	tstring strUsage1 = oParser.formatSwitches(Core::CmdLineParser::UNIX);

//	TRACE1(TXT("\n%s\n"), strUsage1.c_str());
	TEST_TRUE(strUsage1 == psz1);

	const tchar* psz2 =
	TXT("/short                    Short switch\n")
	TXT("/long                     Long switch\n")
	TXT("/b | /both                Both types\n")
	TXT("/?                        Usage\n")
	TXT("/f | /flag                Flag type\n")
	TXT("/s | /single <param>      Single value\n")
	TXT("/m | /multi <params> ...  Multiple values\n");

	tstring strUsage2 = oParser.formatSwitches(Core::CmdLineParser::WINDOWS);

//	TRACE1(TXT("\n%s\n"), strUsage2.c_str());
	TEST_TRUE(strUsage2 == psz2);
}

}
