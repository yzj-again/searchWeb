/*************************************************************************
    > File Name: Acceptor.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 14时39分02秒
 ************************************************************************/
#ifndef __ACCEPTOR_HH
#define __ACCEPTOR_HH

#include "Socket.hh"
#include "InetAddress.hh"
#include <string>

using std::string;

/**
 * @brief 通过数据成员_listenSock绑定一个监听套接字
 * 通过accept操作获取连接该套接字连接
 * 基于TCP协议
 */
class Acceptor
{
public:
    Acceptor(const string &ip, unsigned short port)
    : _listenSock()
    , _servAddr(ip, port)
    { }

    ~Acceptor() { };

    void ready();
    int accept();
    int fd() const { return _listenSock.fd(); }

private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();
private:
    Socket _listenSock;
    InetAddress _servAddr;
};

#endif