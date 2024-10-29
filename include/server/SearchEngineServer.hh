/*************************************************************************
    > File Name: SearchEngineServer.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年08月01日 星期一 17时15分39秒
 ************************************************************************/
#ifndef __SEARCHENGINESERVER_HH
#define __SEARCHENGINESERVER_HH

#include "TcpServer.hh"
#include "ThreadPool.hh"
#include "KeyRecommander.hh"
#include "WebPageSearcher.hh"
//#include "ProtocalParser.hh"
#include "MyRedis.hh"
#include "WebPageQuery.hh"
#include "Dictionary.hh"
#include "MyJson.hh"

class SearchEngineServer
{
public:
    SearchEngineServer(const string & ip, unsigned short port, int threadNum, int queSize);
    ~SearchEngineServer();
    void start();
    void stop();
private:
    void onConnection(const TcpConnectionPtr & conn);
    void onMessage(const TcpConnectionPtr & conn);
    void onClose(const TcpConnectionPtr & conn);
    void doTaskThread(const TcpConnectionPtr & conn, const string msg);
private:
    TcpServer _tcpServer;
    ThreadPool _pool;
    MyRedis _redis;
    WebPageQuery _pageQuery;
    KeyRecommander _keyRecommander;
    WebPageSearcher _webSearcher;
    //ProtocalParser _protocalParser;
    const set<string> _stopWordsList;
};

#endif