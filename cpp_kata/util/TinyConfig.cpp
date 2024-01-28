/*
 * TinyConfig.cpp
 *
 *  Created on: Jul 4, 2016
 *      Author: walter
 */

#include "TinyUtil.h"
#include "TinyConfig.h"

#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <utility>

using namespace std;

namespace wfan {

ostream& operator<<(ostream &os, const TinyConfig &cfg)
{
    ConfigItemMap::const_iterator it = cfg.m_configItemMap.begin();
    for (; it!=cfg.m_configItemMap.end(); ++it)
        os << it->first.groupName << "/" << it->first.keyName << " => " << it->second << '\n';
    return os;
}


TinyConfig::TinyConfig() {


}

TinyConfig::~TinyConfig() {

}

int TinyConfig::ReadConfigFile(const char* szFilename) {

    string line;
    ifstream myfile(szFilename);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            LOG_TRACE("%s", line.c_str());

        }
        myfile.close();
        LOG_TRACE("Success to open file");
        return RET_ERR;
    }
    else {
        LOG_TRACE("Unable to open file");
        return RET_OK;
    }
}

void TinyConfig::AddConfigItem(std::string groupName, std::string keyName, std::string value) {
    ConfigItemKey key = ConfigItemKey(groupName, keyName);
    AddConfigItem(key, value);
}

void TinyConfig::AddConfigItem(ConfigItemKey key, std::string value) {
    //this->m_configItemMap.insert(std::pair<ConfigItemKey, string>(key, value));
    this->m_configItemMap[key] = value;
}

std::string TinyConfig::GetConfigItem(const ConfigItemKey& key, const std::string aDefault) const {
    ConfigItemMap::const_iterator it = this->m_configItemMap.find(key);

    if(it != m_configItemMap.end())
        return it->second;
    else
        return aDefault;
}


std::string TinyConfig::GetConfigItem(const std::string& groupName, const std::string& keyName, const std::string aDefault) const {
    ConfigItemKey key = ConfigItemKey(groupName, keyName);
	return GetConfigItem(key, aDefault);
}

bool TinyConfig::HasConfigItem(const std::string& groupName, const std::string& keyName, char* szDest, int nDest) const {
    ConfigItemKey key = ConfigItemKey(groupName, keyName);
    ConfigItemMap::const_iterator it = this->m_configItemMap.find(key);
    if(it == m_configItemMap.end())
        return false;
    //LOG_TRACE("%s, %d <-- value=%s", szDest, nDest, it->second.c_str());
    if(szDest && nDest > 0) {
        int nSize = it->second.size();
        if(nDest > nSize) {
            nDest = nSize;
        }
        strncpy(szDest, it->second.c_str(), nDest);
    }
    return true;
}

} /* namespace wfan */
