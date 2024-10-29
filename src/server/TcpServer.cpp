/*************************************************************************
    > File Name: TcpServer.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 21时51分17秒
 ************************************************************************/

#include "../../include/server/TcpServer.hh"

TcpServer::TcpServer(const string & ip, unsigned short port)
: _acceptor(ip, port)
, _loop(_acceptor)
{ }

TcpServer::~TcpServer()
{

}

void TcpServer::start()
{
    _acceptor.ready();
    _loop.loop();
}

void TcpServer::stop()
{
    _loop.unloop();
}

void TcpServer::setAllCallback(TcpConnectionCallback && onConnection, 
                               TcpConnectionCallback && onMessage, 
                               TcpConnectionCallback && onClose)
{
    _loop.setConnectionCallback(std::move(onConnection));
    _loop.setMessageCallback(std::move(onMessage));
    _loop.setCloseCallback(std::move(onClose));
}

