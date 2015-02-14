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

#define WINVER			0x0502		//! Target Windows XP SP2+
#define _WIN32_WINNT	0x0502		//! Target Windows XP SP2+

#endif // CORE_WINTARGETS_HPP
