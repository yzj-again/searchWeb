/*************************************************************************
    > File Name: Acceptor.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 16时27分56秒
 ************************************************************************/

#include "../../include/server/Acceptor.hh"

void Acceptor::ready()
{
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}

void Acceptor::setReuseAddr()
{
    int on = 1;
    int ret = setsockopt(_listenSock.fd()
        , SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if(ret) { perror("setsockopt"); }
} 

void Acceptor::setReusePort()
{
    int on = 1;
    int ret = setsockopt(_listenSock.fd()
        , SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
    if(ret) { perror("setsockopt"); }
}

void Acceptor::bind()
{
    int ret = ::bind(_listenSock.fd()
        , (struct sockaddr *)_servAddr.getSockAddrPtr()
        , sizeof(struct sockaddr));
    if(-1 == ret) { perror("bind"); }
}

void Acceptor::listen()
{
    int ret = ::listen(_listenSock.fd(), 128);
    if(-1 == ret) { perror("listen"); }
} 

int Acceptor::accept()
{
    int connfd = ::accept(_listenSock.fd(), nullptr, nullptr);
    if(-1 == connfd) { perror("accept"); }
    return connfd;
}