/*
 * TinyFile.h
 *
 *  Created on: 2014-7-11
 *      Author: walter
 */

#ifndef TINYFILE_H_
#define TINYFILE_H_

#include <vector>
#include <string>
using namespace std;

class TinyFile {
public:
	TinyFile() {};
	virtual ~TinyFile() {};
	int ReadFile(const char* szFile);
	int CopyFile(const char* szSrcFile, const char* szDestFile);
	int RetrieveFiles(const char* szFolder, vector<string>& files);
};

#endif /* TINYFILE_H_ */
