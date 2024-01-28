/*
 * BaseTestCase.cpp
 *
 *  Created on: 13 Sep 2017
 *      Author: yafan
 */

#include "BaseTestCase.h"


CBaseTestCase::CBaseTestCase(): m_strFeature(""), m_strTestName(""), m_pTestCaseBuilder(NULL) {

}

CBaseTestCase:: ~CBaseTestCase() {

}


void CBaseTestCase::SetUp() {
    std::string strClassName(::testing::UnitTest::GetInstance()->current_test_info()->test_case_name());
    std::string strFuncName(::testing::UnitTest::GetInstance()->current_test_info()->name());
    m_strTestName = strClassName + "." + strFuncName;

    SAFE_DELETE(m_pTestCaseBuilder)
    m_pTestCaseBuilder = new TestCaseBuilder();
    m_pTestCaseBuilder->feature(m_strFeature);

    TEST_TRACE("#---" << m_strTestName << " begin ");
}

void CBaseTestCase::TearDown()  {
    TEST_TRACE("#---" << m_strTestName << " end ");
    m_pTestCaseBuilder->record();
}




