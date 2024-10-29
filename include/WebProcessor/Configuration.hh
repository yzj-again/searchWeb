/*************************************************************************
    > File Name: Configuration.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月30日 星期六 09时55分48秒
 ************************************************************************/
#ifndef __CONFIGURATION_HH
#define __CONFIGURATION_HH

#include <string>
#include <map>
#include <set>

using std::string;
using std::map;
using std::set;

class Configuration
{
    friend string getPath(const string file);
public:
    static Configuration * getInstance();
    void showConfig() const;
    map<string, string> & getConfigMap() { return _configMap; }
    set<string> getStopWordList() const { return _stopWordList; }
    set<string> getCnStopWordList() const { return _cnStopWordList; }
private:
    class AutoRelease
    {
    public:
        AutoRelease() { };
        ~AutoRelease()
        {
            if(nullptr != _pInstance)
            { delete _pInstance; }
        }
    };

    Configuration(const string & filepath);
    void initStopWords();
    void insertCnStopWords(const string & filepath);
    void insertEnStopWords(const string & filepath);
    Configuration(const Configuration &) = delete;
    Configuration operator=(const Configuration &) = delete;
private:
    string _filepath;
    map<string, string> _configMap;
    set<string> _stopWordList;
    set<string> _cnStopWordList;
    static Configuration * _pInstance;
    static AutoRelease _ar;
};

string getPath(const string file);

#endif
