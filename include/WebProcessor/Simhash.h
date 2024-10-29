#ifndef __SIMHASH_H
#define __SIMHASH_H

#include "../../lib/simhash/Simhasher.hpp"
#include "../../include/WebProcessor/Configuration.hh"
#include <vector>
#include <string>
#include <utility>
#include <iostream>

using std::vector;
using std::string;
using std::pair;
using std::cout;
using std::endl;

const string D1 = getPath("DICT_PATH");
const string H2 = getPath("HMM_PATH");
const string I3 = getPath("IDF_PATH");
const string S4 = getPath("STOP_WORD_PATH");

class Simhash
{
public:
    Simhash()
    : _simhasher(D1, H2, I3, S4) 
    , _topN(20)
    {
        cout << "simhash init!" << endl;
    }
    
    bool isEqual(uint64_t u1, uint64_t u2)
    {
        return _simhasher.isEqual(u1, u2);
    }
    
    uint64_t make(string s)
    {
        uint64_t u = 0;
        _simhasher.make(s, _topN, u);
        return u;
    }
    
    ~Simhash() {}

private:
    simhash::Simhasher _simhasher; 
    size_t _topN;
};

#endif

