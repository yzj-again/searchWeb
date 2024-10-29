#include "../../include/DictBuilder/CppJiebaCn.hh"

vector<string> CppJieba::cut(const string &sentence){
    vector<string> words;
    _jieba.CutAll(sentence, words); // FullSegment
    return words;
}