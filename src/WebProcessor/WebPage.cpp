#include "../../include/WebProcessor/WebPage.hh"

Simhash WebPage::_sim = Simhash();

using std::cin;
WebPage::WebPage(const string & doc, Configuration & config, WordSegmentation & jieba)
: _doc(doc)
, _config(config)
, _jieba(jieba)
{
    processDoc();
    calcTopK();
}

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
    //处理格式化文档
void WebPage::processDoc()
{
    size_t beg = 0, end = 0;
    beg = _doc.find("<docid>");
    end = _doc.find("</docid>");
    string docId = _doc.substr(beg + 7, end - beg - 7);
    
    _docId = atoi(docId.c_str());

    beg = _doc.find("<title>");
    end = _doc.find("</title>");
    _docTitle = _doc.substr(beg + 7, end - beg - 7);
    
    beg = _doc.find("<url>");
    end = _doc.find("</url>");
    _docLink = _doc.substr(beg + 5, end - beg - 5);
    
    beg = _doc.find("<content>");
    if(beg != string::npos)
    {
        end = _doc.find("</content>");
        _docContent = _doc.substr(beg + 9, end - beg - 9);

        storeWordsMap(_docTitle);//统计标题的词频
        storeWordsMap(_docContent);//统计内容的词频
        int length = getSummarySize(_docContent);
        _docSummary = _docContent.substr(0, length) + "...";
    }else{
        return;
    }
    makeSimValue();
}

void WebPage::storeWordsMap(const string & s)
{   
    // 使用分词工具
    vector<string> vec = _jieba.operator()(s);

    const set<string> & tempStopWordsList = _config.getCnStopWordList();

    //统计文章词频  
    for (auto &e : vec)
    {
        //丢弃停用词
        if(tempStopWordsList.find(e) != tempStopWordsList.end())
            continue;
        _wordsMap[e]++;//词频数加1
    }
    // 停词库中不包含空格字符，手动删除 
    auto it = _wordsMap.find(" ");
    if(it != _wordsMap.end())
    {
        _wordsMap.erase(it);
    }
}

struct WebPage::comp
{
    bool operator()(const pair<string, int> &lhs, const pair<string, int> &rhs)
    {
        return lhs.second > rhs.second;//形成小顶堆
    }
};

void WebPage::calcTopK()
{
    priority_queue<pair<string, int>, vector<pair<string, int>>, WebPage::comp> top20;
    
    for(auto & e: _wordsMap)
    {
        top20.push(e);
        //逐渐淘汰词频小的词
        if(top20.size() > 20)
        {
            top20.pop();
        }
    }
    //最终 top20.size() <= 20
    _topWords.resize(top20.size());
    //it指向vector末尾，将优先级队列倒序存入
    auto it = _topWords.end();
    while (!top20.empty())
        {
            //topK vector中越靠前的词，词频越大
            *(--it) = top20.top().first;
            top20.pop();
        }
}

void WebPage::makeSimValue(){
    if(_docContent.size())
    {
        _simValue = _sim.make(_docContent);
    }else{
        _simValue = _sim.make(_docTitle);
    }
}


bool operator==(const WebPage & lhs, const WebPage & rhs){
    return WebPage::_sim.isEqual(lhs._simValue, rhs._simValue);
}



