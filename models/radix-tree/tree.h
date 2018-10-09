#ifndef TREE_H
#define TREE_H

#define MAX_SIZE_OPTIONS 10
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#include <algorithm>
#include <climits>
#include "node.h"


class Coincidence {
private:
    vector<Word *> all;
    string word;
};


class Tree {
private:
    Node * root;
    bool shouldPrint;
public:
    Tree() {
        // string str = "";
        // root = new Node(str, false);
        root = 0;
        shouldPrint = false;
    }

    void printSons (Node * & node) {
        cout << "*** node: " << node << " ***" << endl;
        for (int i = 0; i < ALPHABET_LENGTH; i++) {
            if (node->sons[i]) {
                cout << "i: " << c(i) << " = " << node->sons[i] << endl;
            }
        }
    }

    void printSons() {
        // printSons(root->sons[p('H')]);
        printSons(root);
    }

    void loadData(vector<vector<string>*>* dictionary, string location){
        string line, dataBuffer;
        ifstream file (location);
        int max = 0;
        if (file.is_open()){
            while ( getline (file,line) ){
                size_t len = line.length();
                while(len >= dictionary->size())
                    dictionary->push_back(new vector<string>);
                (*dictionary)[len]->push_back(line);
                if (len > max)
                    max = len;
            }
            file.close();
        }
    }

    size_t levenshtein_distance(string s1, string s2){
        unsigned int s1len, s2len, x, y, lastdiag, olddiag;
        s1len = s1.length();
        s2len = s2.length();
        unsigned int column[s1len+1];

        for (y = 1; y <= s1len; y++)
            column[y] = y;

        for (x = 1; x <= s2len; x++) {
            column[0] = x;
            for (y = 1, lastdiag = x-1; y <= s1len; y++) {
                olddiag = column[y];
                column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));
                lastdiag = olddiag;
            }
        }
        return column[s1len];
    }

    string processMostNear(vector<vector<string>*> *dictionary, string find_word){
        //cout << "-0" << endl;


        //cout << "-1" << endl;
        size_t distance, min = INT_MAX;
        int minPos;

        size_t len = find_word.length();

        for(int i = 0; i < (*dictionary)[len]->size(); i++){
            distance = levenshtein_distance(find_word, (*(*dictionary)[len])[i]);
            if(min > distance){
                min = distance;
                minPos = i;
                //cout << (*dictionary)[minPos] << " " << distance << endl;
            }
        }
        return (*(*dictionary)[len])[minPos] ;
    }

    void getWords(Node * & node, vector<string> *dictionary){
        if(dictionary->size() < MAX_SIZE_OPTIONS){

            if(node->isWord)
                dictionary->push_back(node->str);

            vector<string> *temp = new vector<string>();

            for (size_t i = 0; i < ALPHABET_LENGTH; i++)
                if (node->sons[i])
                    getWords(node->sons[i], temp);

            int toFill = MAX_SIZE_OPTIONS - dictionary->size();
            int realTemp = temp->size() > MAX_SIZE_OPTIONS ? toFill : (temp->size() > toFill ? toFill : temp->size());

            for (size_t i = 0; i < realTemp; i++)
                dictionary->push_back(node->str+(*temp)[i]);
        }
    }

    void findOptions(string str, vector<string> * & dictionary){
        std::transform(str.begin(), str.end(),str.begin(), ::toupper);
        size_t position = 0;
        Node * node = root;
        Node * next;
        string prevStr = str;
        size_t absPosition = 0;
        while (node) {
            node->contains(str, position);
            absPosition += position;
            str = str.substr(position);
            next = node->sons[p(str[0])];
            if (str.size() && next) {
                node = next;
            } else {
                if(node){
                    string initWord = prevStr.substr(0, absPosition-position);
                    getWords(node, dictionary);
                    for (int i = 0; i < dictionary->size(); i++) {
                        (*dictionary)[i] = initWord + (*dictionary)[i];
                    }
                }
                break;
            }
        }
    }

    bool find(string str, Node * & node) {
        size_t position = 0;
        node = root;
        Node * next;
        size_t result;
        string prevStr = str;

        while (node) {
            result = node->contains(str, position);
            prevStr = str;
            str = str.substr(position);
            next = node->sons[p(str[0])];
            if (str.size() && next) {
                node = next;
            } else {
                break;
            }
        }
        // cout << "str size " << prevStr << endl;
        // cout << "node isw " << node->str << endl;
        if ((prevStr == node->str) && node->isWord) {
            return true;
        }
        return false;
    }

    size_t find(
            string & str,
            Node * & node,
            Node * & parent,
            size_t & position
        ) {
        position = 0;
        parent = 0;
        node = root;
        size_t result;
        while (node) {
            result = node->contains(str, position);
            if (shouldPrint) {
                cout << "str: " << str << endl;
                cout << "result: " << result << endl;
                cout << "pos: " << position << endl;
            }
            if (result == SPLIT_1) {
                str = str.substr(position);
                Node * next = node->sons[p(str[0])];

                if (shouldPrint) {
                    printSons(node);
                    cout << "=> node: " << node->str << endl;
                    cout << "next: " << next << " ";
                }

                if (next) {
                    if (shouldPrint) {
                        cout << "[" << next->str << "]";
                    }
                    parent = node;
                    node = next;
                    if (shouldPrint) cout << endl;
                } else {
                    if (shouldPrint) cout << endl;
                    break;
                }
            } else if (result == SPLIT_2) {
                break;
            } else if ( ! result ) {
                break;
            } else if ( result == 3) {
                break;
            } else if ( result == 4 ) {
                break;
            }
        }
        return result;
    }

    void graphviz(Node * & node, string & tree) {
        if ( ! node) {
            return;
        }
        string name;
        char ss[100];
        sprintf(ss, "\"%p\"", node);
        name = ss;
        if (node->parent) {
            string parentStr = "\"\"";

            if (node->parent->str != "") {
                char sp[100];
                sprintf(sp, "\"%p\"", node->parent);
                parentStr = sp;
            }

            tree += parentStr + " -> " + name + "\n";
        }
        if (node->isWord) {
            tree += name + " [" + "color = cyan style = filled label = " + node->str + "]\n";
        } else if(node->str != "") {
            tree += name + " [" + "label = " + node->str + "]\n";
        }

        for (size_t i = 0; i < ALPHABET_LENGTH; i += 1) {
            graphviz(node->sons[i], tree);
        }
    }

    string graphviz(){
        string str = "digraph G {\n";
        string tree = "";
        graphviz(root, tree);
        str += tree + "}";
        return str;
    }


    void print (Node * & node) {
        if ( ! node) {
            return;
        }
        cout << '"' << node->str << '"' << "(" << node->isWord << ")";

        if (node->parent) {
            cout << " >- (" << node->parent->str << ")";
        } else {
            cout <<  " >- root";
        }
        cout << endl;

        for (size_t i = 0; i < ALPHABET_LENGTH; i += 1) {
            print(node->sons[i]);
        }
    }

    void print () {
        print(root);
    }

    void add(string str, int & word, int & start) {
        
        // string str;

        if ( ! root) {
            root = new Node(word, str, true, start);
            return;
        }
        Node * node;
        Node * parent;
        size_t position;

        size_t kase = find(str, node, parent, position);
        if (shouldPrint) {
            cout << "+++: " << str << endl;
            cout << "kase: " << kase << endl;
            cout << "node: " << node->str << " - " << node << endl;
            cout << "parent: " << parent << endl;
        }
        if (kase == NOT_FOUND) {
            createEmptyRoot(word, str, start);
        } else if (kase == SPLIT_2) {
            splitNode(word, node, str, position, start);
        } else if (kase == SPLIT_1) {
            // cout << "split 1" << endl;
            createNode(word, node, str, start);
        } else if (kase == INSIDE) {
            createNode(word, node, str, position, start);
        } else if (kase == FOUND) {
            node->isWord = true;
            node->directory[word].pagerank += 1;
            if (node->directory[word].start == 0) {
                node->directory[word].start = start;
            }
        }

    }

private:
    void createEmptyRoot(int & word, string & str, int & start) {
        if (root->str != "") {
            Node * tmp = root;
            root = new Node();
            root->sons[p(tmp->str[0])] = tmp;
            tmp->parent = root;
        }

        Node * newNode = new Node(word, str, true, start);
        root->sons[p(str[0])] = newNode;
        newNode->parent = root;
    }
    void createNode(
            int & word,
            Node * & node,
            string & str,
            size_t position,
            int & start
        ) {
        string res = node->reverseCut(position);
        Node * newParent = new Node(word, res, true, start);
        Node * grandparent = node->parent;
        if (node == root) {
            root = newParent;
        } else {
            grandparent->sons[p(node->str[0])] = 0;
            grandparent->sons[p(newParent->str[0])] = newParent;
        }
        newParent->parent = grandparent;
        newParent->sons[p(node->str[0])] = node;
        node->parent = newParent;
    }

    void createNode(int & word, Node * & node, string & str, int & start) {
        Node * newNode = new Node(word, str, true, start);
        node->sons[p(str[0])] = newNode;
        newNode->parent = node;
    }

    void splitNode(
            int & word,
            Node * & node,
            string & str,
            size_t position,
            int & start
        ) {
        if (shouldPrint) cout << "splitting: " << (node == root) << endl;
        if (shouldPrint) cout << "node: " << node->parent->str << endl;
        char nodeIndex = node->str[0];
        string res = node->reverseCut(position);
        Node * newParent = new Node(word, res, false, start);
        Node * parent = node->parent;
        if (node == root) {
            root = newParent;
        } else {
            parent->sons[p(nodeIndex)] = 0;
            parent->sons[p(newParent->str[0])] = newParent;
        }
        newParent->parent = node->parent;

        if (shouldPrint) {
            cout << "node->parent: " << node->parent->str << endl;
            cout << "newParent->parent: " << newParent->parent->str << endl;
        }
        newParent->sons[p(node->str[0])] = node;
        node->parent = newParent;


        string substr = str.substr(position, str.size());
        Node * second = new Node(word, substr, true, start);
        newParent->sons[p(substr[0])] = second;
        second->parent = newParent;

        // node->isWord = false;
    }

};

#endif // TREE_H
