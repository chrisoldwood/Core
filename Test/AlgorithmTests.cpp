////////////////////////////////////////////////////////////////////////////////
//! \file   AlgorithmTests.cpp
//! \brief  The unit tests for the Algorithm functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/Algorithm.hpp>

bool ValueIsTwo(int value)
{
	return value == 2;
}

TEST_SET(Algorithm)
{

TEST_CASE("exists returns true when it finds the item in the vector")
{
	int              values[] = {1, 2, 3};
	std::vector<int> array(values, values+ARRAY_SIZE(values));

	TEST_TRUE(Core::exists(array, 2));
}
TEST_CASE_END

TEST_CASE("exists returns false when it cannot find the item in the vector")
{
	int              values[] = {1, 2, 3};
	std::vector<int> array(values, values+ARRAY_SIZE(values));

	TEST_FALSE(Core::exists(array, 5));
}
TEST_CASE_END

TEST_CASE("exists returns true when it finds the item in the set")
{
	int           values[] = {1, 2, 3};
	std::set<int> a_set(values, values+ARRAY_SIZE(values));

	TEST_TRUE(Core::exists(a_set, 2));
}
TEST_CASE_END

TEST_CASE("exists returns false when it cannot find the item in the set")
{
	int           values[] = {1, 2, 3};
	std::set<int> a_set(values, values+ARRAY_SIZE(values));

	TEST_FALSE(Core::exists(a_set, 5));
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

TEST_CASE("values matching a predicate will be erased from the vector")
{
	std::vector<int> container;

	container.push_back(1);
	container.push_back(2);
	container.push_back(3);

	Core::erase_if(container, ValueIsTwo);

	TEST_TRUE(container.size() == 2);
	TEST_TRUE(container[0] == 1);
	TEST_TRUE(container[1] == 3);
}
TEST_CASE_END

TEST_CASE("values not matching a predicate lives the vector intact")
{
	std::vector<int> container;

	container.push_back(1);
	container.push_back(3);

	Core::erase_if(container, ValueIsTwo);

	TEST_TRUE(container.size() == 2);
}
TEST_CASE_END

}
TEST_SET_END
