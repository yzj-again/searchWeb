#ifndef __PAGELIB_H__
#define __PAGELIB_H__

#include "Configuration.hh"
#include "DirScanner.hh"
#include "../../lib/tinyxml2.h"  

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <utility>

using std::map;
using std::vector;
using std::string;
using std::pair;

class PageLib
{
public:
    PageLib();
    PageLib(Configuration * conf,DirScanner & dirScanner);
    ~PageLib();
    void create();//执行网页分析
    void storeArticle();//生成网页库
    void storeOffsetLib();//生成偏移
    vector<string> getArticle ();//获取网页库
    map<int ,pair<int,int>> getOffsetLib();//获取网页便宜库


private:
    DirScanner & _dirScanner;
    Configuration * _config;
    vector<string> _files;//网页库,每个下标存一篇文章,从下标0开始
    map<int ,pair<int,int>> _offsetLib;//偏移库,从下标1开始

};

#endif
