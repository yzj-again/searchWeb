#ifndef __SPLITTOOLCPPJIEBAEN_H__
#define __SPLITTOOLCPPJIEBAEN_H__

#include "SplitTool.hh"
#include "Configuration.hh"
#include <string>
#include <vector>
#include <sstream>

using std::string;
using std::vector;
using std::istringstream;


class CppJiebaEn
: public SplitTool
{
public:
    CppJiebaEn()
    {
        
    }

    ~CppJiebaEn() {};

    vector<string> cut(const string &sentence) override; //分词函数

private:
    string cleanup_str(const string &word); //针对英文单词大写转小写
};

#endif
