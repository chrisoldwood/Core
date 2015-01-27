////////////////////////////////////////////////////////////////////////////////
//! \file   Functor.hpp
//! \brief  Macros for easier generation of functors.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_FUNCTOR_HPP
#define CORE_FUNCTOR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

//! Define a simple predicate that copies by value
#define CORE_DEFINE_PREDICATE(ClassName, ValueType, ValueName, ArgName)		\
class ClassName																\
{																			\
private:																	\
	ValueType ValueName;													\
public:																		\
	ClassName(ValueType __predicate_value__)								\
		: ValueName(__predicate_value__)									\
	{ }																		\
	bool operator()(ValueType ArgName)

//! End the definition of the predicate
#define CORE_END_PREDICATE													\
};

//! Define a predicate that can pass the values using custom types
#define CORE_DEFINE_PREDICATE_EX(ClassName, ValueType, ValueArgType, ValueName, ArgType, ArgName)	\
class ClassName																\
{																			\
private:																	\
	ValueType ValueName;													\
public:																		\
	ClassName(ValueArgType __predicate_value__)								\
		: ValueName(__predicate_value__)									\
	{ }																		\
	bool operator()(ArgType ArgName)

//! End the definition of the predicate
#define CORE_END_PREDICATE_EX												\
};

#endif // CORE_FUNCTOR_HPP
