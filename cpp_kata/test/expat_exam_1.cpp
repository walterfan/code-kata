#include <stdio.h>
#include <expat.h>

#if defined(__amigaos__) && defined(__USE_INLINE__)
#include <proto/expat.h>
#endif

#ifdef XML_LARGE_SIZE
#if defined(XML_USE_MSC_EXTENSIONS) && _MSC_VER < 1400
#define XML_FMT_INT_MOD "I64"
#else
#define XML_FMT_INT_MOD "ll"
#endif
#else
#define XML_FMT_INT_MOD "l"
#endif

#define BUFFSIZE        8192

char Buff[BUFFSIZE];

int Depth;

static void XMLCALL start(void *data, const char *el, const char **attr)
{
  int i;

  for (i = 0; i < Depth; i++)
    printf("  ");

  printf("%s", el);

  for (i = 0; attr[i]; i += 2) {
    printf(" %s='%s'", attr[i], attr[i + 1]);
  }

  printf("\n");
  Depth++;
}

static void XMLCALL end(void *data, const char *el)
{
  Depth--;
}


int load_file_malloc(const char* szFile, char*& pBuffer, long* pBufSize) 
{
    FILE * pFile = NULL;
    long lSize = 0;
    size_t result = 0;

    pFile = fopen(szFile, "r");
    if (pFile == NULL) {
        fputs("File open error", stderr);
        return 1;
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);

    // allocate memory to contain the whole file:
	if(NULL == pBuffer)
    	pBuffer = (char*) malloc(sizeof(char) * lSize);
	
    if (NULL == pBuffer) {
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
        *pBufSize = lSize;
    //printf("content: %s bytes\n", pBuffer);
    fclose(pFile);
    return 0;
}


int main(int argc, char *argv[])
{
  XML_Parser p = XML_ParserCreate(NULL);
  if (! p) {
    fprintf(stderr, "Couldn't allocate memory for parser\n");
    exit(-1);
  }

  XML_SetElementHandler(p, start, end);
  
  char* szFile = NULL;
  long len = 0;
  
  if(argc > 1)
	  szFile = argv[1];
  
  if(NULL == szFile) {
	  fprintf(stderr, "Please specify a xml file\n");
	  return -1;
  }

  char* pBuffer = NULL;
  int ret = load_file_malloc(szFile, pBuffer, &len);
  int done = 1;
  fprintf(stdout, "load file: %ld bytes\n", len);
  fprintf(stdout, "--- content: %s ---\n", pBuffer);
  
  if (XML_Parse(p, pBuffer, (int)len, done) == XML_STATUS_ERROR) {
    fprintf(stderr, "Parse error at line %" XML_FMT_INT_MOD "u:\n%s\n",
            XML_GetCurrentLineNumber(p),
            XML_ErrorString(XML_GetErrorCode(p)));
    exit(-1);
  }
  
  
 
  XML_ParserFree(p);
  return 0;
}
