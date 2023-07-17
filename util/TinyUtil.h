#ifndef __TINY_UTIL_H__
#define __TINY_UTIL_H__

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <time.h>

#define RET_ERR 1
#define RET_OK 0
#define TIME_FMT_LEN 50

#define ERR_RETURN(ret,fmt, ...) \
    do { \
        if (!(ret)) {\
            fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); \
            return ret;\
        }\
    } while (0)

#define MAX_PRINT_STRING_LEN 1024



#ifndef FALSE
    #define FALSE 0
#endif // FALSE

#ifndef TRUE
    #define TRUE 1
#endif // TRUE

#define DEFAULT_BACK_LOG 1024


#ifndef CERR
#define CERR std::cerr <<"\n["<<time(NULL)<<"]\t"<< __FILE__ << "," << std::dec << __LINE__ << " : "
#endif

#ifndef COUT
#define COUT std::cout <<"\n["<<time(NULL)<<"]\t"<< __FILE__ << "," << std::dec << __LINE__ << " : "
#endif

#ifndef INFO_TRACE
#define INFO_TRACE(msg) std::cout <<"\n["<<time(NULL)<<"]\t"<< __FILE__ << "," << std::dec << __LINE__ << " : "<< msg <<endl;
#endif

#ifndef ASSERT
#define ASSERT(condition) { \
    if(!(condition)) \
    { \
        std::cerr << "ASSERT FAILED: " << #condition << " @ "; \
        std::cerr << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
    }}
#endif

#ifndef ASSERT_RET
#define ASSERT_RET(condition, retval) { \
    if(!(condition)) \
    { \
        std::cerr << "ASSERT FAILED: " << #condition << " @ "; \
        std::cerr << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
        return retval;\
    }}
#endif

#ifndef ASSERT_RET_VOID
#define ASSERT_RET_VOID(condition) { \
    if(!(condition)) \
    { \
        std::cerr << "ASSERT FAILED: " << #condition << " @ "; \
        std::cerr << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
        return;\
    }}
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) \
    do \
    { \
        if(NULL != p) \
        { \
            delete p; \
            p = NULL; \
        } \
    } while(0)
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) \
    do \
    { \
        if(NULL != p) \
        {\
            delete [] p; \
            p = NULL; \
        } \
    } while(0)
#endif

#ifndef SAFE_ADDREF
#define SAFE_ADDREF(p) if(p)\
    {\
        p->AddReference();\
    }

#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) \
    do \
    { \
        if(NULL != p) \
        { \
            p->ReleaseReference(); \
            p = NULL; \
        } \
    } while(0)
#endif

#define LOG_TRACE(fmt,...) printf("[%s@%s:%d] " fmt "\n",  __func__,__FILE__, __LINE__, ##__VA_ARGS__)
//#define LOG_TRACE(...) wfan::WriteLog(__FILE__, __LINE__, __VA_ARGS__ )

char * octet_string_hex_string(const void *s, int length);

int hex_string_to_octet_string(char *raw, char *hex, int len);

namespace wfan {
    /**
     * @brief trim string
     * 
     * @param str string to trim
     * @param whitespace space, return, etc.
     * @return std::string trimmed string
     */
    std::string trim(const std::string& str, const std::string& whitespace = " \t");

    std::string ltrim(const std::string& str, const std::string& whitespace = " \t");

    std::string rtrim(const std::string& str, const std::string& whitespace = " \t");

    bool ReplaceValueAsKey(std::string& str, const char*  szKey, const char* szMask, const char* szSep);

    bool startwith(const std::string &str, const std::string &prefix);

    bool endswith(const std::string &str, const std::string &suffix);

	int file2msg(const char* filename,std::string& msg);

	int file2msg(const char* filename,char *msg);

	int RetrieveFiles(const char* szFolder, std::vector<std::string>& files, const char* szExt = NULL);

	std::string UpperCase( const std::string& p_string );

	std::string LowerCase( const std::string& p_string );

    void WriteLog(char* filename, int line, char *fmt,...);
	
	long long current_timestamp(char arrTimeStr[TIME_FMT_LEN]);
	
	int load_file_malloc(const char* szFile, char*& pBuffer, long* pBufSize = NULL) ;

    int base64_encode( char * lpszOutBuffer, int nOutLen, const void * lpszIn, int nInputLen );

    int base64_decode( char * lpszOutBuffer, int nOutLen, const char * pszSrc, int nInLen );


}

using namespace std;
using namespace wfan;

#endif
