#include "../../include/DictBuilder/CppJiebaEn.hh"

vector<string> CppJiebaEn::cut(const string &sentence){ //分词函数
    vector<string> words;
    istringstream iss(sentence);

    string word;
    while(iss >> word){
        word = cleanup_str(word);   //对单词进行过滤，并转换为小写
        if(word.length() != 0){  //过滤空字符串
            words.push_back(word);
        }
    }
    return words;
}


string CppJiebaEn::cleanup_str(const string &word) //针对英文单词大写转小写
{
    string ret;
    for(auto it = word.begin(); it != word.end(); ++it){
        if(isalpha(*it)){
            ret += tolower(*it);
        }
    }
    return ret;
}