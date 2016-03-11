////////////////////////////////////////////////////////////////////////////////
//! \file   FileSystem.hpp
//! \brief  FileSystem helper functions.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_FILESYSTEM_HPP
#define CORE_FILESYSTEM_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
// Get the folder to use for temporary files.

tstring getTempFolder();

////////////////////////////////////////////////////////////////////////////////
// Combine two paths into a single path.

tstring combinePaths(const tstring& lhs, const tstring& rhs);

////////////////////////////////////////////////////////////////////////////////
// Query if the path points to a file or folder that exists.

bool pathExists(const tstring& path);

////////////////////////////////////////////////////////////////////////////////
// Create the specified folder.

void createFolder(const tstring& path); // throw(FileSystemExceptionException)

////////////////////////////////////////////////////////////////////////////////
// Delete the specified folder.

void deleteFolder(const tstring& path, bool ignoreErrors = false); // throw(FileSystemExceptionException)

////////////////////////////////////////////////////////////////////////////////
// Delete the specified file.

void deleteFile(const tstring& path, bool ignoreErrors = false); // throw(FileSystemExceptionException)

//namespace Core
}

#endif // CORE_FILESYSTEM_HPP
