/*************************************************************************
    > File Name: Socket.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 15时50分44秒
 ************************************************************************/

#include "../../include/server/Socket.hh"

Socket::Socket()
{
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(_fd < 0) { perror("socket"); }
}

void Socket::shutDownWrite()
{
    //关闭写端
    int ret = ::shutdown(_fd, SHUT_WR);
    if(ret) { perror("shutdown"); }
}