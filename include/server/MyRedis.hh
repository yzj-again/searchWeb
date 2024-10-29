/*************************************************************************
    > File Name: MyRedis.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年08月02日 星期二 11时33分25秒
 ************************************************************************/
#ifndef __REDIS_HH
#define __REDIS_HH

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <sw/redis++/redis++.h>
#include <unordered_map>

using std::string;
using std::unordered_set;
using std::vector;
using std::vector;
using std::unordered_map;
using std::pair;

using namespace sw::redis;

class MyRedis
{
public:
    MyRedis(const string connStr = "tcp://127.0.0.1:6379");
    ~MyRedis();
    //hash结构用于网页查询，字段分别为 文章编号，文章标题， 摘要
    //string结构用于关键字推荐，字段分别为 查询关键字，查询结果的拼接
    bool hashInsert(int docId, vector<pair<string, string>>);
    bool stringInsert(string word, string result);
    unordered_map<string, string> hashSearch(int docId);
    string stringSearch(string word);
  
private:
    Redis * connect();
private:
    string _connStr;
    Redis * _pRedis;
};

// cout << vector 
template <typename T> 
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) 
{ 
    if (!v.empty()) 
    { 
        os << '['; 
        std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, ", ")); 
        os << "\b\b]"; // 删除末尾的", " 
    }
    return os; 
}

// cout << unordered_map 
template <typename T, typename U> 
std::ostream &operator<<(std::ostream &os, const std::unordered_map<T, U> &umap) 
{ 
    os << '['; 
    for (auto item : umap)
    { 
        os << "(" << item.first << "," << item.second << "),"; 
    }
    os << "\b]"; // 删除末尾的"," 
    return os; 
}

// cout << unorderd_set
template <typename T> 
std::ostream &operator<<(std::ostream &os, const std::unordered_set<T> &uset) 
{ 
    os << '('; 
    for (auto item : uset) 
    { os << item << ","; }
    os << "\b)"; // 删除末尾的"," 
    return os; 
}
#endif
