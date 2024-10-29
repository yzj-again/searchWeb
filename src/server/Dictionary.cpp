#include "../../include/server/Dictionary.hh"
#include <fstream>
#include <iostream>
#include <sstream>

using std::ifstream;
using std::istringstream;
using std::endl;
using std::cerr;
using std::make_pair;
using std::cout;

Dictionary *Dictionary::_pInstance = nullptr;
Dictionary::AutoRelease Dictionary::_ar;

Dictionary* Dictionary::createInstance()
{
    if(_pInstance == nullptr){
        _pInstance = new Dictionary();
    }
    return _pInstance;
}

void Dictionary::init()   //通过词典文件路径初始化词典
{
    ifstream enDictIfs(dict_en);
    if(!enDictIfs.good()){
        cerr << "open EnDict file fail" << endl;
    }

    ifstream cnDictIfs(dict_cn);
    if(!enDictIfs.good()){
        cerr << "open CnDict file fail" << endl;
    }

    ifstream enIndexIfs(index_en);
    if(!enDictIfs.good()){
        cerr << "open EnIndex file fail" << endl;
    }

    ifstream cnIndexIfs(index_cn);
    if(!enDictIfs.good()){
        cerr << "open CnIndex file fail" << endl;
    }

    string line, word;
    int frequency, index;
    //插入英文词典
    cout << "加载英文词典..." << endl;
    while(getline(enDictIfs, line)){
        istringstream iss(line);
        iss >> word >> frequency;
        _dict.push_back(make_pair(word, frequency));
    }
    //中文词典的偏移量
    cout << "英文词典 size: " << _dict.size() << endl;

    size_t cnDictOffset = _dict.size();

    //插入中文词典
    cout << "加载中文词典..." << endl;
    while(getline(cnDictIfs, line)){
        istringstream iss(line);
        iss >> word >> frequency;
        _dict.push_back(make_pair(word, frequency));
    }

    cout << "中文词典 size: " << _dict.size() - cnDictOffset << endl;

    //插入英文索引
    cout << "加载英文索引..." << endl;
    while(getline(enIndexIfs, line)){
        istringstream iss(line);
        iss >> word;
        while(iss >> index){
            _indexTable[word].insert(index);
        }
    }

    //插入中文索引
    cout << "加载中文索引..." << endl;
    while(getline(cnIndexIfs, line)){
        istringstream iss(line);
        iss >> word;
        while(iss >> index){
            _indexTable[word].insert(cnDictOffset + index);
        }
    }
    
    //关闭文件流
    enDictIfs.close();
    cnDictIfs.close();
    enIndexIfs.close();
    cnIndexIfs.close();
}

vector<pair<string, int>> &Dictionary::getDict()
{
    return _dict;
}

map<string, set<int>> &Dictionary::getIndexTable() //获取索引库
{
    return _indexTable;
}    

