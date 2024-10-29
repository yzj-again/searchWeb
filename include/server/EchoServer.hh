/*************************************************************************
    > File Name: EchoServer.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月29日 星期五 09时41分50秒
 ************************************************************************/
#ifndef __ECHOSERVER_HH
#define __ECHOSERVER_HH

#include "TcpServer.hh"
#include "ThreadPool.hh"

#include <iostream>

using std::cout;
using std::endl;

class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con)
    : _msg(msg)
    , _con(con)
    { }

    void process()
    { 
        //业务处理
        //处理完成之后，将消息发出去，需要一个TCP连接
        _con->sendInLoop(_msg);
    }

private:
    string _msg;
    TcpConnectionPtr _con;
};

class EchoServer
{
public:
    EchoServer(const string & ip, unsigned short port, int threadNum, int queSize)
    : _server(ip, port)
    , _pool(threadNum, queSize)
    { }

    ~EchoServer() { }

    void start()
    {
        _pool.start();
        using namespace std::placeholders;
        _server.setAllCallback(std::bind(&EchoServer::onConnection, this, _1),
                          std::bind(&EchoServer::onMessage, this, _1),
                          std::bind(&EchoServer::onClose, this, _1));
        _server.start();
    }

    void stop()
    {
        _pool.stop();
        _server.stop();
    }
private:
    void onConnection(const TcpConnectionPtr & con)
    {
        cout << con->toString() << " has connected!" << endl;
    }

    void onMessage(const TcpConnectionPtr & con)
    {
        string msg = con->recv();
        cout << ">>receive from client: " << msg << endl;
        MyTask task(msg, con);
        _pool.addTask(std::bind(&MyTask::process, task));
    }

    void onClose(const TcpConnectionPtr & con)
    {
        cout << con->toString() << " has closed!" << endl;
    }
private:
    TcpServer _server;
    ThreadPool _pool;
};

#endif

