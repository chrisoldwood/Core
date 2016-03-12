////////////////////////////////////////////////////////////////////////////////
//! \file   Algorithm.hpp
//! \brief  Extensions to the standard \<algorithm\> functions.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_ALGORITHM_HPP
#define CORE_ALGORITHM_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <algorithm>
#include <vector>
#include <set>
#include <map>

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Find a value within a container using a predicate.

template<typename T, typename P>
inline typename std::vector<T>::const_iterator find_if(const std::vector<T>& container, P predicate)
{
	return std::find_if(container.begin(), container.end(), predicate);
}

////////////////////////////////////////////////////////////////////////////////
//! Search to see if the value exists within the container.

template<typename T>
inline bool exists(const std::vector<T>& container, T value)
{
	return (std::find(container.begin(), container.end(), value) != container.end());
}

////////////////////////////////////////////////////////////////////////////////
//! Search to see if the value exists within the container using the predicate.

template<typename T, typename P>
inline bool exists_if(const std::vector<T>& container, P predicate)
{
	return (Core::find_if(container, predicate) != container.end());
}

////////////////////////////////////////////////////////////////////////////////
//! Search to see if the value exists within the container.

template<typename T>
inline bool exists(const std::set<T>& container, T value)
{
	return (container.find(value) != container.end());
}

////////////////////////////////////////////////////////////////////////////////
//! Search to see if the value exists within the container.

template<typename T, typename P>
inline bool exists_if(const std::set<T>& container, P predicate)
{
	return (std::find_if(container.begin(), container.end(), predicate) != container.end());
}

////////////////////////////////////////////////////////////////////////////////
//! Perform a deep copy of the container.

template<typename T>
inline void deepCopy(const std::vector< Core::SharedPtr<T> >& input, std::vector< Core::SharedPtr<T> >& output)
{
	typedef typename std::vector<Core::SharedPtr<T> >::const_iterator ConstIter;

	for (ConstIter it = input.begin(); it != input.end(); ++it)
		output.push_back(Core::SharedPtr<T>(new T(*(*it))));
}

////////////////////////////////////////////////////////////////////////////////
//! Find the index of the first value that matches. Returns npos if no match
//! is found.

template<typename T>
inline size_t indexOf(std::vector<T>& container, T value)
{
	typename std::vector<T>::iterator it = std::find(container.begin(), container.end(), value);

	if (it == container.end())
		return Core::npos;

	return (it - container.begin());
}

////////////////////////////////////////////////////////////////////////////////
//! Remove items from a vector that match a predicate.

template<typename T, typename P>
inline void erase_if(std::vector<T>& container, P predicate)
{
	typename std::vector<T>::iterator it = std::remove_if(container.begin(), container.end(), predicate);

	container.erase(it, container.end());
}

////////////////////////////////////////////////////////////////////////////////
//! Remove an item from a vector by its position.

template<typename T>
inline void eraseAt(std::vector<T>& container, size_t position)
{
	ASSERT(!container.empty());

	container.erase(container.begin() + position);
}

////////////////////////////////////////////////////////////////////////////////
//! Remove an item from a vector by its value.

template<typename T>
inline void eraseValue(std::vector<T>& container, T value)
{
	size_t index = indexOf(container, value);

	if (index != npos)
		eraseAt(container, index);
}

////////////////////////////////////////////////////////////////////////////////
//! Remove and destroy an item from a vector by its position.

template<typename T>
inline void deleteAt(std::vector<T>& container, size_t position)
{
	ASSERT(!container.empty());

	T item = container[position];
	eraseAt(container, position);
	delete item;
}

////////////////////////////////////////////////////////////////////////////////
//! Remove and destroy an item from a vector by its value.

template<typename T>
inline void deleteValue(std::vector<T>& container, T value)
{
	size_t index = indexOf(container, value);

	if (index != npos)
		deleteAt(container, index);
}

////////////////////////////////////////////////////////////////////////////////
//! Remove and destroy all items in a vector.

template<typename T>
inline void deleteAll(std::vector<T>& container)
{
	typedef typename std::vector<T>::const_iterator ConstIter;

	for (ConstIter it = container.begin(); it != container.end(); ++it)
		delete *it;

	container.clear();	
}

////////////////////////////////////////////////////////////////////////////////
//! Find the value in the map or return the default value.

template<typename K, typename V>
inline V findOrDefault(const std::map<K, V>& container, K key, V defaultValue)
{
	typename std::map<K, V>::const_iterator it = container.find(key);

	if (it == container.end())
		return defaultValue;

	return it->second;
}

//namespace Core
}

#endif // CORE_ALGORITHM_HPP
