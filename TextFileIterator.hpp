////////////////////////////////////////////////////////////////////////////////
//! \file   TextFileIterator.hpp
//! \brief  The TextFileIterator class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_TEXTFILEITERATOR_HPP
#define CORE_TEXTFILEITERATOR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "UniquePtr.hpp"
#include "tfstream.hpp"

#ifdef __GNUG__
// base class 'X' has a non-virtual destructor
#pragma GCC diagnostic ignored "-Weffc++"
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! The iterator type used to read lines of text from a file.

class TextFileIterator
{
public:
	//! Constructor for the End iterator.
	TextFileIterator();

	//! Constructor for the Begin iterator.
	TextFileIterator(const tstring& filename);

	//! Destructor.
	~TextFileIterator();

	//
	// Operators.
	//

	//! Dereference operator.
	const tstring& operator*() const;

	//! Pointer-to-member operator.
	const tstring* operator->() const;

	//! Advance the iterator.
	TextFileIterator& operator++();

	//
	// Methods.
	//

	//! Compare to another iterator for equivalence.
	bool equals(const TextFileIterator& rhs) const;

private:
	//! The underlying input file stream.
	typedef UniquePtr<tifstream> StreamPtr;
	//! The current value;
	typedef UniquePtr<tstring> StringPtr;

	//
	// Members.
	//
	StreamPtr	m_stream;	//!< The underlying file stream;
	StringPtr	m_value;	//!< The current iterator value.

	//
	// Internal methods.
	//

	//! Move the iterator forward.
	void increment();

	//! Move the iterator to the End.
	void reset();
};

////////////////////////////////////////////////////////////////////////////////
//! Advance the iterator.

inline TextFileIterator& TextFileIterator::operator++()
{
	increment();

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//! Compare two iterators for equivalence.

inline bool operator==(const TextFileIterator& lhs, const TextFileIterator& rhs)
{
	return lhs.equals(rhs);
}

////////////////////////////////////////////////////////////////////////////////
//! Compare two iterators for difference.

inline bool operator!=(const TextFileIterator& lhs, const TextFileIterator& rhs)
{
	return !lhs.equals(rhs);
}

//namespace Core
}

#endif // CORE_TEXTFILEITERATOR_HPP
