#include "TinyUtil.h"
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <execinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>

#define DEBUG_TRACE printf

int octet_weight[256] = {
  0, 1, 1, 2, 1, 2, 2, 3,
  1, 2, 2, 3, 2, 3, 3, 4,
  1, 2, 2, 3, 2, 3, 3, 4,
  2, 3, 3, 4, 3, 4, 4, 5,
  1, 2, 2, 3, 2, 3, 3, 4,
  2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5,
  3, 4, 4, 5, 4, 5, 5, 6,
  1, 2, 2, 3, 2, 3, 3, 4,
  2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5,
  3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5,
  3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6,
  4, 5, 5, 6, 5, 6, 6, 7,
  1, 2, 2, 3, 2, 3, 3, 4,
  2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5,
  3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5,
  3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6,
  4, 5, 5, 6, 5, 6, 6, 7,
  2, 3, 3, 4, 3, 4, 4, 5,
  3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6,
  4, 5, 5, 6, 5, 6, 6, 7,
  3, 4, 4, 5, 4, 5, 5, 6,
  4, 5, 5, 6, 5, 6, 6, 7,
  4, 5, 5, 6, 5, 6, 6, 7,
  5, 6, 6, 7, 6, 7, 7, 8
};

char bit_string[MAX_PRINT_STRING_LEN];
char LOG_FILE[256] = "journey.log";

static FILE *fp ;
static int SESSION_TRACKER; //Keeps track of session
 


int octet_get_weight(uint8_t octet) {
  extern int octet_weight[256];

  return octet_weight[octet];
}  

uint8_t nibble_to_hex_char(uint8_t nibble) {
  uint8_t buf[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
		  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
  return buf[nibble & 0xF];
}

char * octet_string_hex_string(const void *s, int length) {
  const uint8_t *str = (const uint8_t *)s;
  int i;
  
  /* double length, since one octet takes two hex characters */
  length *= 2;

  /* truncate string if it would be too long */
  if (length > MAX_PRINT_STRING_LEN)
    length = MAX_PRINT_STRING_LEN-1;
  
  for (i=0; i < length; i+=2) {
    bit_string[i]   = (char)nibble_to_hex_char(*str >> 4);
    bit_string[i+1] = (char)nibble_to_hex_char(*str++ & 0xF);
  }
  bit_string[i] = 0; /* null terminate string */
  return bit_string;
}

static inline int hex_char_to_nibble(uint8_t c) {
  switch(c) {
  case ('0'): return 0x0;
  case ('1'): return 0x1;
  case ('2'): return 0x2;
  case ('3'): return 0x3;
  case ('4'): return 0x4;
  case ('5'): return 0x5;
  case ('6'): return 0x6;
  case ('7'): return 0x7;
  case ('8'): return 0x8;
  case ('9'): return 0x9;
  case ('a'): return 0xa;
  case ('A'): return 0xa;
  case ('b'): return 0xb;
  case ('B'): return 0xb;
  case ('c'): return 0xc;
  case ('C'): return 0xc;
  case ('d'): return 0xd;
  case ('D'): return 0xd;
  case ('e'): return 0xe;
  case ('E'): return 0xe;
  case ('f'): return 0xf;
  case ('F'): return 0xf;
  default: return -1;   /* this flags an error */
  }
  /* NOTREACHED */
  return -1;  /* this keeps compilers from complaining */
}

int is_hex_string(char *s) {
  while(*s != 0)
    if (hex_char_to_nibble((uint8_t)*s++) == -1)
      return 0;
  return 1;
}

/*
 * hex_string_to_octet_string converts a hexadecimal string
 * of length 2 * len to a raw octet string of length len
 */

int hex_string_to_octet_string(char *raw, char *hex, int len) {
  uint8_t x;
  int tmp;
  int hex_len;

  hex_len = 0;
  while (hex_len < len) {
    tmp = hex_char_to_nibble((uint8_t)hex[0]);
    if (tmp == -1)
      return hex_len;
    x = (uint8_t)(tmp << 4);
    hex_len++;
    tmp = hex_char_to_nibble((uint8_t)hex[1]);
    if (tmp == -1)
      return hex_len;
    x |= (uint8_t)(tmp & 0xff);
    hex_len++;
    *raw++ = (char)x;
    hex += 2;
  }
  return hex_len;
}

int octet_string_is_eq(uint8_t *a, uint8_t *b, int len) {
  uint8_t *end = b + len;
  while (b < end)
    if (*a++ != *b++)
      return 1;
  return 0;
}

void octet_string_set_to_zero(uint8_t *s, int len) {
  uint8_t *end = s + len;

  do {
    *s = 0;
  } while (++s < end);
  
}


namespace wfan {

std::string trim(const std::string& str, const std::string& whitespace )
{
    string::size_type strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    string::size_type strEnd = str.find_last_not_of(whitespace);
    string::size_type strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::string ltrim(const std::string& str, const std::string& whitespace )
{
    string::size_type strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content


    string::size_type strRange = str.size() - strBegin + 1;
    return str.substr(strBegin, strRange);
}

std::string rtrim(const std::string& str, const std::string& whitespace )
{
    string::size_type strEnd = str.find_last_not_of(whitespace);
    if (strEnd == std::string::npos)
        return ""; // no content

    string::size_type strRange = strEnd + 1;

    return str.substr(0, strRange);
}

bool ReplaceValueAsKey(string& str, const char* key, const char* mask, const char* sep)
{

    string::size_type nBegin = str.find(key);
    if(string::npos == nBegin) return false;

    string::size_type nEnd = str.find(sep, nBegin);

    if(string::npos == nEnd) {
        str  = str.substr(0, nBegin) + key + mask;
    } else {
        str = str.substr(0, nBegin) + key + mask + str.substr(nEnd);
    }
    return true;
}

bool endswith(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;

}


bool startwith(const std::string &str, const std::string &prefix)
{
    string::size_type totalSize = str.size();
    string::size_type prefixSize = prefix.size();

    if(totalSize < prefixSize) {
        return false;
    }

    return str.compare(0, prefixSize, prefix) == 0;
}

int file2msg(const char* filename,string& msg)
{
    ifstream ifile(filename);
    if(!ifile)
    {
        cout<<"Unable to read "<<filename<<endl;
        return RET_ERR;
    }
    ostringstream buf;
    char ch;
    while ( buf && ifile.get( ch ))
        buf.put( ch );
    ifile.close();
    msg=buf.str();
    return RET_OK;
}

int file2msg(const char* filename,char *msg)
{
    ifstream ifile(filename);
    if(!ifile)
    {
        cout<<"Unable to read "<<filename<<endl;
        return -1;
    }
    ostringstream buf;
    char ch;
    while ( buf && ifile.get( ch ))
    buf.put( ch );
    strcpy(msg, buf.str().c_str());
    return 0;
}

int RetrieveFiles(const char* szFolder, vector<string>& vecFiles, const char* szExt)
{
  struct dirent* direntp;
  DIR* dirp = opendir(szFolder);

  if(NULL == dirp) {
    return -1;
  }

  while( NULL != (direntp = readdir(dirp))) {
    string file = direntp->d_name;
    if(".." == file || "." == file)
        continue;
    
    string strPath = szFolder;
    strPath.append("/");
    strPath.append(file);

    struct stat stFileInfo;
    int nStatus = stat(strPath.c_str(), &stFileInfo);
    if (nStatus == 0 && S_ISDIR(stFileInfo.st_mode )) {
        RetrieveFiles(strPath.c_str(), vecFiles, szExt);
    }



    if(NULL == szExt) {
      vecFiles.push_back(strPath);
    }
    else {
      bool hasSuffix = endswith(strPath, szExt);
      if(hasSuffix) {
        vecFiles.push_back(strPath);
      }
    }
  }

  while((-1 == closedir(dirp)) && (errno == EINTR));

  return (int)vecFiles.size();
}


string UpperCase( const string& p_string )
{
    string str = p_string;

    for( size_t i = 0; i < str.size(); i++ )
    {
        str[i] = (char)toupper( str[i] );
    }
    return str;
}

string LowerCase( const string& p_string )
{
    string str = p_string;

    for( size_t i = 0; i < str.size(); i++ )
    {
        str[i] = (char)tolower( str[i] );
    }
    return str;
}

void WriteLog(char* filename, int line, char *fmt,...)
{
    va_list         args;
    int n = 0;
    char log_msg[1024] = {'\0'}; 

    if(SESSION_TRACKER > 0)
      fp = fopen (LOG_FILE,"a+");
    else
      fp = fopen (LOG_FILE,"w");

    struct timeval tim;  
    gettimeofday(&tim, NULL);  
     
    va_start( args, fmt );
    n = vsnprintf(log_msg, 1024, fmt, args);
    va_end( args );
    
    //fprintf(fp,"%ld ",time(NULL));
    fprintf(fp,"[%ld.%3d, %s, %d] %s", tim.tv_sec, tim.tv_usec/1000, filename,line, log_msg);
    #ifdef __WF_DEBUG__
    fprintf(stdout,"[%ld.%3d, %s, %d] %s", tim.tv_sec, tim.tv_usec/1000, filename,line, log_msg);
    #endif
    
    
    SESSION_TRACKER++;
    fclose(fp);
}

void DumpCallStack(std::ostream &os)
{
	void* trace[ 64 ];
	char** messages = NULL;
	int trace_size = 0;

	os << "Begin Stack Frame Dump" << std::endl;
	os << "(if useful symbols are not found, try recompiling "
		<< "with -rdynamic during link, and -g without -O#)"
		<< std::endl;
	trace_size = backtrace( trace, 64 );
	messages = backtrace_symbols( trace, trace_size );
	for( int i = 0; i < trace_size; ++i )
	{
		os << "Traced: " << messages[i] << std::endl;
	}

	os << "End Stack Frame Dump" << std::endl;

    free (messages);
}

long long current_timestamp(char arrTimeStr[TIME_FMT_LEN]) {
    struct timeval tv;
    struct tm* ptm;
    char time_string[40];

    gettimeofday(&tv, NULL); // get current time
    if (arrTimeStr) {
        ptm = localtime(&tv.tv_sec);
        /* Format the date and time, down to a single second.  */
        strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", ptm);
        /* Compute milliseconds from microseconds.  */
        //snprintf(char * restrict str, size_t size, const char * restrict format,
        snprintf(arrTimeStr, TIME_FMT_LEN, "%s.%06d", time_string, tv.tv_usec);
    }
    long long total_us = tv.tv_sec * 1000000LL + tv.tv_usec ; // caculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return total_us;
}

int load_file_malloc(const char* szFile, char*& pBuffer, long* pBufSize) 
{
    FILE * pFile = NULL;
    size_t lSize = 0;
    size_t result = 0;

    pFile = fopen(szFile, "r");
    if (pFile == NULL) {
        fputs("File open error", stderr);
        return 1;
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = (size_t)ftell(pFile);
    rewind(pFile);

    // allocate memory to contain the whole file:
    pBuffer = (char*) malloc(sizeof(char) * lSize);
    if (pBuffer == NULL) {
        fputs("Memory allocate error", stderr);
        fclose(pFile);
        return 2;
    }

    // copy the file into the buffer:
    result = fread(pBuffer, 1, lSize, pFile);
    if (result != lSize) {
        fputs("Reading file error", stderr);
        fclose(pFile);
        return 3;
    }
    if (pBufSize)
        *pBufSize = (long)lSize;

    fclose(pFile);
    return 0;
}


int base64_encode( char * lpszOutBuffer, int nOutLen, const void * lpszIn, int nInputLen )
 {
    static unsigned char base64str[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    int nNeededLen = (nInputLen + 2) / 3 * 4;
    unsigned char * pInCur = (unsigned char *) lpszIn;
    char * pszOut = lpszOutBuffer;

    if (nInputLen <= 0) {
        lpszOutBuffer[0] = '\0';
        return 0;
    }

    if (nOutLen < nNeededLen)
        return -1;

    while (nInputLen >= 3) {
        pszOut[0] = (char)base64str[pInCur[0] >> 2];
        pszOut[1] = (char)base64str[((pInCur[0] & 0x3) << 4) | (pInCur[1] >> 4)];
        pszOut[2] = (char)base64str[((pInCur[1] & 0xf) << 2) | (pInCur[2] >> 6)];
        pszOut[3] = (char)base64str[pInCur[2] & 0x3f];
        pInCur += 3;
        pszOut += 4;
        nInputLen -= 3;
    }

    switch (nInputLen) {
    case 1:
        pszOut[0] = (char)base64str[pInCur[0] >> 2];
        pszOut[1] = (char)base64str[(pInCur[0] & 0x3) << 4];
        pszOut[2] = '=';
        pszOut[3] = '=';
        pszOut += 4;
        goto ret;
    case 2:
        pszOut[0] = (char)base64str[pInCur[0] >> 2];
        pszOut[1] = (char)base64str[((pInCur[0] & 0x3) << 4) | (pInCur[1] >> 4)];
        pszOut[2] = (char)base64str[(pInCur[1] & 0xf) << 2];
        pszOut[3] = '=';
        pszOut += 4;
        goto ret;
    }
    ret: pszOut[0] = '\0';
    return (int) (pszOut - lpszOutBuffer);
}

int base64_decode( char * lpszOutBuffer, int nOutLen, const char * pszSrc, int nInLen )
{
  static unsigned short BASE64_DECODE_TABLE[128] = {
      255, 255, 255, 255, 255, 255, 255, 255, //  00 -  07
      255, 255, 255, 255, 255, 255, 255, 255, //  08 -  15
      255, 255, 255, 255, 255, 255, 255, 255, //  16 -  23
      255, 255, 255, 255, 255, 255, 255, 255, //  24 -  31
      255, 255, 255, 255, 255, 255, 255, 255, //  32 -  39
      255, 255, 255,  62, 255, 255, 255,  63, //  40 -  47
      52,  53,  54,  55,  56,  57,  58,  59, //  48 -  55
      60,  61, 255, 255, 255, 255, 255, 255, //  56 -  63
      255,   0,   1,   2,   3,   4,   5,   6, //  64 -  71
      7,   8,   9,  10,  11,  12,  13,  14, //  72 -  79
      15,  16,  17,  18,  19,  20,  21,  22, //  80 -  87
      23,  24,  25, 255, 255, 255, 255, 255, //  88 -  95
      255,  26,  27,  28,  29,  30,  31,  32, //  96 - 103
      33,  34,  35,  36,  37,  38,  39,  40, // 104 - 111
      41,  42,  43,  44,  45,  46,  47,  48, // 112 - 119
      49,  50,  51, 255, 255, 255, 255, 255, // 120 - 127
  };
  const unsigned char * pszCur = (const unsigned char * )pszSrc;
  char * pszOut = (char *)lpszOutBuffer;
  unsigned long lByteBuffer = 0, lByteBufferSpace = 4;
  unsigned short tc;
  char *pSave = (char *)&lByteBuffer;
  int nRet = 0;

  if( nInLen > 0 )
  {
    int nTotal = nInLen % 4;
    switch( nTotal )
    {
      case 2:
        nTotal = nInLen * 3 / 4 + 1;
        break;
      case 3:
        nTotal = nInLen * 3 / 4 + 2;
        break;
      case 0:
        nTotal = nInLen * 3 / 4;
        break;
    }
    if( nTotal > nOutLen )
      return -1;
  }
  
  if( pszSrc[0] == '\0' )
    return 0;

  while( *pszCur != '\0' )
  {
    if( *pszCur > 0x7f )
    {
      pszCur ++;
      continue;
    }
    tc = BASE64_DECODE_TABLE[*pszCur];
    pszCur ++;
    if( 255 == tc )
      continue;
    lByteBuffer = (lByteBuffer << 6) | tc ;
    lByteBufferSpace--;
    if (lByteBufferSpace != 0) 
      continue;
         pszOut[0] = pSave[2];
    pszOut[1] = pSave[1];
    pszOut[2] = pSave[0];
        pszOut += 3;
    lByteBuffer = 0; lByteBufferSpace = 4;
    nRet ++;
  }
  nRet *= 3;
  switch(lByteBufferSpace)
  {
  case 1:
    nRet += 2;
    lByteBuffer >>= 2;
    pszOut[0] = pSave[1];
    pszOut[1] = pSave[0];
        pszOut += 2;
    break;
  case 2:
    nRet ++;
    lByteBuffer >>= 4;
    pszOut[0] = pSave[0];
        pszOut ++;
    break;
  }
  if( pszOut[0] != '\0' )
    pszOut[0] = '\0';

  return nRet;
}

}//--end of namespace wfan
