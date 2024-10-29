#ifndef __PAGE_LIB_PREPROCESSOR_HH
#define __PAGE_LIB_PREPROCESSOR_HH 


#include "Configuration.hh"
#include "WordSegmentation.h"
#include "WebPage.hh"

#include <algorithm>
#include <vector>
#include <utility>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <list>
#include <unordered_map>
#include <cmath>

using std::bind2nd;
using std::remove_if;
using std::ofstream;
using std::unordered_map;
using std::list;
using std::cout;
using std::endl;
using std::cerr;
using std::unordered_map;
using std::vector;
using std::pair;
using std::make_pair;
using std::string;
using std::map;
using std::ifstream;
using std::ios;
using std::istringstream;

class PageLibPreprocessor 
{
public:
    PageLibPreprocessor(Configuration& conf);
    void doProcess();

private:
    void readInfoFromFile();  // 根据配置信息读取网页库和偏移库的内容
    void cutRedundantPages();  // 对冗余的网页进行去重
    void buildInvertIndexTable();  // 创建倒排索引表
    void storeOnDisk();  // 写回磁盘
private:
    WordSegmentation _jieba;  // 分词对象
    list<WebPage> _pageList;  // 网页库容器对象
    unordered_map<int, pair<int, int>> _offsetLib;  // 网页库偏移对象
    unordered_map<string, vector<pair<int, double>>> _invertIndexTable;  // 倒排索引表对象 
    map<string, string> _config;  // 配置文件信息
};


#endif  // __PAGE_LIB_PREPROCESSOR_HH 
