
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <map>

using namespace std;

enum identType {IDENT= 0, KEYWORDS, OP};
map<string, identType> ident_type;
void initmap() {
	ident_type["begin"] = KEYWORDS;
    ident_type["forward"] = KEYWORDS;
    ident_type["do"] = KEYWORDS;
    ident_type["else"] = KEYWORDS;
    ident_type["end"] = KEYWORDS;
    ident_type["for"] = KEYWORDS;
    ident_type["function"] = KEYWORDS;
    ident_type["if"] = KEYWORDS;
    ident_type["array"] = KEYWORDS;
    ident_type["of"] = KEYWORDS;
    ident_type["procedure"] = KEYWORDS;
    ident_type["program"] = KEYWORDS;
    ident_type["record"] = KEYWORDS;
    ident_type["then"] = KEYWORDS;
    ident_type["to"] = KEYWORDS;
    ident_type["type"] = KEYWORDS;
    ident_type["var"] = KEYWORDS;
    ident_type["while"] = KEYWORDS;
    ident_type["break"] = KEYWORDS;
    ident_type["continue"] = KEYWORDS;
    ident_type["downto"] = KEYWORDS;
    ident_type["exit"] = KEYWORDS;
    ident_type["repeat"] = KEYWORDS;
    ident_type["until"] = KEYWORDS;

	ident_type["and"] = OP;
    ident_type["div"]= OP;
    ident_type["mod"] = OP;
    ident_type["not"] = OP;
    ident_type["or"] = OP;
    ident_type["xor"] = OP;
	}

ifstream fin("input.txt");
ofstream fout("output.txt");
char currentSymbol;
int line = 1;
int column = 0;
int columnError = 0;
char op_second_char[] = {'/','+','*',' :','-','>', '^', '<', '@', '.', '='};
char sep[] = {';' ,',','(','[',')',']',};
char op[] = {'>','<','/','+','*',':','-','^','=','@',';',',','(','[',')',']','.'};

class Token
{
protected:
    int l;
    int c;
    string tok;
    string leks;
public:
    Token (int L, int C, string TOK, string LEKS)
    {
        l = L;
        c = C;
        tok = TOK;
        leks = LEKS;
    }
    virtual void print(){
        fout << l << "\t" << c << "\t" << tok << "\t" << leks << endl;
    }
    ~Token(){}
};

void next_char()
{
    fin >> currentSymbol;
    if (fin.eof()) currentSymbol = '?';
    if (currentSymbol == '\n')
    {
        line++;
        columnError = column;
        column = 0;
    } else
    if (currentSymbol == '\t')
    {
        column = column/4 * 4 +4;
    }
    else column++;
}

bool in_masiv(char s, char *mas, int len){
	 for(int i = 0; i < len; i++){
		if (s == mas[i]) return true;
	 }
	 return false;
 }

Token *get_token ()
{	
	string leksema = "";
    string tokenType = "";
    int lineCur=line;
    int columnCur=column;

	if (fin.eof()) return 0;
	if ((currentSymbol != ' ' ) && (currentSymbol !='\n') && (currentSymbol !='\t')){
		if (isalpha(currentSymbol) || currentSymbol == '_') {
			tokenType = "ident";	
			leksema += currentSymbol;
			next_char();
			while(isalpha(currentSymbol) || isdigit(currentSymbol) || currentSymbol == '_'){
				leksema += currentSymbol;
				next_char();	
			}
		 }
	else if  (isdigit(currentSymbol)){
		tokenType ="integer";
		leksema += currentSymbol;
		next_char();
		while(isdigit(currentSymbol) || currentSymbol == '.'){
			if (currentSymbol == '.') {
				tokenType ="real"; 
			}
			leksema += currentSymbol;
			next_char();
		}
	}
	else if (in_masiv(currentSymbol, sep, sizeof(sep)/sizeof(sep[0]))) {
		tokenType ="sep"; 
		leksema += currentSymbol;
		next_char();
	}
	else if (in_masiv(currentSymbol, op, sizeof(op)/sizeof(op[0]))) {
		if (currentSymbol == ':') {
			leksema += currentSymbol;
			next_char();
			if (currentSymbol == '=') {
				leksema += currentSymbol;
				next_char();
				tokenType ="op"; 
			}
			else {tokenType ="sep"; }
		}
		if (in_masiv(currentSymbol, op_second_char, sizeof(op_second_char)/sizeof(op_second_char[0]))) {
			leksema += currentSymbol;
			next_char();
			tokenType ="op"; 
			if (currentSymbol == '=') {
				leksema += currentSymbol;
				next_char();
			}
		}
	}
	 else if (currentSymbol == '$') {
			leksema+=currentSymbol;
			next_char();
			while(isdigit(currentSymbol) || (currentSymbol>='a' && currentSymbol<='f') || (currentSymbol>='A' && currentSymbol<='F'))
			{	
				leksema+=currentSymbol;
				next_char();
			}
			tokenType = "hex";	
	 }
	if (tokenType == "ident" && (ident_type[leksema] == KEYWORDS)) tokenType = "keyword";
    if (tokenType == "ident" && (ident_type[leksema] == OP)) tokenType = "op";
	Token *token = new Token(lineCur, columnCur, tokenType, leksema);
        return token;
	  
	 }
	else
    {
        next_char();
        return get_token();
    }
}

int main()
{	
	   initmap();
    fin >> noskipws;
	 next_char();
	Token *cur;
        while (cur = get_token()) {
            cur->print();
            delete cur;
        }
}
