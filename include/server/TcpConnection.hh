/*************************************************************************
    > File Name: TcpConnection.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 14时39分15秒
 ************************************************************************/
#ifndef __TCPCONNECTION_HH
#define __TCPCONNECTION_HH

#include "Socket.hh"
#include "SocketIO.hh"
#include "InetAddress.hh"

#include <memory>
#include <functional>

using std::shared_ptr;
using std::function;
/**
 * @brief 
 * 
 */
class TcpConnection;
class EventLoop;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;

class TcpConnection
: public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(int fd, EventLoop * loop);
    ~TcpConnection() { };

    void send(const string & msg);
    void sendInLoop(const string & msg);
    string recv();
    string toString();

    bool isClosed() const;

    void setConnectionCallback(const TcpConnectionCallback & cb)
    { _onConnectionCb = std::move(cb); }
    void setMessageCallback(const TcpConnectionCallback & cb)
    { _onMessageCb = std::move(cb); }
    void setCloseCallback(const TcpConnectionCallback & cb)
    { _onCloseCb = std::move(cb); }

    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    InetAddress getHostAddr();
    InetAddress getPeerAddr();
private:
    EventLoop * _loop;
    Socket      _sock;
    SocketIO    _sockIO;
    InetAddress _hostAddr;
    InetAddress _peerAddr;
    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;
};

#endif