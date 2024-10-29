#pragma once
#ifndef __WORDSEGMENTATION_H
#define __WORDSEGMENTATION_H


#include "../../lib/cppjieba/Jieba.hpp"
#include "Configuration.hh"

#include <vector>
#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
using std::vector;

const string DICT_PATH = getPath("DICT_PATH");  // 最大概率法(MPSegment: Max Probability)分词所使用的词典路径
const string HMM_PATH = getPath("HMM_PATH");  // 隐式马尔科夫模型(HMMSegment: Hidden Markov Model)分词所使用的词典路径
const string USER_DICT_PATH = getPath("USER_DICT_PATH");  // 用户自定义词典路径
const string IDF_PATH = getPath("IDF_PATH");  // IDF路径
const string STOP_WORD_PATH = getPath("STOP_WORD_PATH");  // 停用词路径
                                                                     
class WordSegmentation  // 使用结巴分词库进行分词
{
public:
    WordSegmentation()
    : _jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH,IDF_PATH, STOP_WORD_PATH)  //初始化Jieba类对象
    {
        cout << "cppjieba初始化成功!" << endl;
    }

    vector<string> operator()(const string str)  //返回str的分词结果
    {
        vector<string> words;
        _jieba.CutAll(str, words);  // FullSegment
        return words;
    }

private:
    cppjieba::Jieba _jieba;
};


#endif  // __WORDSEGMENTATION_H

