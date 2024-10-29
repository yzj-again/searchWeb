/*************************************************************************
    > File Name: main.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 20时24分19秒
 ************************************************************************/
#include "../../include/server/SearchEngineServer.hh"

void test()
{
    SearchEngineServer search("127.0.0.1", 8888, 4, 10);
    search.start();
}

int main()
{
    test();
    return 0;
}