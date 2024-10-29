/*************************************************************************
    > File Name: TcpServer.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 21时51分30秒
 ************************************************************************/

#ifndef __TCPSERVER_HH
#define __TCPSERVER_HH

#include "EventLoop.hh"
#include "Acceptor.hh"

class TcpServer
{
public:
    TcpServer(const string & ip, unsigned short port);
    ~TcpServer();
    void start();
    void stop();
    void setAllCallback(TcpConnectionCallback &&, TcpConnectionCallback &&, TcpConnectionCallback &&);
private:
    Acceptor _acceptor;
    EventLoop _loop;
};

#endif