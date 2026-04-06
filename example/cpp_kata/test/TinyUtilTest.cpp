#include <gtest/gtest.h>
#include <string>
#include <iostream>
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

using namespace std;
using namespace testing;
using namespace wfan;

//using ::testing::Return;
//using ::testing::Mock;


TEST(TinyUtilTest, RetrieveFiles)
{
	cout<<"--- RetrieveFiles --"<<endl;

	bool ret = endswith("test.cpp", ".cpp");
	cout<<"IsEndsWith:" << ret <<endl;

	//int RetrieveFiles(const char* szFolder, vector<string>& vecFiles)
	string strDir = ".";
	vector<string> vecFiles;
	int fileCount = wfan::RetrieveFiles(strDir.c_str(), vecFiles, ".cpp");
	
	int i = 0;
	for (vector<string>::iterator it = vecFiles.begin();
            it != vecFiles.end(); ++it)
    {
    	cout<<++i<<". "<<*it<<endl;
    }
    cout<<"files count:"<<fileCount<<endl;
    //ASSERT_TRUE(i > 30);

}

TEST(TinyUtilTest, Base64Test)
{
	string strSrc = "walter& <yafan@example.com>";
	int nBufLen = strSrc.size() * 2;
	
	char* pSrcBuf = new char[nBufLen];
	char* pDestBuf = new char[nBufLen];

	int nRet = base64_encode(pDestBuf, nBufLen, strSrc.c_str(), nBufLen/2);
	cout << nRet << " " << pDestBuf << endl;

	nRet = base64_decode(pSrcBuf, nBufLen, pDestBuf, nBufLen);
	cout << nRet << " " << pSrcBuf << endl;

	ASSERT_TRUE(strSrc.compare(pSrcBuf) == 0);

	delete[] pDestBuf;
	delete[] pSrcBuf;

}

TEST(TinyUtilTest, AddrInfoTest)
{
	char buffer[8192];
	struct addrinfo* result=0;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags |= AI_CANONNAME;
    hints.ai_family |= AF_UNSPEC;
    int res = getaddrinfo(buffer, 0, &hints, &result);
    cout<< "res=" <<res<<endl;
}


TEST(TinyUtilTest, ReplaceValueAsKeyTest)
{
    string str = "username=Walter&password=abc$@!\n&email=<walter.fan@gmail.com>";
    string str1 = str;
    bool ret  = wfan::ReplaceValueAsKey(str, "pwd=", "******", "&");
    ASSERT_TRUE(!ret);

    ret = wfan::ReplaceValueAsKey(str, "password=", "******", "&");
    cout<<ret<< " str=" <<str<<endl;
    ASSERT_TRUE(ret);

    ret  = wfan::ReplaceValueAsKey(str1, "password=", "******", "&");
    cout<<ret<< " str1=" <<str1<<endl;
    ASSERT_TRUE(ret);

}


