/*************************************************************************
    > File Name: SearchEngineServer.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年08月01日 星期一 17时16分03秒
 ************************************************************************/
#include "../../include/server/SearchEngineServer.hh"
#include "../../include/server/Mylogger.h"
#include <iostream>

using std::cout;
using std::endl;

SearchEngineServer::SearchEngineServer(const string & ip, unsigned short port, int threadNum, int queSize)
: _pool(threadNum, queSize)
, _tcpServer(ip, port)
, _redis()
, _pageQuery()
, _webSearcher(_redis, _pageQuery)
{

}

SearchEngineServer::~SearchEngineServer()
{

}

void SearchEngineServer::start()
{
    _pool.start();
    using namespace std::placeholders;
    _tcpServer.setAllCallback(std::bind(&SearchEngineServer::onConnection, this, _1),
                        std::bind(&SearchEngineServer::onMessage, this, _1),
                        std::bind(&SearchEngineServer::onClose, this, _1));
    _tcpServer.start();
}

void SearchEngineServer::onConnection(const TcpConnectionPtr & conn)
{
    cout << conn->toString() << " has connected!" << endl;
    LogInfo("%s has connected!", conn->toString().c_str());
}

void SearchEngineServer::onMessage(const TcpConnectionPtr & conn)
{
    string msg = conn->recv();
    cout << ">>receive from client: " << msg << endl;
    LogInfo("receive from client:\n%s", msg.c_str());
    _pool.addTask(std::bind(&SearchEngineServer::doTaskThread, this, conn, msg));
}

void SearchEngineServer::onClose(const TcpConnectionPtr & conn)
{
    cout << conn->toString() << " has closed" << endl;
    LogInfo("%s has closed", conn->toString().c_str());
}

void SearchEngineServer::doTaskThread(const TcpConnectionPtr & conn, const string msg)
{
    cout << "thread_idx : " << threadIdx << endl;
    string query;
    MyJson mJ;
    MsgType type = mJ.from_json(msg, query);
    //cout << "查询：" << query << endl;
    if(MsgType::WEBPAGESEARCH == type) { conn->send(this->_webSearcher.doQuery(query)); }
    //以下部分封装到KeyRecommander中更好
    else if(MsgType::KEYRECOMMAND == type) 
    { 
        //先到缓存中查找
        string ret = _redis.stringSearch(query);
        //缓存中没有就到文件中查找，并将结果插入缓存
        if("" == ret) { ret = this->_keyRecommander.doQuery(query); }
        _redis.stringInsert(query, ret);
        cout << ret << endl;
        //没有查找到相关词
        if("#" == ret) { ret = mJ.to_json("", MsgType::NOMESSAGE); }
        else { ret = mJ.to_json(ret, MsgType::KEYRECOMMAND); }
        conn->send(ret); 
    }
}