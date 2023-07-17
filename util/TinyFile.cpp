/*
 * TinyFile.h
 *
 *  Created on: 2014-7-11
 *      Author: walter
 */

#include "TinyFile.h"

int TinyFile::RetrieveFiles(const char* szFolder, vector<string>& files)
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
    files.push_back(file);
  }

  while((-1 == closedir(dirp)) && (errno == EINTR));

  return files.size();
}

