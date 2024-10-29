/**************************************************
 
//File Name: DirScanner.cpp
//Author: HZ_Alfred
//Created Time: 2022-07-31  星期日  11:50:24
 
**************************************************/

#include "../../include/WebProcessor/DirScanner.hh"
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <iostream>

void DirScanner::operator()(const string &dir)
{
    DIR *pdir = opendir(dir.c_str());
    if(nullptr == pdir)
    {
        perror("opendir");
    }
    struct dirent *pd;//与目录相关的结构体
    while((pd = readdir(pdir)))
    {
        if(strcmp(pd->d_name,".") == 0 || strcmp(pd->d_name,"..") == 0)
        {
            continue;
        }
        _files.push_back(string(dir) + string("/") + string(pd->d_name));
    }
    closedir(pdir);
}

vector<string> & DirScanner::GetFilePath()
{
    return _files;
}

void DirScanner::printFiles() const
{
    for(auto &it: _files)
    {
        std::cout<<it<<std::endl;
    }
}
