////////////////////////////////////////////////////////////////////////////////
//! \file   AlgorithmTests.cpp
//! \brief  The unit tests for the Algorithm functions.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <Core/Algorithm.hpp>
#include <Core/Functor.hpp>

static bool ValueIsTwo(int value)
{
	return value == 2;
}

CORE_DEFINE_PREDICATE(IntValueEquals, int, value, rhs)
{
	return rhs == value;
}
CORE_END_PREDICATE

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

TEST_CASE("the first index of an element in a vector can be found by its value")
{
	std::vector<int> container;

	container.push_back(1);
	container.push_back(2);
	container.push_back(3);

	size_t index = Core::indexOf(container, 2);

	TEST_TRUE(index == 1);
}
TEST_CASE_END

TEST_CASE("failing to find the index of an element by value returns npos")
{
	std::vector<int> container;

	container.push_back(1);
	container.push_back(2);
	container.push_back(3);

	size_t index = Core::indexOf(container, 4);

	TEST_TRUE(index == Core::npos);
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

TEST_CASE("values not matching a predicate leaves the vector intact")
{
	std::vector<int> container;

	container.push_back(1);
	container.push_back(3);

	Core::erase_if(container, ValueIsTwo);

	TEST_TRUE(container.size() == 2);
}
TEST_CASE_END

TEST_CASE("a value can be removed from a vector by its position")
{
	std::vector<int> container;

	container.push_back(1);
	container.push_back(2);
	container.push_back(3);

	Core::eraseAt(container, 1);

	TEST_TRUE(container.size() == 2);
	TEST_TRUE(container[0] == 1);
	TEST_TRUE(container[1] == 3);
}
TEST_CASE_END

TEST_CASE("an element can be removed from a vector by its value")
{
	std::vector<int> container;

	container.push_back(1);
	container.push_back(2);
	container.push_back(3);

	Core::eraseValue(container, 2);

	TEST_TRUE(container.size() == 2);
	TEST_TRUE(container[0] == 1);
	TEST_TRUE(container[1] == 3);
}
TEST_CASE_END

TEST_CASE("trying to remove an element does nothing if the value cannot be found")
{
	std::vector<int> container;

	container.push_back(1);
	container.push_back(2);
	container.push_back(3);

	Core::eraseValue(container, 4);

	TEST_TRUE(container.size() == 3);
}
TEST_CASE_END

TEST_CASE("an object owned by a vector can be destroyed by its position")
{
	std::vector<int*> container;

	container.push_back(new int(1));

	Core::deleteAt(container, 0);

	TEST_TRUE(container.size() == 0);
}
TEST_CASE_END

TEST_CASE("an object owned by a vector can be destroyed by specifying its value")
{
	std::vector<int*> container;

	int* value = new int(1);

	container.push_back(value);

	Core::deleteValue(container, value);

	TEST_TRUE(container.size() == 0);
}
TEST_CASE_END

TEST_CASE("trying to destroy an object owned by a vector does nothing if the value cannot be found")
{
	std::vector<int*> container;

	int* value = nullptr;

	Core::deleteValue(container, value);

	TEST_PASSED("no exception was thrown");
}
TEST_CASE_END

TEST_CASE("all objects owned by a vector can be destroyed together")
{
	std::vector<int*> container;

	container.push_back(new int(1));
	container.push_back(new int(2));
	container.push_back(new int(3));

	Core::deleteAll(container);

	TEST_TRUE(container.size() == 0);
}
TEST_CASE_END

TEST_CASE("finding a key in a map returns the value if the key is found")
{
	std::map<int, int> container;

	const int key = 10;
	const int expectedValue = 100;
	const int defaultValue = 999;

	container[key] = expectedValue;

	const int actual = Core::findOrDefault(container, key, defaultValue);

	TEST_TRUE(actual == expectedValue);
}
TEST_CASE_END

TEST_CASE("finding a key in a map returns the default if the key is not found")
{
	std::map<int, int> container;

	const int key = 10;
	const int defaultValue = 999;

	const int actual = Core::findOrDefault(container, key, defaultValue);

	TEST_TRUE(actual == defaultValue);
}
TEST_CASE_END

TEST_CASE("find_if searches the entire vector")
{
	const int first = 1, last = 3;
	const int values[] = { first, 2, last };

	std::vector<int> array(values, values+ARRAY_SIZE(values));
	std::vector<int>::const_iterator it;

	it = Core::find_if(array, IntValueEquals(first));

	TEST_TRUE(*it == first);

	it = Core::find_if(array, IntValueEquals(last));

	TEST_TRUE(*it == last);
}
TEST_CASE_END

}
TEST_SET_END
