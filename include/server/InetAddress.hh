/*************************************************************************
    > File Name: InetAddress.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 14时38分17秒
 ************************************************************************/
#ifndef __INETADDRESS_HH
#define __INETADDRESS_HH

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <string>

using std::string;

/**
 * @brief 封装与网络地址有关的函数与数据
 * 
 */
class InetAddress
{
public:
    InetAddress(const string & ip, unsigned short port);
    InetAddress(const struct sockaddr_in & addr);
    ~InetAddress() { };

    string ip() const { return inet_ntoa(_servAddr.sin_addr); }
    unsigned short port() const { return _servAddr.sin_port; }

    const struct sockaddr_in *
    getSockAddrPtr() const { return &_servAddr; }
private:
    struct sockaddr_in _servAddr;
};

/*
//将一个32位的网络字节序二进制IPv4地址转换成相应的点分十进制数串,
//由该函数的返回值所指向的字符串驻留在静态内存中,
//这意味着该函数是不可重入的 
char *inet_ntoa(struct in_addr in);

//inet_addr函数转换网络主机地址（如192.168.1.10)为网络字节序二进制值，
//如果参数char *cp无效，函数返回-1(INADDR_NONE),
//这个函数在处理地址为255.255.255.255时也返回-1,
//255.255.255.255是一个有效的地址，不过inet_addr无法处理; 
//返回值为32位的网络字节序二进制 
in_addr_t inet_addr(const char *cp);//ok

*/
#endif