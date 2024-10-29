#ifndef __LRUCACHE_HH
#define __LRUCACHE_HH 


#include "Configuration.hh"

#include <list>
#include <fstream>
#include <utility>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

using std::ios;
using std::pair;
using std::list;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::make_pair;
using std::unordered_map;

class LRUCahe 
{
    using ElemType = string;
public:
    LRUCahe(int capacity);  // 初始化 LRU 缓存的容量大小
    ~LRUCahe();
    void addElement(const string& key, const ElemType& value);  // 往缓存中添加元素
    void readFromFile(const string& filename);  // 从文件中读取数据一般用于系统启动
                                                // 的时候从缓存中读取之前保存的持久
                                                // 化数据
    void writeToFile(const string& filename);  // 将缓存中的信息写入文件

private:
    // 用于查找 LRUCache 中是否存在该数据 <关键字，迭代器::<关键字，网页数据>>
    unordered_map<string, list<pair<string, ElemType>>::iterator> _hashMap;  
    
    // 用于实现 LRU 内部逻辑的链表 <关键字，网页数据> 
    // 其中越靠近 “链表头” 表示数据越新，越靠近 “链表尾” 表示数据越旧
    list<pair<string, ElemType>> _resultList;      
    list<pair<string, ElemType>> _pendingUpdateList;  // 作用有待考究
    int _capacity;  // Cache 的容量
};


#endif  // __LRUCACHE_HH 
