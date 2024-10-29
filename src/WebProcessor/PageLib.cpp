#include "../../include/WebProcessor/PageLib.hh"

#include <stdio.h>
#include <map>
#include <regex>
#include <sstream>
#include <fstream>

using namespace tinyxml2;
using std::istringstream;
using std::map;
using std::pair;
using std::cerr;
using std::cout;
using std::endl;

PageLib::PageLib(Configuration * conf, DirScanner & dirScanner)
: _dirScanner(dirScanner)
, _config(Configuration::getInstance())
{
    map<string, string> filePath = _config->getConfigMap();
    string fileP = filePath["xmldir"]; 
    _dirScanner(fileP);                                                                    

    create();
}

PageLib::~PageLib(){
    
}


vector<string> PageLib::getArticle (){
    return _files;
}

map<int ,pair<int,int>> PageLib::getOffsetLib(){
    return _offsetLib;
}

void PageLib::create(){
    
    vector<string> lib = _dirScanner.GetFilePath();
    size_t id = 0;
    int pos = 0;

    for(size_t idx = 0; idx != lib.size(); ++idx){//遍历所有网页
        XMLDocument doc;
        doc.LoadFile(lib[idx].c_str());
        if(doc.ErrorID()){
            std::cerr<<"LoadFile fail"<<endl;
        }

        XMLElement *itemNode= doc.FirstChildElement("rss")->
            FirstChildElement("channel")->FirstChildElement("item");

        while(itemNode)
        {   
            string text;
            string title = itemNode->FirstChildElement("title")->GetText();       
            string link =  itemNode->FirstChildElement("link")->GetText();
            string content = itemNode->FirstChildElement("description")?
                itemNode->FirstChildElement("description")->GetText(): 
                itemNode->FirstChildElement("content")?
                itemNode->FirstChildElement("content")->GetText():"";

            std::regex reg("<[^>]+>");
            content = regex_replace(content, reg, "");

            std::regex reg2("\n");
            content = regex_replace(content, reg2, "");
            text="<doc><docid>" + std::to_string(++id) +
                "</docid><url>" + link +
                "</url><title>" + title +
                "</title><content>" + content +
                "</content></doc>\n";
            int capacity=text.size();
            _offsetLib[id]=std::pair<int,int>(pos,capacity);
            pos+=capacity;
            _files.emplace_back(text);
            itemNode = itemNode->NextSiblingElement("item");
        }
    }
}

void PageLib::storeArticle(){
    cout << "生成网页库..." << endl;
    //生成网页库   
    map<string,string> m = _config->getConfigMap();
    string storePath = m["webpage"];
    cout << storePath << endl;
    std::ofstream ofs;
    ofs.open(storePath, std::ios::out | std::ios::trunc);

    if(!ofs){
        cerr << "ofs open failed" << endl;
        return ;
    }

    for(auto & ele : _files){
        ofs << ele;
    }

    ofs.close();
}

//生成索引库
void PageLib::storeOffsetLib(){
    cout << "生成偏移库..." << endl;
    map<string,string> m = _config->getConfigMap();
    string storePath = m["newoffset"];
    cout << storePath << endl;
    std::ofstream ofs;
    ofs.open(storePath, std::ios::out | std::ios::trunc);
    if(!ofs){
        cerr<<"ofs open failed"<<endl;
        return ;
    }

    for(size_t i=1; i <_offsetLib.size(); ++i){
        ofs << i <<" "<<_offsetLib[i].first << " "
            << _offsetLib[i].second <<endl;
    }
}


