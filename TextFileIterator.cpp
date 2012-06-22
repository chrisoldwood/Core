////////////////////////////////////////////////////////////////////////////////
//! \file   TextFileIterator.cpp
//! \brief  The TextFileIterator class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "TextFileIterator.hpp"
#include "BadLogicException.hpp"
#include "AnsiWide.hpp"
#include "FileSystemException.hpp"
#include "StringUtils.hpp"

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Constructor for the End iterator.

TextFileIterator::TextFileIterator()
	: m_stream()
	, m_value()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Constructor for the Begin iterator.

TextFileIterator::TextFileIterator(const tstring& filename)
	: m_stream()
	, m_value()
{
	m_stream.reset(new tifstream(T2A(filename)));

	if (!m_stream->is_open())
		throw FileSystemException(Core::fmt(TXT("Failed to open file '%s'"), filename.c_str()));

	m_value.reset(new tstring);

	increment();
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

TextFileIterator::~TextFileIterator()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Dereference operator.

const tstring& TextFileIterator::operator*() const
{
	if (m_value.get() == nullptr)
		throw BadLogicException(TXT("Attempted to dereference end iterator"));

	return *m_value;
}

////////////////////////////////////////////////////////////////////////////////
//! Pointer-to-member operator.

const tstring* TextFileIterator::operator->() const
{
	if (m_value.get() == nullptr)
		throw BadLogicException(TXT("Attempted to dereference end iterator"));

	return m_value.get();
}

////////////////////////////////////////////////////////////////////////////////
//! Compare to another iterator for equivalence.

bool TextFileIterator::equals(const TextFileIterator& rhs) const
{
	if (m_stream.get() == nullptr)
		return (rhs.m_stream.get() == nullptr);

	return (m_stream.get() == rhs.m_stream.get());
}

////////////////////////////////////////////////////////////////////////////////
//! Move the iterator forward.

void TextFileIterator::increment()
{
	if (m_stream.get() == nullptr)
		throw BadLogicException(TXT("Attempted to increment end iterator"));

	ASSERT(m_value.get() != nullptr);

	std::getline(*m_stream, *m_value);

	if (!(*m_stream))
		reset();
}

////////////////////////////////////////////////////////////////////////////////
//! Move the iterator to the End.

void TextFileIterator::reset()
{
	m_stream.reset();
	m_value.reset();
}

//namespace Core
}
