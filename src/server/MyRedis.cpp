/*************************************************************************
    > File Name: Redis.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年08月02日 星期二 11时33分42秒
 ************************************************************************/

#include "../../include/server/MyRedis.hh"
#include "../../include/server/WebPage.hh"

#include <unistd.h>
#include <iostream>
#include <iterator>

using std::cout;
using std::endl;
using std::cerr;

/**
 * @brief 数据存放还应该添加超时时间，暂未添加
 *  
 */

MyRedis::MyRedis(const string connStr)
: _connStr(connStr)
{ 
    _pRedis = connect();
    if(_pRedis != nullptr) 
    { 
        cout << "Redis连接成功" << endl; 
        _pRedis->flushdb();
    }
    else { cout << "Redis连接失败" << endl; }
}

MyRedis::~MyRedis()
{
    if(_pRedis) { delete _pRedis; }
}

Redis * MyRedis::connect()
{
    try
    {
        Redis * pRedis = new Redis(_connStr);
        return pRedis;
    } catch (const Error &err){
        cerr << "Redis Error: " << err.what() << endl;
        return nullptr;
    }
}

bool MyRedis::hashInsert(int docId, vector<pair<string, string>> page)
{
    //根据文章Id插入文章
    _pRedis->hmset(std::to_string(docId), page.begin(), page.end());
    return true;
}

bool MyRedis::stringInsert(string word, string result)
{
    //根据关键字插入结果
    return _pRedis->set(word, result);
}

unordered_map<string, string> MyRedis::hashSearch(int docId)
{
    //根据文章Id获取文章
    unordered_map<string, string> pageMap;
    _pRedis->hgetall(std::to_string(docId), std::inserter(pageMap, pageMap.begin()));
    return std::move(pageMap);
}

string MyRedis::stringSearch(string word)
{
    //根据关键字获取结果
    //Optional<string> val = _pRedis->get(word);
    auto val = _pRedis->get(word);
    
    return val ? *val : "";
}