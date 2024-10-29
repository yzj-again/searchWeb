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
#include <iostream>

using std::string;
using std::map;
using std::set;
using std::cout;
using std::endl;

//返回引用可以减少拷贝次数，但是不安全（可以被改变）
//声明为友元函数可以不写类作用域
//用于直接获取configMap中的地址项
string getPath(const string file);

class Configuration
{
    friend string getPath(const string file);
public:
    static Configuration * getInstance();
    void showConfig() const;
    map<string, string> & getConfigMap() { return _configMap; }
    set<string> getStopWordList() const { return _stopWordList; }
private:
    //用于自动释放
    class AutoRelease
    {
    public:
        AutoRelease() { };
        ~AutoRelease()
        {
            if(nullptr != _pInstance)
            { 
                delete _pInstance; 
                cout << "Configuration::~AutoRelease()" << endl;
            }
        }
    };

    Configuration(const string & filepath);
    void initStopWords();
    //加载停用词
    void insertStopWords(const string & filepath);
    //单例类应该删除值语义
    Configuration(const Configuration &) = delete;
    Configuration operator=(const Configuration &) = delete;
private:
    string _filepath;
    map<string, string> _configMap;
    set<string> _stopWordList;
    static Configuration * _pInstance;
    static AutoRelease _ar;
};

#endif
