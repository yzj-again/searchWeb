/*************************************************************************
    > File Name: TcpConnection.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 20时12分52秒
 ************************************************************************/

#include "../../include/server/TcpConnection.hh"
#include "../../include/server/EventLoop.hh"

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::ostringstream;

TcpConnection::TcpConnection(int fd, EventLoop * loop)
: _sock(fd)
, _sockIO(fd)
, _loop(loop)
, _hostAddr(getHostAddr())
, _peerAddr(getPeerAddr())
{ }

void TcpConnection::send(const string &msg)
{ _sockIO.writen(msg.c_str(), msg.size()); }

void TcpConnection::sendInLoop(const string & msg)
{
    if(_loop)
    {
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
    }
}

string TcpConnection::recv()
{
    char buff[65535] = {0};
    //int ret = _sockIO.readLine(buff, sizeof(buff));
    int ret = _sockIO.readJson(buff, sizeof(buff));
    return string(buff);
}

string TcpConnection::toString()
{
    ostringstream oss;
    oss << _hostAddr.ip() << ":"
        << _hostAddr.port() << "---->"
        << _peerAddr.ip() << ":"
        << _peerAddr.port();

    return oss.str();
}

bool TcpConnection::isClosed() const
{
    char buf[10] = {0};
    int ret = ::recv(_sock.fd(), buf, sizeof(buf), MSG_PEEK);
    return 0 == ret;
}

InetAddress TcpConnection::getHostAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getsockname(_sock.fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret) { perror("getsockname"); }

    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getpeername(_sock.fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret) { perror("getsockname"); }

    return InetAddress(addr);
}

void TcpConnection::handleConnectionCallback()
{
    if(_onConnectionCb)
    { _onConnectionCb(shared_from_this()); }
}

void TcpConnection::handleMessageCallback()
{
    if(_onMessageCb)
    { _onMessageCb(shared_from_this()); }
}

void TcpConnection::handleCloseCallback()
{
    if(_onCloseCb)
    { _onCloseCb(shared_from_this()); }
}