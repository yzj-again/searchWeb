/*************************************************************************
    > File Name: Configuration.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月30日 星期六 10时02分41秒
 ************************************************************************/

#include "../../include/DictBuilder/Configuration.hh"

#include <stdlib.h>

#include <fstream>
#include <sstream>
#include <iostream>

using std::ifstream;
using std::istringstream;

Configuration * Configuration::_pInstance = Configuration::getInstance();
Configuration::AutoRelease Configuration::_ar;

Configuration * Configuration::getInstance()
{
    if(nullptr == _pInstance)
    {
        char * home = getenv("HOME");
        if(nullptr == home)
        {
            perror("getenv");
            exit(0);
        }
        string path(home);
        path += "/SearchEngine/conf/myconf.conf";
        _pInstance = new (std::nothrow) Configuration(path);
    }
    return _pInstance;
}

void Configuration::showConfig() const
{
    for(auto & it: _configMap)
    { std::cout << it.first << " " << it.second << std::endl; }
}

Configuration::Configuration(const string & filepath)
: _filepath(filepath)
{
    char * home = getenv("HOME");
    if(nullptr == home)
    {
        perror("getenv");
        exit(0);
    }

    string homestr(home);
    homestr += "/SearchEngine/";
    ifstream file;
    file.open(_filepath);
    if(!file.good())
    { 
        perror("打开配置文件失败");
        exit(0);
    }
    
    string line;
    while(getline(file, line))
    {
        if('#' == line[0] || '\0' == line[0]) { continue; }
        istringstream iss(line);
        string wordName, wordValue;
        iss >> wordName >> wordValue;
        if(0 == wordValue.size() || !iss.eof())
        {
            perror("配置文件错误");
            exit(0);
        }
        string path = homestr + wordValue;
        _configMap[wordName] = path;
    }

    file.close();
    initStopWords();
}

void Configuration::initStopWords()
{
    if(_configMap.find("stopwords-cn") != _configMap.end())
    { insertCnStopWords(_configMap["stopwords-cn"]); } 

    if(_configMap.find("stopwords-en") != _configMap.end())
    { insertEnStopWords(_configMap["stopwords-en"]); } 
}

void Configuration::insertCnStopWords(const string & filepath)
{
    ifstream file;
    file.open(filepath);
    if(!file.good())
    { 
        perror("打开停用词文件失败");
        exit(0);
    }

    string line;
    while(getline(file, line))
    {
        if('\0' == line[0]) { continue; }
        istringstream iss(line);
        string word;
        iss >> word;
        if(0 == word.size())
        {
            perror("停用词文件错误");
            //exit(0);
            continue;
        }
        _stopWordList.insert(word);
        _cnStopWordList.insert(word);
    }

    file.close();
}

void Configuration::insertEnStopWords(const string & filepath)
{
    ifstream file;
    file.open(filepath);
    if(!file.good())
    { 
        perror("打开停用词文件失败");
        exit(0);
    }

    string line;
    while(getline(file, line))
    {
        if('\0' == line[0]) { continue; }
        istringstream iss(line);
        string word;
        iss >> word;
        if(0 == word.size())
        {
            perror("停用词文件错误");
            //exit(0);
            continue;
        }
        _stopWordList.insert(word);
    }

    file.close();
}

string getPath(const string file)
{
    Configuration * pConfig = Configuration::getInstance();
    return pConfig->_configMap[file];
}