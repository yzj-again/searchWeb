/*************************************************************************
    > File Name: WebPageSearcher.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年08月02日 星期二 20时57分26秒
 ************************************************************************/

#include "../../include/server/WebPageSearcher.hh"
#include "../../include/server/MyJson.hh"

#include <unordered_map>

using std::unordered_map;

WebPageSearcher::WebPageSearcher(MyRedis & redis, WebPageQuery & pageQuery)
: _redis(redis)
, _pageQuery(pageQuery)
{ }

string WebPageSearcher::doQuery(const string & sought)
{
    vector<vector<pair<string, string>>> docs;
    //获取候选文章Id
    vector<int> docIds = _pageQuery.doQuery(sought);
    if(0 == docIds.size()) { return returnNoAnswer(); }

    //存放未在缓存中的文章Id
    vector<int> docIdsInDisk;

    for(auto id : docIds)
    {
        //先检查是否在缓存中
        //cout << "id = " << id << endl;
        unordered_map<string, string> val = _redis.hashSearch(id);
        //不在缓存中则记录相应id，并用空vector占位（保证从磁盘取出文章够保持相应的顺序）
        if(0 == val.size())
        { 
            docIdsInDisk.push_back(id); 
            docs.push_back(vector<pair<string, string>>());
        }
        //在缓存中则将相应内存放到结果中
        else
        { 
            docs.emplace_back(vector<pair<string, string>> {
                                {"标题", val["标题"]}, 
                                {"链接", val["链接"]}, 
                                {"摘要", val["摘要"]}}
                              ); 
            cout << "Redis命中《"<< val["标题"] << "》" << endl;
        }
    }
    
    //存放从磁盘获取的查询结果，一个vector<pair<string, string>>代表一篇文章
    vector<vector<pair<string, string>>> docInDisk;

    if(0 != docIdsInDisk.size())
    {
        //从磁盘获取文章，并将结果存放到缓存中
        docInDisk = _pageQuery.getWebPage(docIdsInDisk);
        for(int idx = 0; idx < docIdsInDisk.size(); ++idx)
        { 
            _redis.hashInsert(docIdsInDisk[idx], docInDisk[idx]); 
            //cout << docIdsInDisk[idx] << " : " << docInDisk[idx][0].second << endl;
        }
    }

    //将从磁盘获取的文章放到相应的位置
    int index = -1;
    for(auto & page : docs)
    {
        if(0 == page.size()) 
        { page = std::move(docInDisk[++index]); }
    }

    //转化为JSON格式字符串
    MyJson mJ;
    return mJ.to_json(docs, MsgType::WEBPAGESEARCH);
}

string WebPageSearcher::returnNoAnswer()
{
    MyJson mJson;
    vector<vector<pair<string, string>>> docs;
    return mJson.to_json(docs, MsgType::NOMESSAGE);
}