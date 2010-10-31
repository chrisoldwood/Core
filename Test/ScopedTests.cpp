////////////////////////////////////////////////////////////////////////////////
//! \file   ScopedTests.cpp
//! \brief  The unit tests for the Scoped class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/Scoped.hpp>

TEST_SET(Scoped)
{
	typedef Core::Scoped<void*> ScopedPtr;

TEST_CASE("pointer must be constructed with at least a destroy function")
{
	ScopedPtr test(free);

	TEST_PASSED("compilation succeeded");

	TEST_TRUE(test.get() == nullptr);
}
TEST_CASE_END

TEST_CASE("pointer can be constructed with a destroy function and resource")
{
	void*     resource = malloc(1);
	ScopedPtr test(resource, free);

	TEST_PASSED("compilation succeeded");

	TEST_TRUE(test.get() == resource);
}
TEST_CASE_END

TEST_CASE("reset empties the pointer when argument is null pointer")
{
	ScopedPtr test(malloc(1), free);

	test.reset();

	TEST_TRUE(test.get() == nullptr);
}
TEST_CASE_END

TEST_CASE("attach changes ownership of the resource")
{
	void* original = malloc(1);

	ScopedPtr test(original, free);

	TEST_TRUE(test.get() == original);

	void* replacement = malloc(2);

	test.attach(replacement);

	TEST_TRUE(test.get() == replacement);
}
TEST_CASE_END

TEST_CASE("detach releases ownership of the pointer")
{
	int array[1] = { 12345678 };

	ScopedPtr test(array, free);

	TEST_TRUE(test.get() == array);

	test.detach();

	TEST_TRUE(test.get() == nullptr);
}
TEST_CASE_END

TEST_CASE("smart pointer unaware functions can attach a pointer to an empty instance")
{
	ScopedPtr test(free);

	void* resource = malloc(1);

	*attachTo(test) = resource;

	TEST_TRUE(test.get() == resource);
}
TEST_CASE_END

}
TEST_SET_END
