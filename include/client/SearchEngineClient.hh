/*************************************************************************
    > File Name: SearchEngineClient.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年08月04日 星期四 15时05分43秒
 ************************************************************************/
#ifndef __SEARCHENGINECLIENT_HH
#define __SEARCHENGINECLIENT_HH

#include <string>
using std::string;

class SearchEngineClient
{
public:
    SearchEngineClient();
    ~SearchEngineClient();
    void start();
private:
    void run();
    void keyWordQuery(const string & query);
    void webPageQuery(const string & query);
    void showQueryResult(const string & result);

private:
    const string _ip = "127.0.0.1";
    const unsigned short _port = 8888;
    int _sockFd;
    bool _isRunning;
};

#endif