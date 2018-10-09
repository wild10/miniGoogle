#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <string.h>
#include "../common/common.h"
// We are not considering the ñ letter
#define ALPHABET_LENGTH 36
#define NOT_FOUND 0
#define SPLIT_1 1
#define SPLIT_2 2
#define INSIDE 3
#define FOUND 4

using namespace std;

size_t firstLetter = int('0');

int p(char c) {
    
    /*
    A function to return the postion of a letter.
    Starts from 0 to ALPHABET_LENGTH.
    e.g.
    A = 10
    B = 11
     */
    
    if (c >= '0' && c <= '9') {
        return int(c) - firstLetter;
    }
    else {
        return int(c) - firstLetter - 7;
    }
}

char c(int p) {
    if (p >= 0 && p <= 9) {
        return char(p + firstLetter);
    }
    else {
        return char(p + firstLetter + 7);
    }
}

class Tree;

class Parse;

class Node {

public:
    Node ** sons;
    Node * parent;
    bool isWord;
    string str;

public:
    unordered_map <int, WordDoc> directory;

    Node() {
        this->str = "";
        this->sons = new Node * [ALPHABET_LENGTH];
        this->isWord = false;
        this->parent = 0;
        for (int i = 0; i < ALPHABET_LENGTH; i += 1) {
            this->sons[i] = 0;
        }
    }

    Node(int doc, string & str, bool isWord, int & start) {
        this->str = str;
        this->sons = new Node * [ALPHABET_LENGTH];
        this->isWord = isWord;
        this->parent = 0;
        for (int i = 0; i < ALPHABET_LENGTH; i += 1) {
            this->sons[i] = 0;
        }
        directory[doc].pagerank += 1;
    }

    string cut(size_t pos) {
        string rest = str.substr(pos, str.size());
        str = str.substr(0, pos);
        return rest;
    }

    string reverseCut(size_t pos) {
        string rest = str.substr(0, pos);
        str = str.substr(pos, str.size());
        return rest;
    }

    size_t contains(string & otherStr, size_t & i) {
        size_t size = str.size();
        size_t otherSize = otherStr.size();
        for (i = 0; i < size; i += 1) {
            if (str[i] != otherStr[i]) {
                if (i >= otherSize) {
                    return INSIDE;
                } else if (i > 0) {
                    return SPLIT_2;
                } else {
                    return NOT_FOUND;
                }
            }
        }
        if (size == otherSize) {
            return FOUND;
        }
        return SPLIT_1;
    }

    friend class Tree;
    friend class Parse;
};


#endif // NODE_H
