/*************************************************************************
    > File Name: MyJson.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年08月04日 星期四 09时08分48秒
 ************************************************************************/
#ifndef __MYJSON_HH
#define __MYJSON_HH

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
using std::string;
using std::vector;
using std::pair;

enum class MsgType{KEYRECOMMAND = 1, WEBPAGESEARCH, NOMESSAGE};

class MyJson
{
    using json = nlohmann::json;
public:
    string to_json(string sendMsg, MsgType type);
    MsgType from_json(string & query, vector<vector<pair<string, string>>> & webResult, string &keyResult);
    MsgType from_json1(string & query, string & queryResult);
    MsgType from_json2(string & query, vector<vector<pair<string, string>>> & queryResult);
};

#endif