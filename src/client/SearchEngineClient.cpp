/*************************************************************************
    > File Name: SearchEngineClient.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年08月04日 星期四 15时06分05秒
 ************************************************************************/
#include "../../include/client/SearchEngineClient.hh"
#include "../../include/client/MyJson.hh"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::istringstream;

SearchEngineClient::SearchEngineClient()
{ _isRunning = false; }

SearchEngineClient::~SearchEngineClient()
{
    if(_isRunning) { close(_sockFd); }
}

void SearchEngineClient::start()
{
    _sockFd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_sockFd < 0)
    {
        perror("socket");
        return;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_port);
    serverAddr.sin_addr.s_addr = inet_addr(_ip.c_str());
    socklen_t length = sizeof(serverAddr);

    int ret = ::connect(_sockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (-1 == ret) {
        perror("Connection failed!");
        exit(EXIT_FAILURE);
    } else if (0 == ret) {
        cout << "连接成功！" << endl;
        cout << "######## Search Engine v1.0 ########" << endl;
        cout << "User Guide: " << endl;
        cout << "Keyword Recommander: [1 keyword] eg:[1 hello]" << endl;
        cout << "Webpage Search: [2 sentence] eg:[2 国务院台湾事务办公室30日举行例行新闻发布会]" << endl;
        _isRunning = true;
    }
    run();
}

void SearchEngineClient::run()
{
    fd_set rdset;
   

    while(1)
    {
        //注册事件
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(_sockFd, &rdset);
        
        int nready = select(_sockFd + 1, &rdset, nullptr, nullptr, nullptr);

        if(nready < 0){
            perror("select error");
            exit(EXIT_FAILURE);
        }

        if(FD_ISSET(STDIN_FILENO, &rdset))
        {
            /*
            char buf[4096] = {0};
            memset(buf, 0, sizeof(buf));
            int ret = read(STDIN_FILENO, buf, sizeof(buf));
            */
            
            
            string line;
            getline(cin, line);

            int msgId;
            string query;

            istringstream iss(line);
            iss >> msgId >> query;

            
            if(static_cast<int>(MsgType::KEYRECOMMAND) == msgId)
            { 
                MyJson mJ;
                string sendMsg = mJ.to_json(query, MsgType::KEYRECOMMAND);
                //cout << sendMsg << endl;
                send(_sockFd, sendMsg.data(), sendMsg.size() + 1, 0);
            }

            else if(static_cast<int>(MsgType::WEBPAGESEARCH) == msgId)
            { 
                MyJson mJ;
                string sendMsg = mJ.to_json(query, MsgType::WEBPAGESEARCH);
                send(_sockFd, sendMsg.data(), sendMsg.size() + 1, 0);
            }
        }
        else if(FD_ISSET(_sockFd, &rdset))
        {
            char buff[4096] = {0};
            int ret = recv(_sockFd, buff, sizeof(buff), 0);
            if(0 == ret)
            { 
                cerr << "Server has closed the connection." << endl;
                _isRunning = false;
                break;
            }
            
            MyJson mJ;

            string msg = buff;
            string keyResult;
            vector<vector<pair<string, string>>> webResult;
            MsgType type = mJ.from_json(msg, webResult, keyResult);

            if(MsgType::NOMESSAGE == type){ 
                cout << "未查询到关联词" << endl; 
            }else if(MsgType::KEYRECOMMAND == type){ 
                cout << keyResult << endl; 
            }else if(MsgType::WEBPAGESEARCH == type){
                for(auto & elemVec : webResult)
                {
                    for(auto & elem : elemVec)
                    {
                        cout << elem.first + ": " << elem.second << endl;
                    }
                    cout << endl;
                }
            }
        }
    }
}
