//#include "gmock/gmock.h"
#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <stdint.h>

using namespace std;
using namespace testing;

//using ::testing::Return;
//using ::testing::Mock;

class CReferenceControl
{
public:
	CReferenceControl(): m_nRefCnt(0)
	{}

	virtual ~CReferenceControl()
	{
		cout<<"m_nRefCnt="<<m_nRefCnt<<endl;
	}

	uint32_t AddReference()
	{
		return ++m_nRefCnt;
	}

	uint32_t ReleaseReference()
	{
		uint32_t nCnt = --m_nRefCnt;
		if(0==nCnt)
		{
			delete this;
		}
		return nCnt;

	}
	uint32_t GetRefCnt()
	{
		return m_nRefCnt;
	}
private:
	uint32_t m_nRefCnt;
};

class CReceiver: virtual public CReferenceControl
{
public:
	void OnReceive();
};

class CSender: virtual public CReferenceControl
{
public:
	int Send();
};

class CTransport: public CSender, public CReceiver
{
public:
	uint32_t AddReference() {return CReferenceControl::AddReference();}
	uint32_t ReleaseReference() { return CReferenceControl::ReleaseReference();}
	void Dump()
	{
		cout<<this->GetRefCnt()<<endl;
	};
};

class IServer
{
public:
   virtual int Start() = 0;
   virtual int Stop() = 0;
   virtual ~IServer() { cout<<"~IServer"<<endl;};
};

class CServer: public IServer
{
   virtual int Start() { cout<<"CServer::Start"<<endl; return 0;};
   virtual int Stop() { cout<<"CServer::Stop"<<endl;return 0;};
   virtual ~CServer() { cout<<"~CServer"<<endl;};
};

TEST(ServerTest, VirtualDestructor)
{

	IServer* pSvr = new CServer();
	delete pSvr;
}

TEST(ServerTest, VirtualInherit)
{
	cout<<"--- VirtualInherit --"<<endl;
	CTransport* pSvr = new CTransport();
	pSvr->AddReference();
	pSvr->Dump();
	pSvr->ReleaseReference();

}
