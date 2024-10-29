#ifndef __WEBPAGE_HH
#define __WEBPAGE_HH

#include <string>

using std::string;

class WebPage
{
public:
    WebPage(const string & doc);
    
    string getContent() { return _docContent; }
    string getTitle()   { return _docTitle; }
    string getSummary() { return _docSummary; }
    string getLink()    { return _docLink; }
    
private:
    //处理格式化文档，得到相应内容
    void processDoc();

private:
    int _docId;
    string _doc;        //整篇<doc>文档
    string _docTitle;   //文档标题
    string _docLink;    //文档链接
    string _docSummary; //文档摘要
    string _docContent; //文档内容
};

#endif

