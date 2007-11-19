////////////////////////////////////////////////////////////////////////////////
//! \file   WinTargets.hpp
//! \brief  Configure the Windows versions the build can support.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_WINTARGETS_HPP
#define CORE_WINTARGETS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// These definitions control what versions of Windows we are targetting.

#define WINVER			0x0400		//! Target Windows 95+
#define _WIN32_WINNT	0x0400		//! Target Windows NT 4.0+
#define _WIN32_WINDOWS	0x0400		//! Target Windows 95+
#define _WIN32_IE		0x0400		//! Target Internet Explorer 4.0+

#endif // CORE_WINTARGETS_HPP
