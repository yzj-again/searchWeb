#ifndef __KEYRECOMMANDER_HH__
#define __KEYRECOMMANDER_HH__

#include "./CandidateResult.hh"
#include "./Dictionary.hh"
#include <string>
#include <queue>
#include <set>

using std::string;
using std::set;

class Dictionary;   //类的前向声明
class KeyRecommander{
public:
    KeyRecommander(Dictionary *dict = Dictionary::createInstance());    
    string doQuery(const string &query); //执行查询

private:
    void queryIndexTable(); //查询索引
    void statistic(set<int> &iset); //进行计算
    
    size_t getByteNum_UTF8(const char byte);
    size_t getCharNum_UTF8(const string &str);
    int triple_min(const int &a, const int &b, const int &c);
    int distance(const string & rhs);   //计算最小编辑距离

private:
    string _queryWord;
    Dictionary *_dict;
};



#endif