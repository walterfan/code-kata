/*
 * TinyConfig.h
 *
 *  Created on: Jul 4, 2016
 *      Author: walter
 */

#ifndef UTIL_TINYCONFIG_H_
#define UTIL_TINYCONFIG_H_

#include <map>
#include <string>



namespace wfan {

struct ConfigItemKey {
    std::string groupName;
    std::string keyName;

    ConfigItemKey(std::string aGroupName, std::string aKeyName): groupName(aGroupName), keyName(aKeyName) {

    }
};

struct KeyLessComparator {
    bool operator()(ConfigItemKey item1, ConfigItemKey item2) const
    {
        int ret = item1.groupName.compare(item2.groupName) ;
        if(ret == 0) {
           return item1.keyName.compare(item2.keyName) < 0;
        }
        return ret < 0;
    }
};

typedef std::map<ConfigItemKey, std::string, KeyLessComparator> ConfigItemMap;

class TinyConfig {
public:
    TinyConfig();
    virtual ~TinyConfig();

    int ReadConfigFile(const char* szFilename);

    int size() { return m_configItemMap.size(); };

    void AddConfigItem(ConfigItemKey key, std::string value);

    void AddConfigItem(std::string groupName, std::string keyName, std::string value) ;

    std::string GetConfigItem(const ConfigItemKey& key, const std::string aDefaultValue = "") const;

    std::string GetConfigItem(const std::string& groupName, const std::string& keyName, const std::string aDefaultValue = "") const;

    bool HasConfigItem(const std::string& groupName, const std::string& keyName, char* szDest = NULL, int nDest = 0) const;

    friend std::ostream& operator<<(std::ostream &os, const TinyConfig &cfg);

private:
    ConfigItemMap m_configItemMap;
};


std::ostream& operator<<(std::ostream &os, const TinyConfig &cfg);

} /* namespace wfan */

#endif /* UTIL_TINYCONFIG_H_ */
