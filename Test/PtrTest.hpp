////////////////////////////////////////////////////////////////////////////////
//! \file   PtrTest.hpp
//! \brief  The classes used in unit testing of the smart pointer classes..
//! \author Chris Oldwood

////////////////////////////////////////////////////////////////////////////////
//! The test interface.

class IPtrTest
{
public:
	virtual ulong addRef() = 0;
	virtual ulong release() = 0;
	virtual void  run() = 0;
};

////////////////////////////////////////////////////////////////////////////////
//! The test concrete class.

class PtrTest : public IPtrTest
{
public:
	PtrTest()
		: m_nRefCount(1)
	{ }

	virtual ulong addRef()
	{
		return ++m_nRefCount;
	}

	virtual ulong release()
	{
		if (--m_nRefCount == 0)
			delete this;
		return m_nRefCount;
	}

	virtual void run()
	{ }

	ulong m_nRefCount;
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
