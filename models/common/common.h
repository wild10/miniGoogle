#ifndef COMMON_H
#define COMMON_H

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <iomanip>

using namespace std;

struct Result {
    string docId;
    int docNumber;
    string title;
    string preview;
};

struct Document {
    int idDocument;
    unsigned short int idFile;
    int start;
    int end;
    string title;
};

struct WordDoc {
    unsigned short int pagerank;
    unsigned int start;

    WordDoc() {
        pagerank = 0;
        start = 0;
    }

    WordDoc(int & start) {
        this->start = start;
        pagerank = 1;
    }

    int inc(int n = 1) {
        pagerank += n;
        return pagerank;
    }
};

struct Word {
    unsigned short int idFile;
    int start;
    // int end;
    // string content;
};

string escape_json(const string &s) {
    ostringstream o;
    for (auto c = s.cbegin(); c != s.cend(); c++) {
        if (*c == '\n') {
            o << " ";
        } else if (*c == '"' || *c == '\\' || ('\x00' <= *c && *c <= '\x1f')) {
            o << "\\u" << hex << setw(4) << setfill('0') << (int)*c;
        } else {
            o << *c;
        }
    }
    return o.str();
}

#endif // COMMON_H
