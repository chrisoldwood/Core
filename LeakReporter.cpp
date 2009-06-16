////////////////////////////////////////////////////////////////////////////////
//! \file   LeakReporter.cpp
//! \brief  The special static object used to report memory leaks.
//! \author Chris Oldwood

#include "Common.hpp"

#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2)) // GCC 4.2+
// Caused by the DEBUG_USE_ONLY macro.
#pragma GCC diagnostic ignored "-Wunused-value"
#endif

////////////////////////////////////////////////////////////////////////////////
// Avoid bringing in <windows.h>.

#ifndef _STLPORT_VERSION
extern "C" void __stdcall OutputDebugStringA(const char*);
#endif
extern "C" void __stdcall OutputDebugStringW(const wchar_t*);

namespace Core
{

#ifdef _DEBUG
////////////////////////////////////////////////////////////////////////////////
// Ensure we dump memory leaks after STLport has performed its clean up.

#ifdef CORE_CRTDBG_ENABLED
#pragma warning(disable : 4073 4074)
#pragma init_seg(compiler)
#endif

//! The flag used to signal whether to dump leaks or not.
static bool g_bReportLeaks = false;
#endif

////////////////////////////////////////////////////////////////////////////////
//! Enable or disable memory leak reporting.
//! NB: This function is available in all builds, but does nothing in Release.

void EnableLeakReporting(bool bEnable)
{
	DEBUG_USE_ONLY(bEnable);

#ifdef _DEBUG
	g_bReportLeaks = bEnable;
#endif
}

#ifdef _DEBUG

////////////////////////////////////////////////////////////////////////////////
// Avoid bringing in <windows.h>.

#ifndef _STLPORT_VERSION
extern "C" void __stdcall OutputDebugStringA(const char*);
#endif
extern "C" void __stdcall OutputDebugStringW(const wchar_t*);

#ifdef ANSI_BUILD
#define	OutputDebugString	OutputDebugStringA
#else
#define	OutputDebugString	OutputDebugStringW
#endif

////////////////////////////////////////////////////////////////////////////////
//! The static object that uses it's dtor to report any memory leaks. This
//! object must be the last one to be destroyed.

class LeakReporter
{
public:
	//! Default consttructor.
	LeakReporter();

	//! Destructor.
	~LeakReporter();

} g_oReporter;

////////////////////////////////////////////////////////////////////////////////
//! Default consttructor.

LeakReporter::LeakReporter()
{
#ifdef CORE_CRTDBG_ENABLED
//	_CrtSetBreakAlloc(63);
#endif
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

LeakReporter::~LeakReporter()
{
	if (g_bReportLeaks)
	{
#ifdef CORE_CRTDBG_ENABLED
		::OutputDebugString(TXT("Leak reporting enabled.\n"));
		_CrtDumpMemoryLeaks();
#endif
	}
}

#endif // _DEBUG

//namespace Core
}
