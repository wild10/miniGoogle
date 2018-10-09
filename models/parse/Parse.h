#pragma once
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/foreach.hpp>
#include <boost/bimap.hpp>
#include <time.h>
#include "../common/common.h"

using namespace std;

struct cmp
{
    template<class T>
    bool operator()(T const &a, T const &b) const { return a > b; }
};

class Parse
{
public:
	Tree * t;
private:
	string pathDocuments;
	int countFiles;
	map<int, string> files;
	map<int, Document*> documents;
    size_t pos;
    string word;
    map<string *, WordDoc *> pagerank;
	set<string> stopWords;
    int pageSize;

	//Variables constantes
	const string startDoc= "<doc";
	const string endDoc = "</doc>";
	const string url = "http://";
	const string delimiterId = "id";
	const string delimiterTitle = "title";
	const string delimiterEndDoc = "ENDOFARTICLE";

	//ofstream outputFile;

	void initStopWords()
	{
		ifstream stopFile;
		stopFile.open("parse/stopwords");
		string sw;
		while( stopFile >> sw)
		{
			stopWords.insert(sw);

		}
        auto it = stopWords.begin();
        if (it == stopWords.end()) {
            cout << "WARNING: Stopwords are not being loaded" << endl;
        }
	}

	string removeCharacter(string word)
	{
		string newWord = "";
		unsigned char charAt;
		int c;
		for (int i = 0 ; i < word.length() ; i ++)
		{
			charAt = word[i];
			c = (int)charAt;
			if((c >= 48 && c <= 57) || (c >= 65 && c <= 90)
				|| (c >= 97 && c <= 122))
				newWord += toupper(c);
			else if( c == 209 || c == 241)
				newWord += "N"; //ñ
			else if( (c >= 192 && c <= 194) || c == 196 || (c >= 224 && c <= 226) || c == 228)
				newWord += "A"; //á
			else if( (c >= 200 && c <= 203) || (c >= 232 && c <= 235) )
				newWord += "E"; //É é
			else if((c >= 204 && c <= 207) || (c >= 236 && c <= 239) )
				newWord += "I"; //í
			else if((c >= 210 && c <= 211) || c == 214 || c == 220 || (c >= 242 && c <= 244) || c == 246 || c == 252)
				newWord += "O"; //ó
			else if((c >= 217 && c <= 219) || (c >= 249 && c <= 250) )
				newWord += "U"; //ú
			 /*else
			 {
			 	cout << word << " - " << newWord << endl;
			 	cout << word[i] << "- ascii: " << c << endl;
			 	break;
			 }*/

		}
		//cout << word << "-"<< newWord << endl << endl;
		return newWord;
	};


	string getInformation(string delimiter, string line)
	{
		size_t pos = line.find(delimiter) + delimiter.length() + 2;
		line.erase(0, pos);
		pos = line.find("\"");
		return line.substr(0, pos);
	};

	void readFile(int idFile, string nameFile)
	{
        int start, idDocument;
		ifstream inputFile;
		inputFile.open(pathDocuments + nameFile);
		if (inputFile)
		{
			//outputFile  << pathDocuments << nameFile << endl;
			cout << "File Found " << nameFile << endl;
			Document *tempDoc = NULL;
			for (string line; getline(inputFile, line); )
			{
				//Get information doc
                start = (int)inputFile.tellg() - line.length() - 1;
				if (line.find(startDoc) != string::npos)
				{
					//Init Document
					tempDoc = new Document();
					idDocument = stoi(getInformation(delimiterId, line));
					tempDoc->title = getInformation(delimiterTitle, line);
                    tempDoc->idDocument = idDocument;
					tempDoc->start = start + line.length(); // +1
					tempDoc->idFile = idFile;
					// cout << "Titulo"<< tempDoc->title << endl;
					// cout << "Inicio doc "<< tempDoc->start << endl;

				}
				else if (line.find(endDoc) != string::npos)
				{
					//Add document
					tempDoc->end = start - delimiterEndDoc.length() - 2; //-1
					documents.insert(make_pair(tempDoc->idDocument, tempDoc));
					// cout << "- "<< tempDoc->end << endl;
				}
				else
				{

					string word, temp;
					istringstream readLine(line);
					int iniLine = (int)inputFile.tellg() - line.length() - 1;
					int startchar = 0;
					int countChar = iniLine;
					set<string>::iterator a;
					for(int i = 0 ; i <line.length(); i++)
					{
						countChar++;
						if((line.at(i) == ' '  && word.length() > 0))
						{
							temp = removeCharacter(word);
							a = stopWords.find(temp);
							if(a == stopWords.end() && temp.length() > 1)
							{
								startchar = countChar - 1 - word.length();
								t->add(temp, idDocument, startchar);
								iniLine += countChar;
							}
							word = "";
						}
						else
						{
							word += line.at(i);

						}
					}
					if(word.length() > 0)
					{
						temp = removeCharacter(word);
						a = stopWords.find(temp);
						if(a == stopWords.end() && temp.length() > 1)
						{
							startchar = countChar - 1 - word.length();
							t->add(temp, idDocument, startchar);
						}
						word = "";
					}
				}
			}
		}
		else
		{
			cout << "File not Found" << endl;
		}
	};

	vector<string> obtenerDirectorio()
	{
		vector<string> g1;
		//g1.push_back("prueba");
		// g1.push_back("spanishText_10000_15000");
        //g1.push_back("spanishText_15000_20000");
		// g1.push_back("spanishText_20000_25000");
		// g1.push_back("spanishText_25000_30000");
		// g1.push_back("spanishText_40000_45000");
		// g1.push_back("spanishText_45000_50000");
		// g1.push_back("spanishText_70000_75000");
		// g1.push_back("spanishText_90000_95000");
		// g1.push_back("spanishText_110000_115000");
		// g1.push_back("spanishText_120000_125000");
		// g1.push_back("spanishText_180000_185000");
		// g1.push_back("spanishText_185000_190000");
		// g1.push_back("spanishText_200000_205000");
		// g1.push_back("spanishText_205000_210000");
		// g1.push_back("spanishText_210000_215000");
		// g1.push_back("spanishText_225000_230000");
		// g1.push_back("spanishText_230000_235000");
		// g1.push_back("spanishText_260000_265000");
		// g1.push_back("spanishText_265000_270000");
		// g1.push_back("spanishText_270000_275000");
		// g1.push_back("spanishText_270000_275000");
		// g1.push_back("spanishText_285000_290000");
		// g1.push_back("spanishText_305000_310000");
		// g1.push_back("spanishText_310000_315000");
		// g1.push_back("spanishText_315000_320000");
		// g1.push_back("spanishText_320000_325000");
		// g1.push_back("spanishText_325000_330000");
		// g1.push_back("spanishText_330000_335000");
		// g1.push_back("spanishText_335000_340000");
		// g1.push_back("spanishText_340000_345000");
		// g1.push_back("spanishText_345000_350000");
		// g1.push_back("spanishText_350000_355000");
		// g1.push_back("spanishText_355000_360000");
		// g1.push_back("spanishText_360000_365000");
		// g1.push_back("spanishText_365000_370000");
		// g1.push_back("spanishText_370000_375000");
		// g1.push_back("spanishText_375000_380000");
		// g1.push_back("spanishText_380000_385000");
		// g1.push_back("spanishText_385000_390000");
		// g1.push_back("spanishText_390000_395000");
		// g1.push_back("spanishText_395000_400000");
		// g1.push_back("spanishText_400000_405000");
		// g1.push_back("spanishText_405000_410000");
		// g1.push_back("spanishText_410000_415000");
		// g1.push_back("spanishText_415000_420000");
		// g1.push_back("spanishText_420000_425000");
		// g1.push_back("spanishText_425000_430000");
		// g1.push_back("spanishText_430000_435000");
		// g1.push_back("spanishText_435000_440000");
		// g1.push_back("spanishText_440000_445000");
		// g1.push_back("spanishText_445000_450000");
		// g1.push_back("spanishText_450000_455000");
		// g1.push_back("spanishText_455000_460000");
		// g1.push_back("spanishText_460000_465000");
		// g1.push_back("spanishText_465000_470000");
	    g1.push_back("spanishText_470000_475000");
		g1.push_back("spanishText_475000_480000");
		g1.push_back("spanishText_480000_485000");
		return g1;
	};

public:
    string findJson(string word, int start = 0){
        string jsonResults = "[ ";
        int prev, next, total;
        double time;
        vector <Result> results;
        find(word, results, total, next, prev, time, start);
        int resultsSize = results.size();
        string response = "{ ";
        for (auto it = results.begin(); it != results.end(); it++) {
            jsonResults += " {\"docid\": " + it->docId + ",";
            jsonResults += "\"title\": \"" + it->title + "\",";
            jsonResults += "\"preview\": \"" + it->preview + "\"},";
        }

        jsonResults.pop_back();
        jsonResults += "]";
        response += "\"results\": " + jsonResults + ",";
        response += "\"prev\": " + to_string(prev) + ",";
        response += "\"next\": " + to_string(next) + ",";
        response += "\"total\": " + to_string(total) + ",";
        response += "\"time\": " + to_string(time);
        response += "}";

        return response;
    }

    bool find(
            string word,
            vector <Result> & results,
            int & total,
            int & next,
            int & prev,
            double & time,
            int start = 0,
            int previewSize = 200
        ) {
        vector<string> words;
        string response = "{ ";
        boost::split(words, word, boost::is_any_of(" "));
        unordered_map <int, unsigned short int> directories;
        unordered_map <int, unsigned short int> pageranks;
        unordered_map <int, unsigned short int> intersections;
        unordered_map <int, vector<int> > positions;

        clock_t tStart = clock();

        int size = words.size();

        if (start < 0) {
            start = 0;
        }

        for (auto it = words.begin(); it != words.end(); it++) {
            Node * node;
            bool found = t->find(removeCharacter(*it), node);
            if (found) {
                for (auto it = node->directory.begin(); it != node->directory.end(); it++) {
                    // cout << "idDocument: " << it->first << endl;
                    int idDocument = it->first;
                    directories[idDocument] += 1;
                    pageranks[idDocument] += it->second.pagerank;
                    positions[idDocument].push_back(it->second.start);
                }
            }
        }

        for (auto it = directories.begin(); it != directories.end(); it++) {
            int idDocument = it->first;
            if (it->second == size) {
                intersections[idDocument] += pageranks[idDocument];
            }
        }
        time = (double)(clock() - tStart) / CLOCKS_PER_SEC;

        vector <pair <unsigned short int, int> > preResults;

        for (auto it = intersections.begin(); it != intersections.end(); it++) {
            preResults.push_back(
                *new pair<unsigned short int, int> (it->second, it->first));
        }

        total = preResults.size();
        prev = (start - pageSize) >= 0 ? (start - pageSize) : -1;
        next = (start + pageSize < total) ? (start + pageSize) : -1;
        sort(preResults.begin(), preResults.end(), cmp());

        if (pageSize > 0) {
            for (int i = start; i < start + pageSize && i < total; i++) {
                pair <unsigned short int, int> * it = &(preResults[i]);
                Result r;
                r.docId = to_string(it->second);
                r.title = documents[it->second]->title;
                r.docNumber = it->second;
                auto range = positions.equal_range(it->second);
                vector<int> pos = range.first->second;
                if (pos.size()) {
                    r.preview = getText(it->second, pos[0], pos[0] + previewSize, "...");
                }
                results.push_back(r);
            }
        } else {
            for (int i = 0; i < total; i++) {
                pair <unsigned short int, int> * it = &(preResults[i]);
                Result r;
                r.docId = to_string(it->second);
                r.title = documents[it->second]->title;
                r.docNumber = it->second;
                auto range = positions.equal_range(it->second);
                vector<int> pos = range.first->second;
                if (pos.size()) {
                    r.preview = getText(it->second, pos[0], pos[0] + previewSize, "...");
                }
                results.push_back(r);
            }
        }


        return results.size() > 0;
    };

    // string findSimilarWords(string word) {
    //     vector <string> * results = new vector <string>();
    //     t->findOptions(word, results);
    //     cout << "results: " << results->size() << endl;
    // }

    string getDocumentContent(int idDocument) {
        Document * doc = documents[idDocument];
        string response = "{\"content\": \"" + getText(idDocument, doc->start, doc->end) + "\", \"title\": \"" + doc->title + "\" }";
        return response;
    }

    Document * getDocument(int idDocument) {
        return documents[idDocument];
    }

    string getText(int idDocument, int start, int end, string endText = "")
    {
        ifstream ifs;
        Document * doc = documents[idDocument];
        if ( ! doc) {
            return "";
        }
        string s;
        string fileName = files[doc->idFile];

        if (fileName != "") {
            ifstream inputFile;
            inputFile.open(pathDocuments + fileName);
            inputFile.seekg(start);
            s.resize(end - start);
            inputFile.read(&s[0], end - start);
        }

        return escape_json(s + endText);
    };

	Parse(string pathDirectory, int pageSize = 10)
	{
		this->pos = 0;
        this->pathDocuments = pathDirectory;
        this->t = new Tree();
        this->pageSize = pageSize;
		initStopWords();
	};

	void processFile()
	{

		//outputFile.open("output.txt");
		vector<string> listado = obtenerDirectorio();
		for (int i = 0; i <listado.size(); i++)
		{
			countFiles++;
			files.insert(make_pair(countFiles, listado.at(i)));
			readFile(countFiles, listado.at(i));
		}
		//outputFile.close();
	};

	~Parse();
};
