#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>

#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>
#include <netdb.h>

#include "TinyUtil.h"
#include "TinyChart.h"
#include "BaseTestCase.h"

using namespace std;
using namespace testing;
using namespace wfan;

//using ::testing::Return;
//using ::testing::Mock;

const char*  LATENCY_PNG_FILE = "latency_trendline.png";
const char*  DISTRIBUTION_PNG_FILE = "users_distribution.png";

class CTinyChartTest : public CBaseTestCase
{
public:
public:
    CTinyChartTest()
    : CBaseTestCase()
    {

    }    

    virtual void SetUp() {
        CBaseTestCase::SetUp();
        ASSERT_TRUE(m_pTestCaseBuilder);
        m_pTestCaseBuilder->feature("CTinyChartTest");
    }


};

TEST_F(CTinyChartTest, TrendlineDiagram)
{

    m_pTestCaseBuilder->given("X Scales for 2 weeks, Y Scales with different values")
    ->when("set title, labels and values ")
    ->then("draw and write image")
    ->scenario("Draw a trendline diagram")
    ->checkpoints(" write a png file ");

    STR_VEC xScales;
    xScales.reserve(10);
    xScales.push_back("11-09");
    xScales.push_back("11-10");
    xScales.push_back("11-11");
    xScales.push_back("11-12");
    xScales.push_back("11-13");
    xScales.push_back("11-14");
    xScales.push_back("11-15");
    xScales.push_back("11-16");
    xScales.push_back("11-17");
    xScales.push_back("11-18");
    xScales.push_back("11-19");
    xScales.push_back("11-20");
    xScales.push_back("11-21");
    xScales.push_back("11-22");

    INT_VEC yScales;
    yScales.reserve(10);
    yScales.push_back(200);
    yScales.push_back(130);
    yScales.push_back(3456);
    yScales.push_back(2345);
    yScales.push_back(1320);
    yScales.push_back(30);
    yScales.push_back(2200);
    yScales.push_back(1330);
    yScales.push_back(3330);
    yScales.push_back(332);
    yScales.push_back(788);
    yScales.push_back(200);
    yScales.push_back(13890);
    yScales.push_back(200);

    TinyTrendlineDiagram* pt = new TinyTrendlineDiagram(
            LATENCY_PNG_FILE, 800, 250);
    pt->SetTitle("Trend of Latency (the middle network latency is the value of 0 coordinate)");
    pt->SetLabels(xScales);
    pt->SetValues(yScales);

    pt->Draw();
    pt->WriteImage();

    ifstream fs(LATENCY_PNG_FILE);
    ASSERT_TRUE(fs.good());
    
    SAFE_DELETE(pt);
}

TEST_F(CTinyChartTest, DistributionDiagram)
{
     m_pTestCaseBuilder->given("X scales are 8 countries, Y Scales with different values ")
    ->when("set title, labels and values ")
    ->then("draw and write image")
    ->scenario("Draw a trendline diagram")
    ->checkpoints(" write a png file ");

    STR_VEC xScales;
    xScales.push_back("America");
    xScales.push_back("China");
    xScales.push_back("Japan");
    xScales.push_back("England");
    xScales.push_back("France");
    xScales.push_back("Germany");
    xScales.push_back("South Korean");
    xScales.push_back("India");

    INT_VEC yScales;
    yScales.push_back(4800);
    yScales.push_back(3008);
    yScales.push_back(100);
    yScales.push_back(20);
    yScales.push_back(2000);
    yScales.push_back(178);
    yScales.push_back(258);
    yScales.push_back(1789);

    TinyDistributionDiagram* pd = new TinyDistributionDiagram(
            DISTRIBUTION_PNG_FILE, 800, 600);
    pd->SetTitle("Daily active users distribution among the countries");
    pd->SetLabels(xScales);
    pd->SetValues(yScales);
    pd->Draw();
    pd->WriteImage();

    ifstream fs(LATENCY_PNG_FILE);
    ASSERT_TRUE(fs.good());

    SAFE_DELETE(pd);
}
 
