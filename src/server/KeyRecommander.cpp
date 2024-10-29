#include "../../include/server/KeyRecommander.hh"
#include "../../include/server/Dictionary.hh"

#include <iostream>
using std::endl;
using std::cout;
using std::priority_queue;
/*
vector<string> KeyRecommander::doQuery(const string &key)    //执行查询
{

}
*/

KeyRecommander::KeyRecommander(Dictionary *dict)
: _dict(dict)
{

}

string KeyRecommander::doQuery(const string &query)
{ 
	_queryWord = query;
    vector<std::pair<string,int>> &dict = _dict->getDict();
    map<string,set<int>> &index = _dict->getIndexTable();
    //将词组拆分为单个word

    size_t charNums = getCharNum_UTF8(_queryWord);
	
	//cout << "charNums: " << charNums << endl;


	set<string> uniqSubWord;	//保存去重后的单个word
	map<string,int> uniqWord;	//保存去重后的候选词
    
	for(size_t idx = 0, n = 0; n!= charNums; ++idx, ++n){
        size_t charLen = getByteNum_UTF8(_queryWord[idx]);
        string subWord = _queryWord.substr(idx, charLen);
		
		//cout << "charLen: " << charLen << endl;
		//cout << "subWord: " << subWord << endl;

		uniqSubWord.insert(subWord);

        idx += (charLen - 1);
    }

	//cout << "uniqSubWord size: " << uniqSubWord.size() << endl;


	for(auto &uniqSubWord : uniqSubWord)
	{
		auto indexIt = index.find(uniqSubWord);
		if(indexIt != index.end()){
            for(auto &index : indexIt->second){ 
				////cout << "index No: " << index << endl;
				
				string word = dict[index].first;
				int frequency = dict[index].second;
				//cout << "word: " << word << " frequency: " << frequency << endl;
				auto ret = uniqWord.insert({word, frequency});
            }
        }else{
            //该subWord未在索引表中
            continue;
        }
	}
	//cout << "uniqWord size: " << uniqWord.size() << endl;

	
	priority_queue<CandidateResult> resultQue;
	//插入优先级队列
	for(auto &uniqWord : uniqWord)
	{
		CandidateResult candidateResult;
        candidateResult._word = uniqWord.first;
        candidateResult._freq = uniqWord.second;
        candidateResult._dist = distance(candidateResult._word);
        resultQue.push(candidateResult);
	}			


    //判断resultQue结果,暂定返回五个，不足五个全部返回
	string result;
    if(resultQue.empty()){
		result = "#";
    }else{
        int size = resultQue.size();
		
		//cout << "resultQue size: " << size << endl;
		
		if(size < 5){
			while(size--){
				result += resultQue.top()._word;
				result += " ";
				resultQue.pop();
			}
		}else{
			int cnt = 5;
			while(cnt--){
				result += resultQue.top()._word;
				result += " ";
				resultQue.pop();
			}
		}
    }

	return result;
}

size_t KeyRecommander::getByteNum_UTF8(const char byte)
{
    size_t byteNum = 0;
    for(size_t i = 0; i < 6; ++i)
    {
        if(byte & (1 << (7 - i))){
            ++byteNum;
        }else{
            break;
        }
    }
    return byteNum == 0 ? 1 : byteNum;
}

size_t KeyRecommander::getCharNum_UTF8(const string &str)
{
	size_t CharNum = 0;
	for(size_t idx = 0; idx != str.size(); ++idx)
	{
		int nBytes = getByteNum_UTF8(str[idx]);
		idx += (nBytes - 1);
		++CharNum;
	}
	return CharNum;
}

int KeyRecommander::triple_min(const int &a, const int &b, const int &c)
{
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int KeyRecommander::distance(const string & rhs)
{
    size_t lhs_len = getCharNum_UTF8(this->_queryWord);
    size_t rhs_len = getCharNum_UTF8(rhs);

	int editDist[lhs_len + 1][rhs_len + 1];
	for(size_t idx = 0; idx <= lhs_len; ++idx)
	{
		editDist[idx][0] = idx;
	}

	for(size_t idx = 0; idx <= rhs_len; ++idx)
	{
		editDist[0][idx] = idx;
	}
	
	std::string sublhs, subrhs;
	for(std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx)
	{
		size_t nBytes = getByteNum_UTF8(this->_queryWord[lhs_idx]);
		sublhs = this->_queryWord.substr(lhs_idx, nBytes);
		lhs_idx += (nBytes - 1);

		for(std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx)
		{
			nBytes = getByteNum_UTF8(rhs[rhs_idx]);
			subrhs = rhs.substr(rhs_idx, nBytes);
			rhs_idx += (nBytes - 1);
			if(sublhs == subrhs)
			{
				editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
			}
			else
			{
				editDist[dist_i][dist_j] = triple_min(
					editDist[dist_i][dist_j - 1] + 1,
					editDist[dist_i - 1][dist_j] + 1,
					editDist[dist_i - 1][dist_j - 1] + 1);
			}
		}
	}
	return editDist[lhs_len][rhs_len];
}

