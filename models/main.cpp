#include <iostream>

#include <boost/property_tree/json_parser.hpp>
#include "radix-tree/tree.h"
#include "parse/Parse.h"

using namespace std;

vector<string>* findSimilarWords(Tree t, string word){
    vector<string> *dictionary = new vector<string>;
    t.findOptions(word, dictionary);
    return dictionary;
}

string vectorToJson(vector<string> *list){
    string json_string = "{'words':[";
    for(int i = 0; i < list->size(); i++){
        json_string += "'"+(*list)[i]+"'";
        if(i != list->size()-1)
            json_string += ",";
    }
    json_string += "]}";
    return json_string;
}

string getNearWord(Tree t, vector<vector<string>*> *dictionary, string find_word){
    std::transform(find_word.begin(), find_word.end(),find_word.begin(), ::toupper);
    if(find_word.length() < 2)
        return "VACIO";
    return t.processMostNear(dictionary, find_word);
}

void mainMenu() {
    cout << "  Type one of the following options " << endl;
    cout << "  1. Make a search " << endl;
    cout << "  2. Show a document " << endl;
    cout << "  0. Exit " << endl;
}

void searchMenu(int next, int prev) {
    cout << "=> Type one of the following options " << endl;
    if (next > 0) {
        cout << "1. Next page " << endl;
    }
    if (prev > 0) {
        cout << "2. Prev page " << endl;
    }
    cout << "0. Back " << endl;
}

int main(int argc, char *argv[]) {
    
    int a, b;
    
    cout << "a: " << &a << " b :" << &b << endl;
    
    Tree * t = new Tree();
    t->add("TEST", a, b);
    t->add("TOASTER", a, b);
    t->add("TOASTING", a, b);
    t->add("SLOW", a, b);
    t->add("SLOWLY", a, b);
    t->add("RATA", a, b);
    t->add("RA", a, b);

    cout << t->graphviz() << endl;


/*    Parse *parse = new Parse("../../files/", 0);
    parse->processFile();

    int prev, next, total;
    double time;

    cout << "... Load index done! " << endl;
    cout << "====================================" << endl;
    cout << "   (⌐■_■)  ** ALTAVISTA ** (⌐■_■)" << endl;
    cout << "====================================" << endl;

    while(true) {
        string word;
        string result;
        prev = 0;
        next = 0;
        total = 0;
        time = 0;
        int start = 0;
        cout << "Enter your query: ";
        getline(cin, word);
        vector <Result> results;
        parse->find(word, results, total, next, prev, time, start, 30);

        while (true) {
            for (int i = 0; i < results.size(); i++) {
                cout << "[" << i + 1 << "] " << results[i].title << " | " << results[i].preview << endl;
                if (i > 5) {
                    break;
                }
            }
            if (results.size() > 5) {
                cout << "..." << endl;
            }
            cout << endl << "About: " << total << " results (" << time << " seconds)" << endl << endl;

            cout << "Do you want to open any result [n or result number]? Type 0 to go back to search  ";
            getline(cin, result);
            if (result == "0") {
                break;
            } else if (all_of(result.begin(), result.end(), ::isdigit)) {
                int index = atoi(result.c_str());
                if (index < 1 || index > total) {
                    cout << "**ERROR** Invalid result number " << endl;
                } else {
                    int docIdInt = results[index - 1].docNumber;
                    Document * doc = parse->getDocument(docIdInt);
                    cout << doc->title << endl;
                    cout << parse->getText(docIdInt, doc->start, doc->end) << endl << endl;
                }

            } else {
                cout << "**ERROR** not a valid number" << endl;
            }
            cout << endl;
        }


    }*/

    return 0;
}
