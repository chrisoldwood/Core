////////////////////////////////////////////////////////////////////////////////
//! \file   BuildConfig.hpp
//! \brief  Definitions that configure the build.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_BUILDCONFIG_HPP
#define CORE_BUILDCONFIG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// These defintions control what versions of Windows we are targetting.

#define WINVER			0x0400		//! Target Windows 95+
#define _WIN32_WINNT	0x0400		//! Target Windows NT 4.0+
#define _WIN32_WINDOWS	0x0400		//! Target Windows 95+
#define _WIN32_IE		0x0400		//! Target Internet Explorer 4.0+

////////////////////////////////////////////////////////////////////////////////
// These additional defintions control other aspects of <windows.h>

#define STRICT						//! Ensures handles are distinct types.
#define VC_EXTRALEAN				//! Trim uncommon portions of <windows.h>

#endif // CORE_BUILDCONFIG_HPP