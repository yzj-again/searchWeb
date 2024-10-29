/*************************************************************************
    > File Name: Configuration.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月30日 星期六 10时02分41秒
 ************************************************************************/

#include "../../include/server/Configuration.hh"

#include <stdlib.h>

#include <fstream>
#include <sstream>
#include <iostream>

using std::ifstream;
using std::istringstream;

//饿汉模式，线程安全
Configuration * Configuration::_pInstance = Configuration::getInstance();
Configuration::AutoRelease Configuration::_ar;

Configuration * Configuration::getInstance()
{
    if(nullptr == _pInstance)
    {
        //拼装绝对路径，对项目文件夹位置有要求
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
    //此处直接写死了路径，也可以通过传入的配置文件路径获取项目文件夹位置
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
        //符号#是规定的注释符号，'\0'对应空行
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
    //若对应停用词文件存在则加载文件
    if(_configMap.find("stopwords-cn") != _configMap.end())
    { insertStopWords(_configMap["stopwords-cn"]); } 

    if(_configMap.find("stopwords-en") != _configMap.end())
    { insertStopWords(_configMap["stopwords-en"]); } 
}

void Configuration::insertStopWords(const string & filepath)
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

//需要外部检查路径是否真实存在
string getPath(const string file)
{
    Configuration * pConfig = Configuration::getInstance();
    return pConfig->_configMap[file];
}