#include "../../include/server/WebPage.hh"
#include <iostream>

/**
 * @brief Get the ByteNum UTF8 object
 * 
 * @param byte      开头的字节
 * @return size_t   以byte开头的字符对应的UTF8格式长度
 */
size_t getByteNum_UTF8(const char byte)
{
    int byteNum = 0;
    for(size_t i = 0; i < 6; ++i)
    {
        if(byte & (1 << (7 - i))){
            ++byteNum;
        }else{
            break;
        }
    }
    return byteNum == 0 ? 1 : byteNum;
}

/**
 * @brief Get the Summary Size object
 * 
 * @param content 文章内容
 * @return int    前30个UTF8字符对应的字符串长度
 */
int getSummarySize(const string & content)
{
    int index = 0;
    int cnt = 0;
    while(cnt < 30)
    {
        index += getByteNum_UTF8(content[index]);
        ++cnt;
    }
    return index;
}


WebPage::WebPage(const string & doc)
: _doc(doc)
{
    processDoc();
}
    
//处理格式化文档
void WebPage::processDoc()
{
    int beg = 0, end = 0;
    //std::cout << _doc << std::endl;
    beg = _doc.find("<docid>");
    end = _doc.find("</docid>");
    string id = _doc.substr(beg + 7, end - beg - 7);
    _docId = stoi(id);
    //std::cout << id << std::endl;

    beg = _doc.find("<title>");
    end = _doc.find("</title>");
    _docTitle = _doc.substr(beg + 7, end - beg - 7);
    //std::cout << _docTitle << std::endl;

    beg = _doc.find("<url>");
    end = _doc.find("</url>");
    _docLink = _doc.substr(beg + 5, end - beg - 5);
    //std::cout << _docLink << std::endl;

    beg = _doc.find("<content>");
    end = _doc.find("</content>");
    _docContent = _doc.substr(beg + 9, end - beg - 9);
    
    //要保证截取的字符串符合UTF8格式
    int length = getSummarySize(_docContent);
    _docSummary = _docContent.substr(0, length) + "...";
    //std::cout << _docSummary << std::endl;
}


