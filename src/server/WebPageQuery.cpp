/*************************************************************************
    > File Name: WebPageQuery.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年08月02日 星期二 20时57分20秒
 ************************************************************************/

#include "../../include/server/Configuration.hh"
#include "../../include/server/WebPageQuery.hh"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using std::ifstream;
using std::istringstream;
using std::make_pair;
using std::priority_queue;

WebPageQuery::WebPageQuery()
: _jieba(WordSegmentation())
, _stopWordsList(Configuration::getInstance()->getStopWordList())
{ loadLibrary(); }

vector<int> WebPageQuery::doQuery(const string & str)
{
    //存放返回结果
    vector<int> docIds;
    //先分词
    vector<string> wordCutResult = _jieba(str);
    vector<string> word;
    //去除停用词
    for(auto & it: wordCutResult)
    {
        //cout << it << endl;
        if(_stopWordsList.find(it) == _stopWordsList.end())
        { word.push_back(it); }
    }

    //对结果去重并得到相应的权重，此处用频率替代
    vector<double> weight = getQueryWordWeightVector(word);
    if(0 == word.size()) { return docIds; }

    //获取查询词在某篇文章中的权重，如果没有文章包含全部查询词，则会返回false
    vector<std::pair<int, vector<double>>> candidateVec;
    bool ret = executeQuery(word, candidateVec);
    if(false == ret) { return docIds; }
    
    priority_queue<pair<int, double>, vector<pair<int, double>>, QueCompare> tmpQue;
    
    //计算余弦相似度并放置于优先级队列中
    for(auto & elem : candidateVec)
    {   tmpQue.emplace(elem.first, calSimilarity(weight, elem.second)); }

    //获取最相关的几篇文章
    for(int idx = 0; idx < _resultNum && !tmpQue.empty(); ++idx)
    { 
        docIds.push_back(tmpQue.top().first);
        tmpQue.pop();
    }

    return docIds;
}

vector<vector<pair<string, string>>> WebPageQuery::getWebPage(const vector<int> docIds)
{
    vector<vector<pair<string, string>>> result;

    ifstream file;
    string pageLibPath = getPath("webpage");
    file.open(pageLibPath);
    if(!file.good())
    { 
        perror("打开网页库文件失败");
        exit(0);
    }

    for(auto id : docIds)
    {
        if(0 > id) { continue; }
        pair<int, int> offset = _offsetLib[id];
        //根据偏移量将读指针移动到对应位置（某一行的开头）
        file.seekg(offset.first, std::ios_base::beg);
        //cout << "offset = " << offset.first << endl;
        string line;
        string doc;
        //由于一篇文章存放一行，则只需要getline
        getline(file, line);
        if(!file.good() && !file.eof()) 
        { 
            std::cerr << "文件流错误" << endl; 
            file.clear();
            result.push_back(vector<pair<string, string>>());
            continue;
        }
        //cout << line << endl;

        //对文章进行相应处理，结果放到result中
        WebPage page(line);
        result.emplace_back( vector<pair<string, string>>{ 
                                {"标题", page.getTitle()}, 
                                {"链接", page.getLink()}, 
                                {"摘要", page.getSummary()}}
                           );
    }

    file.close();
    return std::move(result);
}

void WebPageQuery::loadLibrary()
{
    string offsetLibPath = getPath("newoffset");
    string invertIndexTablePath = getPath("invertindex");
    
    //加载偏移库
    ifstream file;
    file.open(offsetLibPath);
    if(!file.good())
    { 
        cout << offsetLibPath << endl;
        perror("打开网页偏移库文件失败");
        exit(0);
    }
    else { cout << "加载网页偏移库..." << endl; }

    string line;
    while(getline(file, line))
    {
        if('\0' == line[0]) { continue; }
        istringstream iss(line);
        int docId;
        int startPosition;
        int length = 0;
        iss >> docId >> startPosition >> length;
        _offsetLib.emplace(docId, make_pair(startPosition, length));
    }
    file.close();
    if(0 == _offsetLib.size())
    {
        cout << "网页偏移库为空，请检查文件" << endl;
        exit(0);
    }
    else { cout << "加载网页偏移库成功" << endl; }

    //加载倒排索引库
    file.open(invertIndexTablePath);
    if(!file.good())
    { 
        perror("打开倒排索引库文件失败");
        exit(0);
    }
    else { cout << "加载倒排索引库..." << endl; }

    line.clear();
    while(getline(file, line))
    {
        if('\0' == line[0]) { continue; }
        istringstream iss(line);
        string word;
        int docId;
        double weight;
        iss >> word;
        set<pair<int, double>> tmpSet;

        while(iss.good() && !iss.eof())
        {
            iss >> docId >> weight;
            tmpSet.insert(make_pair(docId, weight));
        }

        _invertIndexTable.emplace(word, std::move(tmpSet));
    }
    file.close();
    if(0 == _invertIndexTable.size())
    {
        cout << "倒排索引库为空，请检查文件" << endl;
        exit(0);
    }
    else { cout << "加载倒排索引库成功" << endl; }
}

vector<double> WebPageQuery::getQueryWordWeightVector(vector<string> & queryWords)
{
    vector<double> retVec;
    map<string, int> tmpMap;
    double cnt = 1.0 * queryWords.size();
    for(auto & word : queryWords)
    { ++tmpMap[word]; }

    //需要去除重复关键字
    queryWords.clear();        
    for(auto & elem : tmpMap)
    { 
        queryWords.push_back(elem.first);
        retVec.push_back(elem.second / cnt);
    }

    return retVec;
}

bool WebPageQuery::executeQuery(const vector<string> & queryWords, vector<std::pair<int, vector<double>>> & candidateVec)
{
    //传进来的queryWords至少有一个元素
    //存放每个关键字在某篇文章中的权重
    vector<set<pair<int, double>>> docIdWeightSet;
    //存放包含对应关键字的文章Id
    vector<set<int>> docIdSet;
    for(auto & word : queryWords)
    {
        //cout << word << endl;
        auto ret = _invertIndexTable.find(word);
        //只要有任意词语不在索引库中，则查询失败
        if(_invertIndexTable.end() == ret) { return false; }    

        //获取查询词的倒排索引
        docIdWeightSet.emplace_back(_invertIndexTable[word]);
        docIdSet.emplace_back(set<int>());
        for(auto it : _invertIndexTable[word])
        { docIdSet.back().insert(it.first); }
    }

    //取文章Id集合的交集
    int sz = docIdSet.size() - 1;

    if(docIdSet.size() != 1) 
    {  
        for(int idx = 0; idx < sz; ++idx)
        {
            int rightIdx = docIdSet.size() - 1;
            docIdSet.emplace_back(set<int>());
            std::set_intersection(docIdSet[idx].begin(), docIdSet[idx].end(),
                                docIdSet[rightIdx].begin(), docIdSet[rightIdx].end(),
                                std::inserter(docIdSet.back(), docIdSet.back().begin()));
        }
    } 

    //包含所有关键字的文章Id集合
    set<int>  finalSet = docIdSet.back();

    if(0 == finalSet.size()) { return false; } //交集没有元素
    sz += 1;
    for(auto id : finalSet)   //对每一篇文章
    {
        vector<double> weightInDoc;
        for(int idx = 0; idx < sz; ++idx)   //对每个关键字
        {
            for(auto & elem : docIdWeightSet[idx])  //遍历set，找到对应文章的关键字权重
            {
                if(id == elem.first)
                { 
                    weightInDoc.push_back(elem.second);
                    break; 
                }
            }
        }
        candidateVec.emplace_back(id, weightInDoc);
    }
    return true;
}            

//计算余弦相似度
double WebPageQuery::calSimilarity(const vector<double> &lhs, const vector<double> &rhs)
{
    double dotProduct;
    double labsolute;
    double rabsolute;
    for(int idx = 0; idx < lhs.size(); ++idx)
    { 
        dotProduct += lhs[idx] * rhs[idx]; 
        labsolute += lhs[idx] * lhs[idx];
        rabsolute += rhs[idx] * rhs[idx];
    }

    labsolute = sqrt(labsolute);
    rabsolute = sqrt(rabsolute);
    return dotProduct/(labsolute * rabsolute);
}