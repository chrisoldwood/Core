////////////////////////////////////////////////////////////////////////////////
//! \file   FileSystem.cpp
//! \brief  FileSystem helper functions.
//! \author Chris Oldwood

#include "Common.hpp"
#include "FileSystem.hpp"
#include "BadLogicException.hpp"
#include "StringUtils.hpp"
#include "FileSystemException.hpp"
#include "Scoped.hpp"
#include <windows.h>
#include <malloc.h>
#include <io.h>
#include <tchar.h>
#include <direct.h>

#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2)) // GCC 4.2+
#pragma GCC diagnostic ignored "-Wunused-value"
#endif

////////////////////////////////////////////////////////////////////////////////
// Localised TCHAR mappings for the file-system related functions.

#ifdef ANSI_BUILD
#define tstrerror	strerror
#define tmkdir		_mkdir
#define trmdir		_rmdir
#else // UNICODE_BUILD
#define tstrerror	_wcserror
#define tmkdir		_wmkdir
#define trmdir		_wrmdir
#endif

namespace Core
{

const tchar FOLDER_SEPARATOR = TXT('\\');

////////////////////////////////////////////////////////////////////////////////
//! Get the folder to use for temporary files.

tstring getTempFolder()
{
	DWORD   length = ::GetTempPath(0, nullptr);
	size_t  bytes = Core::numBytes<tchar>(length);
	tchar*	buffer = static_cast<tchar*>(_alloca(bytes));
	DWORD   result = ::GetTempPath(length, buffer);

	if (result > length)
		throw BadLogicException(fmt(TXT("Invalid buffer size used in ::GetTempPath(). Result: %lu"), result));

	return buffer;
}

////////////////////////////////////////////////////////////////////////////////
//! Combine two paths into a single path.

tstring combinePaths(const tstring& lhs, const tstring& rhs)
{
	tstring path = lhs;

	if (path.find_last_of(FOLDER_SEPARATOR) != (path.length()-1))
		path += FOLDER_SEPARATOR;

	return path + rhs;
}

////////////////////////////////////////////////////////////////////////////////
//! Query if the path points to a file or folder that exists.

bool pathExists(const tstring& path)
{
	return (_taccess(path.c_str(), 0) == 0);
}

////////////////////////////////////////////////////////////////////////////////
//! Wrapper to invoke LocalFree on the buffer pointer.

static void localFree(tchar* buffer)
{
	HLOCAL handle = ::LocalFree(buffer);

	ASSERT(handle == NULL);
	DEBUG_USE_ONLY(handle);
}

////////////////////////////////////////////////////////////////////////////////
//! Convert the WIN32 error code into a text message.

static tstring formatWin32ErrorMessage(DWORD errorCode)
{
	typedef Core::Scoped<tchar*> BufferPtr;

	tchar* buffer;

	DWORD result = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
									nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
									reinterpret_cast<tchar*>(&buffer), 0, nullptr);

	if (result == 0)
		return TXT("Failed to format message with::FormatMessage()");

	ASSERT(buffer != nullptr);

	BufferPtr managedBuffer(buffer, localFree);
	tstring   message(buffer);

	trim(message);

	return message;
}

////////////////////////////////////////////////////////////////////////////////
//! Create the specified folder.

void createFolder(const tstring& path)
{
	int result = tmkdir(path.c_str());

	if (result != 0)
	{
		int     errorCode = errno;
		tstring errorText = tstrerror(errorCode);

		tstring operation = Core::fmt(TXT("Failed to create folder '%s'"), path.c_str());
		tstring message = Core::fmt(TXT("%s [%d - %s]"), operation.c_str(), errorCode, errorText.c_str());

		throw FileSystemException(message);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Delete the specified folder.

void deleteFolder(const tstring& path, bool ignoreErrors)
{
	int result = trmdir(path.c_str());

	if ( (result != 0) && (!ignoreErrors) )
	{
		int     errorCode = errno;
		tstring errorText = tstrerror(errorCode);

		tstring operation = Core::fmt(TXT("Failed to delete folder '%s'"), path.c_str());
		tstring message = Core::fmt(TXT("%s [%d - %s]"), operation.c_str(), errorCode, errorText.c_str());

		throw FileSystemException(message);
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Delete the specified file.

void deleteFile(const tstring& path, bool ignoreErrors)
{
	BOOL result = ::DeleteFile(path.c_str());

	if ( (result == 0) && (!ignoreErrors) )
	{
		DWORD   errorCode = ::GetLastError();
		tstring errorText = formatWin32ErrorMessage(errorCode);

		tstring operation = Core::fmt(TXT("Failed to delete file '%s'"), path.c_str());
		tstring message = Core::fmt(TXT("%s [0x%08lX - %s]"), operation.c_str(), errorCode, errorText.c_str());

		throw FileSystemException(message);
	}
}

//namespace Core
}
