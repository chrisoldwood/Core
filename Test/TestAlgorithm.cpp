////////////////////////////////////////////////////////////////////////////////
//! \file   TestSharedPtr.cpp
//! \brief  The unit tests for the Algorithm functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/Algorithm.hpp>

typedef Core::SharedPtr<int> IntPtr;

////////////////////////////////////////////////////////////////////////////////
//! Function to compare ptr address and values.

bool isEqualValueNotPtr(const IntPtr& p1, IntPtr& p2)
{
	return ((*p1 == *p2) && (p1.get() != p2.get()));
}

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the Algorithm functions.

void testAlgorithm()
{
{
	int              values[] = {1, 2, 3};
	std::vector<int> array(values, values+ARRAY_SIZE(values));

	TEST_TRUE(Core::exists(array, 2));
	TEST_FALSE(Core::exists(array, 5));
}
{
	std::vector<IntPtr> array1, array2;

	array1.push_back(IntPtr(new int(1)));
	array1.push_back(IntPtr(new int(2)));
	array1.push_back(IntPtr(new int(3)));

	Core::deepCopy(array1, array2);

	TEST_TRUE(std::equal(array1.begin(), array1.end(), array2.begin(), isEqualValueNotPtr));
}
}
