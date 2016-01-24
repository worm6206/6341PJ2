// Kevin Yen Jan 24
#include "scanner.h"
#include <sstream>

using namespace std;

Scanner s;
stringstream ss;


void Expr();
void List();
void Check(){
	if(s.Current.Type=="ERROR" || !s.error.empty()){
		cout << "ERROR: " << s.error << endl;
		exit(0);
	}else if (s.Current.Type=="EOF"){
		cout << "ERROR: [syntax error] sentence not finished?" << endl;;
		exit(0);
	}
}

void Start(){
	Check();
	while(s.Current.Type != "EOF"){
		ss.str(string());
		Expr();
		cout << ss.str() << endl;
	}
}

void Expr(){
	Check();
	if(s.Current.Type == "Atom"){
		s.print(ss);
		s.MoveToNext();
	}
	else if(s.Current.Type == "OpenParenthesis"){
		s.MoveToNext();
		if(s.Current.Type == "ClosingParenthesis")
			ss<<"NIL";
		while(s.Current.Type != "ClosingParenthesis"){
			List();
		}
		s.MoveToNext();
	}
	else
		s.error = "[syntax error] not expecting \"" + s.Current.Content + "\"";
}

void List(){
	Check();
	int x = 0;
	while(s.Current.Type == "Atom" || s.Current.Type == "OpenParenthesis"){
		x++;
		ss<<"(";
		Expr();
		ss<<" . ";
	}
	ss << "NIL";
	while(x!=0){
		x--;
		ss<<")";
	}

}

int main(){

	s.Init();
	Start();
}