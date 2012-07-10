////////////////////////////////////////////////////////////////////////////////
//! \file   PtrTest.hpp
//! \brief  The classes used in unit testing of the smart pointer classes..
//! \author Chris Oldwood

////////////////////////////////////////////////////////////////////////////////
//! The test interface.

class IPtrTest
{
public:
	virtual ~IPtrTest() {}

	virtual ulong addRef() = 0;
	virtual ulong release() = 0;
	virtual bool  run() = 0;
};

////////////////////////////////////////////////////////////////////////////////
//! The test concrete class.

class PtrTest : public IPtrTest
{
public:
	PtrTest()
		: m_refCount(1)
	{ }

	virtual ulong addRef()
	{
		return ++m_refCount;
	}

	virtual ulong release()
	{
		if (--m_refCount == 0)
			delete this;
		return m_refCount;
	}

	virtual bool run()
	{
		return true;
	}

	ulong m_refCount;
};

////////////////////////////////////////////////////////////////////////////////
//! The test derived concrete class.

class Derived : public PtrTest
{
};

////////////////////////////////////////////////////////////////////////////////
//! The test unrelated concrete class.

class Unrelated
{
};
