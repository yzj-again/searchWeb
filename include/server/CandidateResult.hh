#ifndef __CANDIDATERESULT_HH__
#define __CANDIDATERESULT_HH__

#include <iostream>
#include <string>

using std::string;
class CandidateResult
{
public:
    string _word;
    int _freq;
    int _dist;
};

/*
1) 实现最小编辑距离核心算法 ---- 计算候选词与查询词的相似度
2) 候选词选择策略：hello helli
a.优先比较最小编辑距离；
b.在编辑距离相同的条件下，再比较候选词的词频；词频越大优先选择
c.在词频相同的条件下，按字母表顺序比较候选词；
3) 获取 k 个（例如 3 个或者 5 个）候选词，返回给客户端。（需要使用优先级队
列
*/
//模板的全特化
namespace std
{
template <>
struct less<CandidateResult>
{
    bool operator()(const CandidateResult &lhs, const CandidateResult &rhs)
    {
        if(lhs._dist > rhs._dist)
        {
            return true;
        }else if(lhs._dist == rhs._dist)
        {
            if(lhs._freq < rhs._freq)
            {
                return true;
            }
            else if(lhs._freq == rhs._freq)
            {
                if(lhs._word > rhs._word)
                {
                    return true;
                }
            }
        }
        return false;
    }
};  //end of struct less
}   //end of namespace std


#endif