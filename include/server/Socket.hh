/*************************************************************************
    > File Name: Socket.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 14时38分32秒
 ************************************************************************/
#ifndef __SOCKET_HH
#define __SOCKET_HH

#include "NoCopyable.hh"
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>

/**
 * @brief 维护一个TCP连接的套接字
 * 
 */
class Socket
: NoCopyable
{
public:
    Socket();
    explicit Socket(int fd)
    : _fd(fd) { }

    ~Socket() { close(_fd); };
    int fd() const { return _fd; };
    void shutDownWrite();
private:
    int _fd;
};

#endif
