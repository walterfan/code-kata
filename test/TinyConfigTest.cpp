/*
 * TinyConfig_test.cpp
 *
 *  Created on: Jul 4, 2016
 *      Author: walter
 */

#include "TinyConfig.h"

#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <stdint.h>

using namespace std;
using namespace testing;
using namespace wfan;

//using ::testing::Return;
//using ::testing::Mock;

class TinyConfigTest: public Test {
public:
	TinyConfigTest( ) {
		m_pConfig = new TinyConfig();
   }

   void SetUp( ) {
       // code here will execute just before the test ensues
   }

   void TearDown( ) {
       // code here will be called just after the test completes
       // ok to through exceptions from here if need be
   }

   ~TinyConfigTest( )  {
       delete m_pConfig;
   }

   // put in any custom data members that you need
   TinyConfig* m_pConfig;
};

TEST_F (TinyConfigTest, UnitTest1)
{
	m_pConfig->ReadConfigFile("../etc/hfrtc.ini");
	m_pConfig->AddConfigItem("server", "host", "127.0.0.1");
	m_pConfig->AddConfigItem("misc", "host", "127.0.0.2");

	cout<<"# config items: \n"<<*m_pConfig<<endl;

	string a = m_pConfig->GetConfigItem("server", "host");
	cout<<"a=" << a <<endl;
	EXPECT_EQ(0, a.compare("127.0.0.1"));

	string b = m_pConfig->GetConfigItem("misc", "host");
	cout<<"b=" << b <<endl;
	EXPECT_EQ(0, b.compare("127.0.0.2"));

	m_pConfig->AddConfigItem("misc", "host", "127.0.0.3");
	cout<<"# config items: \n"<<*m_pConfig<<endl;

	string c = m_pConfig->GetConfigItem("misc", "host");
    cout<<"c=" << c <<endl;
    EXPECT_EQ(0, c.compare("127.0.0.3"));

    EXPECT_EQ(2, m_pConfig->size());

    m_pConfig->AddConfigItem("misc", "port", "9090");
    cout<<"# config items: \n"<<*m_pConfig<<endl;
    EXPECT_EQ(3, m_pConfig->size());

    char str_value[256] = "";
    m_pConfig->HasConfigItem("misc", "port", str_value, 256);
    cout<<"# str_value: "<<str_value<<endl;
}
