/*************************************************************************
    > File Name: WebPageQuery.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年08月02日 星期二 20时56分23秒
 ************************************************************************/
#ifndef __WEBPAGEQUERY_HH
#define __WEBPAGEQUERY_HH

#include "WordSegmentation.h"
#include "WebPage.hh"

#include <unordered_map>
#include <set>

using std::unordered_map;
using std::pair;
using std::set;

class WebPageQuery
{
public:
    WebPageQuery();
    //获取候选文章Id
    vector<int> doQuery(const string & str);
    //获取文章Id对应的文章，顺序一一对应
    vector<vector<pair<string, string>>> getWebPage(const vector<int> docId);
private:
    //加载偏移库和倒排索引库
    void loadLibrary();
    //获取查询词在查询语句中的的权重，以频率代替
    vector<double> getQueryWordWeightVector(vector<string> & queryWords);
    //获取查询词对应的文章Id，以及在该文中对应查询词的权重，结果置于resultVec中
    bool executeQuery(const vector<string> & queryWords, 
            vector<pair<int, vector<double>>> & resultVec);
    //计算两篇文章的余弦相似度
    double calSimilarity(const vector<double> &lhs, const vector<double> &rhs);
private:
    WordSegmentation  _jieba;               //分词工具
    const set<string> _stopWordsList;       //停词库
    const int _resultNum = 5;               //推荐文章的最大数量
    unordered_map<int, pair<int, int>> _offsetLib;  //docId--startPosition--length
    unordered_map<string, set<pair<int, double>>> _invertIndexTable; //word--docId--weight
};

//用于优先级队列的函数对象
struct SetCompare
{
    bool operator()(const pair<int, double> & lhs, const pair<int, double> & rhs)
    {
        return lhs.first < rhs.first;
    }
};


struct QueCompare
{
    bool operator()(const pair<int, double> & lhs, const pair<int, double> & rhs)
    {
        return lhs.second < rhs.second;
    }
};
#endif