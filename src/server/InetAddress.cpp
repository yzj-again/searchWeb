/*************************************************************************
    > File Name: InetAddress.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 15时40分12秒
 ************************************************************************/
#include "../../include/server/InetAddress.hh"

InetAddress::InetAddress(const string & ip, unsigned short port)
{
    _servAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    _servAddr.sin_port = htons(port); //host to network short
    _servAddr.sin_family = AF_INET;
}

InetAddress::InetAddress(const struct sockaddr_in & addr)
: _servAddr(addr)
{ }
