#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <cctype>
#include <string>

using namespace std;

enum lexem_type
{
    LEX_NULL,
    LEX_AND, LEX_BEGIN, LEX_ELSE, LEX_END, LEX_IF, LEX_INT, LEX_REAL, LEX_STRING,
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_WHILE, LEX_FOR, LEX_CONST, LEX_CONTINUE, LEX_WRITE,
    LEX_FIN,
    LEX_SEMICOLON, 
    LEX_COMMA, LEX_COLON, 
    LEX_ASSIGN, LEX_LPAREN, 
    LEX_RPAREN, 
    LEX_EQ, LEX_LSS, LEX_LFIG, LEX_RFIG,
    LEX_GRT, 
    LEX_PLUS, LEX_MINUS, LEX_MUL, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_QUOTE, LEX_MOD,
    LEX_NUM,LEX_REALNUM, LEX_STR,
    LEX_ID
};

class Lexema
{
    lexem_type tlex;
    string value_lex;
  public:
    Lexema(lexem_type t = LEX_NULL, const string v = ""): tlex(t){ value_lex = v;}
    lexem_type get_type() const {return tlex;}
    string get_value() const {return value_lex;}
};

class Identificator
{
    string name;
    bool inited;
    lexem_type type;
    string value;
  public:
    Identificator() { name = nullptr; inited = false;}
    Identificator(const string s) { name = s; inited = false;}
    string get_name() const { return name;}
    bool get_inited() const { return inited;}
    void put_inited(bool i) { inited = i;}
    lexem_type get_type() const { return type;}
    void set_type(lexem_type t) { type = t;}
    string get_value() const { return value;}
    void set_value(int v) { value = v;}
    friend int put(const string & s);
};

vector <Identificator> TID;

int put(const string & s)
{
    vector <Identificator>::iterator i;

    for (i = TID.begin(); i != TID.end(); i++)
        if (i->name == s)
            return i - TID.begin();
    TID.push_back(Identificator(s));
    return TID.size() - 1;
}

class Scanner
{
    FILE * fp;
    char c;
    int look(const string s, const char ** spisok)
    {
        for (int i = 0; spisok[i]; i++)
            if (s == spisok[i]) return i;
        return 0;
    }
    void get_symbol()
    {
        c = fgetc(fp);
    }
  public:
    static const char * TW[], * TD [];
    Scanner(const char * program)
    {
            if (!(fp = fopen(program, "r")))
            throw "The file cannot be opened";
    }
    Lexema get_lexem();
};

const char * Scanner::TW[] = {"", "and", "begin", "else", "end", "if", "int", "real", "string", "not", "or", "program", "read", "then", "while", "for", "const", "continue", "write", NULL};
const char * Scanner::TD[] = {"@", ";", ",", ":", "=", "(", ")", "==", "<", "{", "}", ">", "+", "-", "*", "/", "<=", "!=", ">=", "\"", "%", NULL};
ostream & operator<< (ostream & out, const Lexema& lex)
{
    string tip;
    if (lex.get_type() < LEX_FIN)
        tip = Scanner::TW[lex.get_type()];
    else
        if (lex.get_type() < LEX_NUM)
        tip = Scanner::TD[lex.get_type() - LEX_FIN];
    else
        if (lex.get_type() == LEX_NUM)
            tip = "NUMBER_INTEGER";
    else
        if (lex.get_type() == LEX_REALNUM)
            tip = "NUMBER_REAL";
    else
        if (lex.get_type() == LEX_STR)
            tip = "STRING";
    else
        if (lex.get_type() == LEX_ID)
            tip = TID[stoi(lex.get_value())].get_name();
    out << "(" << tip << ", " << lex.get_value() << ")";
    return out;
}
Lexema Scanner::get_lexem()
{
    enum state {H, IDENTIFICATOR, NUMBER, COMMENT, ALG, NEQ, STR};    
    string s;
    int j;
    state AS = H;     
    bool R = false;   
    do
    {
        get_symbol();
        switch (AS)
        {
        case H:
            if (isspace(c)) {}
            else
                if (isalpha(c))
            {
                s.push_back(c);
                AS = IDENTIFICATOR;
            }
            else
                if (isdigit(c))
            {

                s.push_back(c);
                AS = NUMBER;
            }
            else
                if ( c == '/')
                {
                    get_symbol();
                    if (c == '*')
                        AS = COMMENT;
                    else
                    {
                        ungetc(c, fp);
                        s.push_back(c);
                        if (j = look(s, TD))
                            return Lexema((lexem_type) ((int) LEX_FIN + j), to_string(j));
                        else
                            throw c;
                    }
                }
            else
                if ( c == '"')
                    AS = STR;
            else
                if (c == '=' || c == '<' || c == '>')
            {
                s.push_back(c);
                AS = ALG;
            }
            else
                if (c == '!')
                    AS = NEQ;
            else
                if (c == '@')
                    return Lexema(LEX_FIN);
            else
            {
                s.push_back(c);
                if (j = look(s, TD))
                    return Lexema((lexem_type) ((int) LEX_FIN + j), to_string(j));
                else
                    throw c;
            }
            break;
        case IDENTIFICATOR:
            if (isalpha(c) || isdigit(c))
                s.push_back(c);
            else
            {
                ungetc(c, fp);
                if (j = look(s, TW))
                    return Lexema((lexem_type) j, to_string(j));
                else
                {
                    j = put(s);
                    return Lexema(LEX_ID, to_string(j));
                }
            }
            break;
        case COMMENT:
            if ((c == '@'))
                throw c;
            if (c == '/')
            {
                get_symbol();
                if (c == '*')
                    throw c;
                else
                {
                    ungetc(c, fp);
                    s.push_back(c);
                }
            }
            if (c == '*')
            {
                get_symbol();
                if (c == '/')
                    AS = H;
                else
                {
                    ungetc(c, fp);
                    s.push_back(c);
                }
            }
            break;
        case NEQ:
            if (c == '=')
            {
                s.push_back(c);
                j = look(s, TD);
                return Lexema(LEX_NEQ, to_string(j));
            }
            else
                throw '!';
            break;
        case  ALG:
            if (c == '=')
                s.push_back(c);
            else
                ungetc(c, fp);
            j = look(s, TD);
            return Lexema((lexem_type) (j + (int) LEX_FIN), to_string(j));
            break;
        case NUMBER:
            if (isdigit(c))
                s.push_back(c);
            else
                if (c == '.')
                {
                    if ((!R) && (s.size() != 0))
                    {
                        s.push_back(c);
                        R = true;
                    }
                    else
                        throw c;
                }
            else
                if (isalpha(c))
                    throw c;
            else
            {
                ungetc(c, fp);
                if (!R)
                {
                    return Lexema(LEX_NUM, s);
                }
                return Lexema(LEX_REALNUM, s);
            }
            break;
        case STR:
            if (c == '\n')
                throw c;
            if (c != '"')
                s.push_back(c);
            else
                return Lexema(LEX_STR, s);
        }
    } while (true);
}

class Parser
{
    Lexema curr;
    lexem_type c_type;
    string c_value;
    Scanner sc;
    void P();
    void D1();
    void D();
    void T();
    void V();
    void I();
    void C();
    void Z();
    void S1();
    void S();
    void SS();
    void SV();
    void Vyr();
    void A();
    void B();
    void E();
    void F();
    void G();
    void H();
    void gl()
    {
        curr = sc.get_lexem();
        c_type = curr.get_type();
        c_value = curr.get_value();
    }
  public:
    Parser(const char* program): sc(program) {};
    void analyze();
};

void Parser::analyze()
{
    gl();
    P();
    if (c_type != LEX_FIN)
        throw curr;
}

void Parser::P()
{
    if (c_type == LEX_PROGRAM)
        gl();
    else
        throw curr;
    if (c_type != LEX_LFIG)
        throw curr;
    D1();
    S1();
}

void Parser::D1()
{
    while (true){
    gl();
    if (c_type == LEX_CONST)
    {
        gl();
    }
    if (c_type == LEX_INT)
    {
        gl();
    }
    else
        if (c_type == LEX_STRING)
    {
        gl();
    }
    else
        if (c_type == LEX_REAL)
    {
        gl();
    }
    else
        return;
    D();
    if (c_type != LEX_SEMICOLON)
        throw curr;
    }
}

void Parser::D()
{

    V();
    while (c_type == LEX_COMMA)
    {
        gl();
        V();
    }
}

void Parser::V()
{
    if (c_type != LEX_ID)
        throw curr;
    gl();
    if (c_type == LEX_ASSIGN)
    {
        gl();
        C();
    }
}

void Parser::C()
{
    if ((c_type != LEX_NUM) && (c_type != LEX_REALNUM) && (c_type != LEX_STR))
        throw curr;
    gl();
}

void Parser::S1()
{
    while (c_type != LEX_RFIG)
        S();
    gl();
}

void Parser::S()
{
    switch (c_type)
    {
    case LEX_IF:
        gl();
        if (c_type != LEX_LPAREN)
            throw curr;
        gl();
        Vyr();
        if (c_type != LEX_RPAREN)
            throw curr;
        gl();
        S();
        if (c_type != LEX_ELSE)
            throw curr;
        S();
        break;
    case LEX_WHILE:
        gl();
        if (c_type != LEX_LPAREN)
            throw curr;
        gl();
        Vyr();
        if (c_type != LEX_RPAREN)
            throw curr;
        gl();
        S();
        break;
    case LEX_READ:
        gl();
        if (c_type != LEX_LPAREN)
            throw curr;
        gl();
        if (c_type != LEX_ID)
            throw curr;
        gl();
        if (c_type != LEX_RPAREN)
            throw curr;
        gl();
        if (c_type != LEX_SEMICOLON)
            throw curr;
        gl();
        break;
    case LEX_WRITE:
        gl();
        if (c_type != LEX_LPAREN)
            throw curr;
        gl();
        Vyr();
        while (c_type == LEX_COMMA)
            Vyr();
        if (c_type != LEX_RPAREN)
            throw curr;
        gl();
        if (c_type != LEX_SEMICOLON)
            throw curr;
        gl();
        break;
    case LEX_FOR:
        gl();
        if (c_type != LEX_LPAREN)
            throw curr;
        gl();
        if (c_type != LEX_SEMICOLON)
            Vyr();
        if (c_type != LEX_SEMICOLON)
            throw curr;
        gl();
        if (c_type != LEX_SEMICOLON)
            Vyr();
        if (c_type != LEX_SEMICOLON)
            throw curr;
        gl();
        if (c_type != LEX_RPAREN)
            Vyr();
        if (c_type != LEX_RPAREN)
            throw curr;
        gl();
        S();
        break;
    case LEX_CONTINUE:
        gl();
        if (c_type != LEX_SEMICOLON)
            throw curr;
        gl();
        break;
    case LEX_ELSE:
        gl();
        S();
        if (c_type != LEX_RFIG)
            throw curr;
        break;
    case LEX_LFIG:
        gl();
        S1();
    case LEX_RFIG:
        break;
    default:
        Vyr();
        if (c_type != LEX_SEMICOLON)
            throw curr;
        gl();
        break;
    }
}

void Parser::Vyr()
{
    A();
    if (c_type == LEX_ASSIGN)
    {
        gl();
        A();
    }
}

void Parser::A()
{
    B();
    if (c_type == LEX_OR)
    {
        gl();
        B();
    }
}

void Parser::B()
{
    E();
    if (c_type == LEX_AND)
    {
        gl();
        E();
    }
}
void Parser::E()
{
    F();
    if ((c_type == LEX_LSS) || (c_type == LEX_GRT) || (c_type == LEX_LEQ) || (c_type == LEX_GEQ) || (c_type == LEX_EQ) || (c_type == LEX_NEQ))
    {
        gl();
        F();
    }
}
void Parser::F()
{
    G();
    if ((c_type == LEX_PLUS) || (c_type == LEX_MINUS))
    {
        gl();
        G();
    }
}
void Parser::G()
{
    H();
    if ((c_type == LEX_MUL) || (c_type == LEX_SLASH) || (c_type == LEX_MOD))
    {
        gl();
        H();
    }
}
void Parser::H()
{
    switch (c_type)
    {
    case LEX_ID:
        break;
    case LEX_NUM:
        break;
    case LEX_REALNUM:
        break;
    case LEX_STR:
        break;
    case LEX_NOT:
        gl();
        H();
        break;
    case LEX_LPAREN:
        gl();
        Vyr();
        if (c_type != LEX_RPAREN)
            throw curr;
        break;
    default:
        throw curr;
    }
    gl();
}
int main()
{
    try{
    char * program = "test.txt";
    Parser Par(program);
    Par.analyze();
    cout << "Успешно" << endl;
    }
    catch (char& c)
    {
        cout << "Непредвиденный символ" << c << endl;
    }
    catch(Lexema& L)
    {
        cout << "Непредвиденный токен" << L << endl;
    }

    return 0;
}