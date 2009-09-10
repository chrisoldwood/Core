////////////////////////////////////////////////////////////////////////////////
//! \file   TestScoped.cpp
//! \brief  The unit tests for the Scoped class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/Scoped.hpp>

TEST_SET(Scoped)
{
	typedef Core::Scoped<void*> ScopedPtr;

TEST_CASE(Scoped, compilationFails)
{
	ScopedPtr test(free);
}
TEST_CASE_END

TEST_CASE(Scoped, accessors)
{
	ScopedPtr test1(free);
	ScopedPtr test2(malloc(1), free);

	TEST_TRUE(test1.get() == 0);
	TEST_TRUE(test2.get() != 0);
}
TEST_CASE_END

TEST_CASE(Scoped, mutators)
{
	ScopedPtr test(malloc(1), free);

	ASSERT(test.get() != nullptr);

	test.reset();

	TEST_TRUE(test.get() == nullptr);

	free(test.detach());

	TEST_TRUE(test.get() == nullptr);

	test.attach(malloc(1));

	TEST_TRUE(test.get() != nullptr);
}
TEST_CASE_END

TEST_CASE(Scoped, freeFunctions)
{
	void* rsc = malloc(1);

	ScopedPtr test(free);

	*attachTo(test) = rsc;

	TEST_TRUE(test.get() == rsc);
}
TEST_CASE_END

}
TEST_SET_END
