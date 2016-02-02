////////////////////////////////////////////////////////////////////////////////
//! \file   Functor.cpp
//! \brief  The unit tests for the Functor macros.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <algorithm>
#include <Core/Functor.hpp>

CORE_DEFINE_PREDICATE(IntPredicate, int, value, rhs)
{
	return rhs == value;
}
CORE_END_PREDICATE

CORE_DEFINE_PREDICATE(StringPredicate, tstring, value, rhs)
{
	return rhs == value;
}
CORE_END_PREDICATE

CORE_DEFINE_PREDICATE_EX(StringRefPredicate, tstring, const tstring&, value, const tstring&, rhs)
{
	return rhs == value;
}
CORE_END_PREDICATE_EX

CORE_DEFINE_PREDICATE_EX(StringMixedPredicate, tstring, tstring, value, const tchar*, rhs)
{
	return rhs == value;
}
CORE_END_PREDICATE_EX

TEST_SET(Functor)
{

TEST_CASE("a simple int predicate matches by value")
{
	typedef const int* ConstIter;

	const int expected = 2;
	const int values[] = { 1, expected, 3 };
	ConstIter end = values+ARRAY_SIZE(values);

	ConstIter it = std::find_if(values, end, IntPredicate(expected));

	TEST_TRUE(it != end);
	TEST_TRUE(*it == expected);
}
TEST_CASE_END

TEST_CASE("a simple string predicate matches by value")
{
	typedef const tchar** ConstIter;

	const tstring expected = TXT("2");
	const tchar*  values[] = { TXT("1"), expected.c_str(), TXT("3") };
	ConstIter     end = values+ARRAY_SIZE(values);

	ConstIter it = std::find_if(values, end, StringPredicate(expected));

	TEST_TRUE(it != end);
	TEST_TRUE(*it == expected);
}
TEST_CASE_END

TEST_CASE("a custom string predicate can pass the values by reference")
{
	typedef const tchar** ConstIter;

	const tstring expected = TXT("2");
	const tchar*  values[] = { TXT("1"), expected.c_str(), TXT("3") };
	ConstIter     end = values+ARRAY_SIZE(values);

	ConstIter it = std::find_if(values, end, StringRefPredicate(expected));

	TEST_TRUE(it != end);
	TEST_TRUE(*it == expected);
}
TEST_CASE_END

TEST_CASE("a custom string predicate can pass the values by different argument types")
{
	typedef const tchar** ConstIter;

	const tstring expected = TXT("2");
	const tchar*  values[] = { TXT("1"), expected.c_str(), TXT("3") };
	ConstIter     end = values+ARRAY_SIZE(values);

	ConstIter it = std::find_if(values, end, StringMixedPredicate(expected));

	TEST_TRUE(it != end);
	TEST_TRUE(*it == expected);
}
TEST_CASE_END

}
TEST_SET_END
