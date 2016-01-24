// Kevin Yen 
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


void readFile(string& inputLines){
	string buff;
	while (getline(cin, buff)){
		inputLines+=buff + "\n";
	}
}

string getNextToken(int& index, string inputLines, string& error, vector<string>& atoms, int& sum){ 
	if((inputLines.length())==(unsigned)index){
		return "EOF";
	}
	if (inputLines[index]==32 || inputLines[index]==9 || inputLines[index]==13 || inputLines[index]==10){
		return getNextToken(++index, inputLines, error, atoms, sum);
	}
	else if(inputLines[index]==40){
		index++;
		return "OpenParenthesis";
	}
	else if(inputLines[index]==41){
		index++;
		return "ClosingParenthesis";
	}
	else if((inputLines[index]>=65) && (inputLines[index]<=90)){
		string word = string(1,inputLines[index]);
		int temp = index;
		while( ((inputLines[++temp])>=65 && (inputLines[temp])<=90) || ((inputLines[temp])>=48 && (inputLines[temp])<=57) )
			word+=inputLines[temp];
		index=temp;
		atoms.push_back(word);
		return "LiteralAtom";
	}
	else if((inputLines[index]>=48) && (inputLines[index]<=57)){
		string number = string(1,inputLines[index]);
		bool valid = true;
		int temp = index;
		while(((inputLines[++temp])>=65 && (inputLines[temp])<=90) || ((inputLines[temp])>=48 && (inputLines[temp])<=57) )
			number+=inputLines[temp];
		index = temp;
		for(unsigned int i =0;i<number.length();i++){
			if (number[i] >=65 && number[i] <=90)
				valid = false;
			if(!valid) break;
		}
		if(!valid){
			error = "Invalid token " + number;
			return "ERROR";
		}
		sum+=stoi(number);
		return "NumericAtom";
	}
	else{
		stringstream convert;
		convert << index+1;
		error="unrecognized character \"" + string(1,inputLines[index]) + "\" at " + convert.str();
		return "ERROR";
	}
}

int main( int argc, char *argv[] ){

/*VARIABLES*/
	string error, token, inputLines, outputLines;
	vector<string> atoms;
	int  literalAtom=0, numericAtom=0, openPar=0, closingPar=0, sum=0, index=0;

/*READCIN*/
	readFile(inputLines);

/*TOKEN*/
	while( token = getNextToken(index, inputLines, error, atoms, sum), token!= "EOF" && token!= "ERROR" ){
		if(token == "OpenParenthesis")
			openPar++;
		else if(token == "ClosingParenthesis")
			closingPar++;
		else if(token == "LiteralAtom")
			literalAtom++;
		else if (token == "NumericAtom")
			numericAtom++;
		else{
			error = "uncaught token " + token;
			break;
		}
	}

/*WRITECOUT*/
	if (!error.empty()){
		cout << "ERROR: " << error << endl;
	}
	else{
		cout << "LITERAL ATOMS: ";
		if(literalAtom==0)
			cout << "0" << endl;
		else {
			cout << literalAtom << ", ";
			for (unsigned int i =0; i<=atoms.size()-1; ++i)
			{
				cout << atoms[i];
				if((i+1)!=atoms.size()) 
					cout << ", ";
				else
					cout << endl;
			}
		}
		cout << "NUMERIC ATOMS: ";
		if(numericAtom==0)
			cout << "0" << endl;
		else
			cout << numericAtom << ", " << sum << endl;
		cout << "OPEN PARENTHESES: " << openPar << endl;
		cout  << "CLOSING PARENTHESES: " << closingPar << endl;
	}
}
