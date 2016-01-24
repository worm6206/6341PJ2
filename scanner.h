// Kevin Yen Jan 24
#include <string>
#include <iostream>
#include <sstream>

#ifndef SCANNER_H
#define SCANNER_H

using namespace std;

// Count stuff
struct Counter{
	int LiteralAtom, numericAtom, openPar, closingPar, sum, index;
	Counter() : LiteralAtom(0), numericAtom(0), openPar(0), closingPar(0), sum(0), index(0) {}
};

struct Token{
	string Type, Content;
	Token(string a, string b){
		Type = a;
		Content = b;
	}
	Token(string a){
		Type = Content = a;
	}
	Token(){}
};

// Scan stuff return token
struct Scanner{
	int Index;
	string InputLine, error;
	Token Current;
	void Init();// read file
	Token GetCurrent();
	void MoveToNext();
	Token getNextToken();
	void print(stringstream& ss);
};

#endif