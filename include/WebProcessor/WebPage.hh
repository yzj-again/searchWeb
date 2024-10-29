#ifndef __WEBPAGE_HH
#define __WEBPAGE_HH

#include "Configuration.hh"
#include "WordSegmentation.h"
#include "Simhash.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>

using std::string;
using std::vector;
using std::map;
using std::set;
using std::priority_queue;

class WebPage
{
    friend bool operator==(const WebPage & lhs, const WebPage & rhs);
public:
    
    WebPage(const string & doc, Configuration & config, WordSegmentation & jieba);
    
    int getDocId(){ return _docId; }
    
    string getDoc(){ return _doc;  }

    uint64_t getSimhashValue(){ return _simValue; }

    vector<string> getTopK(){ return _topWords; }
    
    map<string, int> & getWordsMap(){ return _wordsMap;  }//获取文档的词频统计 map

    struct comp;
private:
    //处理格式化文档
    void processDoc();
    //统计词频
    void storeWordsMap(const string & s);
    //计算文档simhash值
    void makeSimValue();
    //计算词频最高的20词
    void calcTopK(); 
private:
    string _doc;  // 整篇<doc>文档
    int _docId; // 文档id docid
    string _docTitle;//文档标题
    string _docLink;//文档链接
    string _docSummary;//文档摘要
    string _docContent;//文档内容
    map<string, int> _wordsMap;//该文档 <单词，词频>
    vector<string> _topWords;// 词频最高的20词
    Configuration & _config;
    WordSegmentation & _jieba;//jieba中文分词库
    static Simhash _sim; 
    uint64_t _simValue;
};


#endif

