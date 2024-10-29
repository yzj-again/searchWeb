/*************************************************************************
    > File Name: MyJson.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年08月04日 星期四 09时16分52秒
 ************************************************************************/
#include "../../include/client/MyJson.hh"

string MyJson::to_json(string sendMsg, MsgType type)
{
    json msgJson;
    msgJson["msgId"] = type;
    msgJson["msg"] = sendMsg;
    return msgJson.dump(4);
}

MsgType MyJson::from_json(string & query, vector<vector<pair<string, string>>> & webResult, string &keyResult)
{
    json msgJson = json::parse(query);
    if(msgJson["msgId"] == MsgType::KEYRECOMMAND){
        keyResult = msgJson["msg"];
    }else if(msgJson["msgId"] == MsgType::WEBPAGESEARCH){
        webResult = msgJson["msg"];
    }
    return msgJson["msgId"];
}

MsgType MyJson::from_json1(string & query, string & queryResult)
{
    json msgJson = json::parse(query);
    queryResult = msgJson["msg"];
    return msgJson["msgId"];
}

MsgType MyJson::from_json2(string & query, vector<vector<pair<string, string>>> & queryResult)
{
    json msgJson = json::parse(query);
    queryResult = msgJson["msg"];
    return msgJson["msgId"];
}

