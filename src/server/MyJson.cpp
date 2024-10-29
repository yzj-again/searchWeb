/*************************************************************************
    > File Name: MyJson.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年08月04日 星期四 09时16分52秒
 ************************************************************************/
#include "../../include/server/MyJson.hh"
#include <iostream>

/**
 * @brief MsgType应该封装在协议解析中，用私有协议指明Json字符串长度，避免粘包，此处暂未作此处理
 * 
 */

string MyJson::to_json(vector<vector<pair<string, string>>> & sendMsg, MsgType type)
{
    json msgJson;
    msgJson["msgId"] = type;
    msgJson["msg"] = sendMsg;
    return msgJson.dump(4);
}

string MyJson::to_json(string sendMsg, MsgType type)
{
    json msgJson;
    msgJson["msgId"] = type;
    msgJson["msg"] = sendMsg;
    return msgJson.dump(4);
}

MsgType MyJson::from_json(const string & msg, string & query)
{
    //std::cout << "111" << std::endl;
    json msgJson = json::parse(msg);
    query = msgJson["msg"];
    //std::cout << query << std::endl;
    return msgJson["msgId"];
}
