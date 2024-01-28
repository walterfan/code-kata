//#include "openssl/crypto.h"  
//#include "openssl/sha.h"  
#include "openssl/evp.h"  
#include <string.h>
#include <stdio.h>
/*
void sha256(char *string, char outputBuffer[65])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    int i = 0;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}
*/
typedef unsigned char BYTE;
const unsigned int SHA256_MD_LEN = 32;

int message_digest(const BYTE *message, BYTE **digest, unsigned int *digest_len, const EVP_MD *method)
{
	EVP_MD_CTX *mdctx;

	if((mdctx = EVP_MD_CTX_create()) == NULL)
		return -1;

	if(1 != EVP_DigestInit_ex(mdctx, method, NULL)) {
		EVP_MD_CTX_destroy(mdctx);
		return -2;
	}
	if(1 != EVP_DigestUpdate(mdctx, message, strlen((const char*)message))) {
		EVP_MD_CTX_destroy(mdctx);
		return -3;

	}	
	if(NULL == *digest) {
		//if((*digest = (BYTE *)malloc(32)) == NULL)
		if((*digest = (BYTE *)OPENSSL_malloc(EVP_MD_size(method))) == NULL) {
		    EVP_MD_CTX_destroy(mdctx);
			return -4;
		}
	}
	
	if(1 != EVP_DigestFinal_ex(mdctx, *digest, digest_len)) {
		EVP_MD_CTX_destroy(mdctx);
		return -5;
	}
	
	EVP_MD_CTX_destroy(mdctx);
	return 0;
}


int main()  
{  
	//OpenSSL_add_all_digests();
    //const EVP_MD*  pMethod = EVP_get_digestbyname("SHA256");
	const EVP_MD* pMethod = EVP_sha256();
	
    BYTE pSrc[] = "Hello world! ";  
    BYTE pDest[SHA256_MD_LEN + 1] = {'\0'};  
    
	unsigned int uLen = 0; 
    int ret = message_digest(pSrc, (BYTE**)&pDest, &uLen, pMethod);
	if(ret < 0 ) {
		fprintf(stderr, "digest error %d", ret);
		return ret;
	}
     
    printf("Digest(%d) is: \n", uLen);  
    for(int i = 0; i < uLen; i++)
        printf("%02x", pDest+i);
    printf("\n");
	//if(pDest)	OPENSSL_free(pDest);
	//EVP_cleanup();
    return 0;  
}  
