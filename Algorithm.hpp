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
//! Search to see if the value exists within the container.

template<typename T>
inline bool exists(const std::vector<T>& container, T value)
{
	return (std::find(container.begin(), container.end(), value) != container.end());
}

////////////////////////////////////////////////////////////////////////////////
//! Search to see if the value exists within the container.

template<typename T>
inline bool exists(const std::set<T>& container, T value)
{
	return (container.find(value) != container.end());
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
//! Remove items from a vector that match a predicate.

template<typename T, typename F>
inline void erase_if(std::vector<T>& container, F predicate)
{
	typename std::vector<T>::iterator it = std::remove_if(container.begin(), container.end(), predicate);

	container.erase(it, container.end());
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

////////////////////////////////////////////////////////////////////////////////
//! Find a value within a container using a predicate.

template<typename T, typename P>
inline typename std::vector<T>::const_iterator find_if(const std::vector<T>& container, P predicate)
{
	return std::find_if(container.begin(), container.end(), predicate);
}

//namespace Core
}

#endif // CORE_ALGORITHM_HPP
