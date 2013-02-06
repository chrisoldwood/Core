////////////////////////////////////////////////////////////////////////////////
//! \file   AlgorithmTests.cpp
//! \brief  The unit tests for the Algorithm functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/Algorithm.hpp>

TEST_SET(Algorithm)
{

TEST_CASE("exists returns true when it finds the item in the sequence")
{
	int              values[] = {1, 2, 3};
	std::vector<int> array(values, values+ARRAY_SIZE(values));

	TEST_TRUE(Core::exists(array, 2));
}
TEST_CASE_END

TEST_CASE("exists returns false when it cannot find the item in the sequence")
{
	int              values[] = {1, 2, 3};
	std::vector<int> array(values, values+ARRAY_SIZE(values));

	TEST_FALSE(Core::exists(array, 5));
}
TEST_CASE_END

TEST_CASE("deep copying a vector creates an equivalent sequence with different object references")
{
	typedef Core::SharedPtr<int> IntPtr;

	struct comparator
	{
		static bool equalStateNotReference(const IntPtr& p1, const IntPtr& p2)
		{
			return ((*p1 == *p2) && (p1.get() != p2.get()));
		}
	};

	std::vector<IntPtr> array1, array2;

	array1.push_back(IntPtr(new int(1)));
	array1.push_back(IntPtr(new int(2)));
	array1.push_back(IntPtr(new int(3)));

	Core::deepCopy(array1, array2);

	TEST_TRUE(std::equal(array1.begin(), array1.end(), array2.begin(), comparator::equalStateNotReference));
}
TEST_CASE_END

}
TEST_SET_END
