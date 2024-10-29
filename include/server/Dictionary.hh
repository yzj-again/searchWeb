#ifndef __DICTIONARY_HH__
#define __DICTIONARY_HH__

#include "Configuration.hh"
#include <vector>
#include <string>
#include <map>
#include <set>

using std::vector;
using std::string;
using std::pair;
using std::set;
using std::map;

//单例类
class Dictionary{
public:
    class AutoRelease
    {
    public:
        AutoRelease()  {};
        ~AutoRelease(){
            if(_pInstance){
                delete _pInstance;
                _pInstance = nullptr;
            }
        }
    };
    static Dictionary *createInstance();
    void init();      //通过配置文件路径初始化词典和索引
    vector<pair<string, int>> &getDict();   //获取词典
    map<string, set<int>> &getIndexTable(); //获取索引库

    Dictionary(const Dictionary &rhs) = delete;
    Dictionary &operator=(const Dictionary &rhs) = delete;

private:
    Dictionary()
    {
        //加载词典库和索引库（先英文，再中文）
        init();
    }
    ~Dictionary() {};

    size_t getByteNum_UTF8(const char byte);
    size_t getCharNum_UTF8(const string &str);

private:
    static Dictionary *_pInstance;
    static AutoRelease _ar;
    
    vector<pair<string, int>> _dict;    //词典
    map<string,set<int>> _indexTable;  //索引表

    //词典库和索引库文件路径
    const string dict_cn = getPath("dict-cn");
    const string dict_en = getPath("dict-en");
    const string index_cn = getPath("index-cn");
    const string index_en = getPath("index-en");
};



#endif