/*
 * CallFlowTestBase.h
 *
 *  Created on: 13 Sep 2017
 *      Author: yafan
 */

#ifndef _BASE_TEST_CASE_H_
#define _BASE_TEST_CASE_H_

#include <string>
#include <iostream>
#include "time.h"
#include "gtest/gtest.h"
//#include "gmock/gmock.h"

#ifndef TEST_TRACE
#define TEST_TRACE(msg) std::cout <<"["<<time(NULL)<<", TEST --"<< __FILE__ << "," << std::dec << __LINE__ << " ]\t"<< msg << std::endl
#endif

#if !defined( SAFE_DELETE )
#define SAFE_DELETE( p )                        { if(( p ) != NULL) { delete ( p ); (p)=NULL; } }
#endif

struct TestCase {
    std::string name;
    std::string feature;
    std::string scenario;
    std::string given;
    std::string when;
    std::string then;
    std::string checkpoints;
};

class TestCaseBuilder
{
  public:
    TestCaseBuilder():m_pTestCase(NULL)
    {
        m_pTestCase = new TestCase();
    }
    virtual ~TestCaseBuilder()
    {
        SAFE_DELETE(m_pTestCase);
    }

    TestCaseBuilder* name(std::string name)
    {
        m_pTestCase->name = name ;
        return this;
    }

    TestCaseBuilder* feature(std::string feature)
    {
        m_pTestCase->feature = feature ;
        return this;
    }

    TestCaseBuilder* scenario(std::string scenario)
    {
        m_pTestCase->scenario = scenario ;
        return this;
    }

    TestCaseBuilder* given(std::string given)
    {
        m_pTestCase->given = given ;
        return this;
    }

    TestCaseBuilder* when(std::string when)
    {
        m_pTestCase->when = when ;
        return this;
    }
    TestCaseBuilder* then(std::string then)
    {
        m_pTestCase->then = then ;
        return this;
    }

    TestCaseBuilder* checkpoints(std::string checkpoints)
    {
        m_pTestCase->checkpoints = checkpoints ;
        return this;
    }

    TestCase* getTestCase()
    {
        return m_pTestCase;
    }

    virtual void record()
    {
        testing::Test::RecordProperty("feature", m_pTestCase->feature);
        testing::Test::RecordProperty("scenario", m_pTestCase->scenario);
        testing::Test::RecordProperty("given", m_pTestCase->given);
        testing::Test::RecordProperty("when", m_pTestCase->when);
        testing::Test::RecordProperty("then", m_pTestCase->then);
        testing::Test::RecordProperty("checkpoints", m_pTestCase->checkpoints);
    }
protected:
    TestCase * m_pTestCase;
};

class CBaseTestCase : public testing::Test
{
public:
    CBaseTestCase();

    virtual ~CBaseTestCase() ;

    virtual void SetUp();
    virtual void TearDown();

protected:

    std::string m_strFeature;
    std::string m_strTestName;
    TestCaseBuilder* m_pTestCaseBuilder;
};


#endif 
