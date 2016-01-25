// Kevin Yen Jan 24
#include "scanner.h"
#include <sstream>
#include <vector>

using namespace std;

struct node{ //
	int index; // current index
	string key; // content of the node
	int back; // back link to it's father/mother/whoever-adpoted-this-node.
	int left; // index number of left child
	int right; // index number of right child
};

Scanner s;
stringstream ss;
vector<node> tree;
int focus,treeindex;
vector<Token> bank;

// returns a token, for constructing the vector.
Token newbank(string Type, string Content){
	Token temp(Type,Content);
	return temp;
}

// returns a node, for constructing the vector.
node anode(int index, string key, int back, int left, int right){
	node temp;
	temp.back = back;
	temp.index = index;
	temp.key = key;
	temp.left = left;
	temp.right = right;
	return temp;
}

// initialize tree for new Expr
void treeinit(){
	tree.clear();
	focus = 0;
	treeindex = 0;
	tree.push_back(anode(0,"",-1,-1,-1));
}

// converting a vector of tokens into a tree, represented by a vector of nodes.
void BankToTree(){
	for (std::vector<Token>::iterator i = bank.begin(); i != bank.end(); ++i)
	{
		if(i->Type=="OpenParenthesis"){
			tree[focus].left = ++treeindex;
			tree.push_back(anode(treeindex,"",focus,-1,-1));
			tree[focus].right = ++treeindex;
			tree.push_back(anode(treeindex,"",focus,-1,-1));
			focus = tree[focus].left;
		}
		else if(i->Type=="dot"){
			focus = tree[tree[focus].back].right;
		}
		else if(i->Type=="Atom" || i->Type=="NIL"){
			tree[focus].key = i->Type;
		}else if(i->Type=="ClosingParenthesis")
			focus = tree[focus].back;
	}
}

// print the tree. The format is (my_index,content,back_index,left_index,right_index)
void printtree(){
	for (std::vector<node>::iterator i = tree.begin(); i != tree.end(); ++i)
	{
		cout << i->index << "," << i->key << "," << i->back << "," << i->left << "," << i->right << endl;
	}
}

// print out Type and Content of tokens. Mostly for debugging.
void printbank(){
	for (std::vector<Token>::iterator i = bank.begin(); i != bank.end(); ++i)
	{
		cout << i->Type << endl;
		cout << i->Content << endl;
	}
	cout << endl;
}

// main stuff. Start(), Expr(), and List().
void Expr();
void List();

// Check() checks for any error messages to show. If error appears, print error and exit the whole program.
void Check(){
	if(s.Current.Type=="ERROR" || !s.error.empty()){
		// returning token is ERROR or the string named error is written by something.
		cout << "ERROR: " << s.error << endl;
		exit(0);
	}else if (s.Current.Type=="EOF"){
		// this happens when file ended but the grammar is not finished.
		cout << "ERROR: [syntax error] sentence not finished?" << endl;;
		exit(0);
	}
}

void Start(){
	Check();
	while(s.Current.Type != "EOF"){
		// instead of simply cout the output, store them at a buff called ss. If there's no error, print out ss. This prevents error message appear in the middle of parsing/scanning.
		ss.str(string());
		// bank and tree initialize for new Expr tree.
		bank.clear();
		treeinit();
		Expr();
		// if the program manages to reach here, means there's no error. Thus print out legal results.
		cout << ss.str() << endl;
		// printbank();// uncomment to see token list thru stdout
		BankToTree();
		// printtree();// uncomment to see tree. (my_index,content,back_index,left_index,right_index)
	}
}

void Expr(){
	Check();
	if(s.Current.Type == "Atom"){
		s.print(ss); // print current token to stringstream
		bank.push_back(newbank(s.Current.Type, s.Current.Content)); // store current token to bank. Doesn't effect output.
		s.MoveToNext();
	}
	else if(s.Current.Type == "OpenParenthesis"){
		s.MoveToNext();
		if(s.Current.Type == "ClosingParenthesis"){
			bank.push_back(newbank("NIL", "NIL")); // store current token to bank. Doesn't effect output.
			ss<<"NIL"; // print to stringstream
		}
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
	int x = 0; // x is for counting how many closing parenthesis needs to be printed according to open parenthesis.
	while(s.Current.Type == "Atom" || s.Current.Type == "OpenParenthesis"){
		x++;
		bank.push_back(newbank("OpenParenthesis", "(")); // store current token to bank. Doesn't effect output.
		ss<<"(";
		Expr();
		bank.push_back(newbank("dot", ".")); // store current token to bank. Doesn't effect output.
		ss<<" . ";
	}
	bank.push_back(newbank("NIL", "NIL")); // store current token to bank. Doesn't effect output.
	ss << "NIL";
	while(x!=0){ // fill up all the forgotten closing parenthesises.
		x--;
		bank.push_back(newbank("ClosingParenthesis", ")")); // store current token to bank. Doesn't effect output.
		ss<<")";
	}
}

int main(){
	s.Init();
	Start();
	return 0;
}