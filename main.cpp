
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
template<class V>
class TokenVal: public Token
{
    V val;
public:
    TokenVal(int L, int C, string TOK, string LEKS, V val):
            val(val),
            Token(L, C, TOK, LEKS){}
    void print(){
        fout << l << "\t" << c << "\t" << tok << "\t" << leks << "\t" << val << endl;
    }
};
template<>
void TokenVal<double>::print() {
    char buf[11];
    sprintf(buf, "%.4E", val);
    buf[8] = buf[9]; buf[9] = buf[10]; buf[10] = 0;
    fout << l << "\t" << c << "\t" << tok <<"\t" << leks << "\t" << buf << endl;
}

class Error
{
protected:
    int l;
    int c;
    string er;
public:
    Error (int L, int C, string ER)
    {
        l = L;
        c = C;
        er = ER;
    }
    void print (){
        fout << l << "\t" << c << "\t" << er << endl;
    }
    ~Error(){}
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

bool ishex(char c)
{
    return c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F';
}

void ReplaceAll(string *str, string from, string to)
{
    unsigned int start_pos = 0;
    while((start_pos = (*str).find(from, start_pos)) != std::string::npos) {
        (*str).replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

Token *hex(string leksema, int lineCur, int columnCur, string tokenType){
	  long int a;
      string s;
      s = leksema.substr(1, leksema.size());
      a = strtol (s.c_str(), NULL, 16);
      TokenVal<int> *tokenVal = new TokenVal<int>(lineCur, columnCur, tokenType, leksema, a);
	  return tokenVal;	
}

Token *integer(string leksema, int lineCur, int columnCur, string tokenType){
	 int a;
     a = atoi(leksema.c_str());
     TokenVal<int> *tokenVal = new TokenVal<int>(lineCur, columnCur, tokenType, leksema, a);
	 return tokenVal;
			
}

Token *real(string leksema, int lineCur, int columnCur, string tokenType){
	   double a;
       a = atof(leksema.c_str());
       TokenVal<double> *tokenVal = new TokenVal<double>(lineCur, columnCur, "real", leksema, a);
	   return tokenVal;	
}

Token *get_token ()
{	
	string leksema = "";
    string tokenType = "";
    int lineCur=line;
    int columnCur=column;
	string eror;

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
		if (currentSymbol == '(') {
			leksema += currentSymbol;
			next_char();
			if (currentSymbol == '*'){
				leksema += currentSymbol;
				next_char();
				while(1){
					if (currentSymbol == '*'){
						leksema += currentSymbol;
						next_char();
						if (currentSymbol == ')'){
						leksema += currentSymbol;
						next_char();
						tokenType ="comment";
						break;
						}
					}
					leksema += currentSymbol;
					next_char();
				}
			}
			else { tokenType ="sep"; }
		}
		else {tokenType ="sep"; 
		leksema += currentSymbol;
		next_char();}
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
		 else	if (in_masiv(currentSymbol, op_second_char, sizeof(op_second_char)/sizeof(op_second_char[0]))) {
			leksema += currentSymbol;
			if (currentSymbol == '.') {
				next_char();
				if (currentSymbol == '.'){
					leksema += currentSymbol;
					next_char();
					tokenType ="sep";
				}
				else {tokenType ="op";}
			}
			else if (currentSymbol == '/'){
				next_char();
				if (currentSymbol == '/'){
					while(1){
						if (currentSymbol == '\n' || currentSymbol == '?'  ) 
						{break;}
						leksema += currentSymbol;
						next_char();
					}
					tokenType ="comment"; 
				}
				else if (currentSymbol == '='){
					leksema += currentSymbol;
					next_char();
					tokenType ="op";
				}
				else {
					tokenType ="op";
				}
			}
			else if (currentSymbol == '<') {
				tokenType ="op";
				next_char();
				if (currentSymbol == '>' || currentSymbol == '=') {
					leksema += currentSymbol;
					next_char();
			}
		}
			else {next_char();
			tokenType ="op"; 
			if (currentSymbol == '=') {
				leksema += currentSymbol;
				next_char();
			}
			}
		}
	}
	else if (currentSymbol == '$') {
			leksema+=currentSymbol;
			next_char();
			if  (ishex(currentSymbol) || isdigit(currentSymbol)){
				while(isdigit(currentSymbol) || ishex(currentSymbol))
				{	
					leksema+=currentSymbol;
					next_char();
					tokenType = "hex";	
				}
			}		
			else {	eror="NoHex";
				throw new Error(lineCur, columnCur+1, eror);}
	}
	else if (currentSymbol == '#') {
		leksema+=currentSymbol;
		next_char();
		if (currentSymbol == '$') {
			leksema+=currentSymbol;
			next_char();
		}
		while(ishex(currentSymbol) || isdigit(currentSymbol)){
			leksema+=currentSymbol;
			next_char();
		}
		tokenType = "char";
	}
	 else if (currentSymbol == '\'')
    {	int size = 0;
        leksema+=currentSymbol;
        next_char();
        while (1)
        {	if (currentSymbol == '\n'){ eror="BadNL";
				throw new Error(lineCur, columnCur+1, eror);}
			if (currentSymbol =='?') {eror="BadEOF";
				throw new Error(lineCur, column, eror);}
			else if (currentSymbol == '\'')
            {	next_char();
                 if (currentSymbol == '\'')
                {
                    leksema += string(2, currentSymbol);
                    size++;
                    next_char();
                }
                else
                {
                    leksema += '\'';
                    string value = leksema.substr(1, leksema.size() - 2);
                    ReplaceAll(&value, "''", "'");
                    if (size == 1)
                        return new TokenVal<char>(lineCur, columnCur,"char", leksema, leksema[1]);
                    else
                        return new TokenVal<string>(lineCur, columnCur, "string", leksema, value);
                }
            }
            else leksema += currentSymbol, size++,  next_char();
        }
    }
	else if (currentSymbol == '{') {
		while(currentSymbol != '}'){
			 leksema +=  currentSymbol;
             next_char();
		}
		leksema +=  currentSymbol;
		tokenType = "comment";
		next_char();
	}
	 else if (!fin.eof()) {eror="BadChar";
	 throw new Error(lineCur, columnCur+1, eror);}
	  if (tokenType == "comment")
        {
            return get_token();
        }
	 if ( tokenType == "integer")
        {
            return integer(leksema, lineCur, columnCur, tokenType);
        }
        if (tokenType == "real")
        {
            return real( leksema, lineCur,  columnCur,tokenType);
        }
	if (tokenType == "hex")
        {
            return hex( leksema,  lineCur,  columnCur,  tokenType);
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
       try {
        while (cur = get_token()) {
            cur->print();
            delete cur;
        }
    }catch (Error *e) { e->print(); }
}
