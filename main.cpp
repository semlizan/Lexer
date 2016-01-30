
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>

using namespace std;


ifstream fin("input.txt");
ofstream fout("output.txt");
char currentSymbol;
int line = 1;
int column = 0;
int columnError = 0;
char ops_second_char[] = {'/','+','*',' :','-','>', '^', '<', '@', '.', '='};
char sep[] = {';' ,',','(','[',')',']',};
char ops[] = {'>','<','/','+','*',':','-','^','=','@',';',',','(','[',')',']','.'};

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


int main()
{
    fin >> noskipws;
    next_char();
}
