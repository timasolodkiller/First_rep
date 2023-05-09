#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;
enum type_of_lex {
    LEX_NULL,                                                                                   
    LEX_AND, LEX_BOOLEAN, LEX_STRING, LEX_DO, LEX_IF, LEX_FALSE, LEX_INT,                     
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_TRUE, LEX_CONT, LEX_WRITE,                     
    LEX_WHILE,
    LEX_FIN,                                                                                    
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS,   
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ,           
    LEX_LFIG, LEX_RFIG, LEX_PERC,                                                             
    LEX_NUM,                                                                             
    LEX_STR,                                                                               
    LEX_ID,                                                                                   
    LEX_BOOL,
    POLIZ_LABEL,                                                                                
    POLIZ_ADDRESS,                                                                             
    POLIZ_GO,                                                                                   
    POLIZ_FGO                                                                                   
};
class Lex {
    type_of_lex   t_lex;
    int           v_lex;
    string        s_lex;
public:
    Lex ( type_of_lex t = LEX_NULL, int v = 0, string str = ""): t_lex (t), v_lex (v) , s_lex(str) { }
    type_of_lex  get_type () const {
      return t_lex;
    }
    int get_value () const {
      return v_lex;
    }
    string get_str () const {
      return s_lex;
    }
    friend ostream & operator<< ( ostream &s, Lex l );
};
class Ident {
    string      name;
    bool        declare;
    type_of_lex type;
    bool        assign;
    int         value;
    string      svalue;
public:
    Ident() {
      declare = false;
      assign  = false;
    }
    bool operator== ( const string& s ) const {
      return name == s;
    }
    Ident ( const string n ) {
      name    = n;
      declare = false;
      assign  = false;
    }
    string get_name () const {
      return name;
    }
    bool get_declare () const {
      return declare;
    }
    void put_declare () {
      declare   = true;
    }
    type_of_lex get_type () const {
      return type;
    }
    void put_type ( type_of_lex t ) {
      type      = t;
    }
    bool get_assign () const {
      return assign;
    }
    void put_assign () {
      assign    = true;
    }
    int  get_value () const {
      return value;
    }
    void put_value ( int v ) {
      value     = v;
    }
    string get_svalue () const {
      return svalue;
    }
    void put_svalue ( string s ) {
      svalue    = s;
    }
};
vector<Ident> TID;
int put ( const string & buf ) {
  vector<Ident>::iterator k;

  if ( ( k = find ( TID.begin (), TID.end (), buf ) ) != TID.end () )
    return k - TID.begin();
  TID.push_back ( Ident(buf) );
  return TID.size () - 1;
}

void print_TID () {
  for ( auto k = TID.begin(); k < TID.end(); k ++) {
    cout << k->get_name() << endl
      << k->get_declare() << endl
      << k->get_type() << endl
      << k->get_assign() << endl
      << k->get_value() << endl
      << k->get_svalue() << endl;
  }

}
class Scanner {
    FILE * fp;
    char   c;
    int look ( const string buf, const char ** list ) {
      int i = 0;
      while ( list[i] ) {
        if ( buf == list[i] )
          return i;
        ++i;
      }
      return 0;
    }
    void gc () {
      c = fgetc (fp);
    }
public:
    static const char * TW [], * TD [];
    Scanner ( const char * program ) {
      if ( !(fp = fopen ( program, "r" )) )
        throw  "can't open file" ;
    }
    Lex get_lex ();
};

const char *
    Scanner::TW    [] = { "", "and", "boolean", "string", "do","if","false", "int",
                          "not", "or","program","read", "true", "continue","write",
                          "while", NULL };

const char *
    Scanner::TD    [] = { "", ";", ",", ":", "=", "(", ")", "==", "<",
                          ">", "+", "-","*", "/", "<=", "!=", ">=",
                          "{", "}", "%",NULL };

Lex Scanner::get_lex () {
  enum    state { H, IDENT, NUMB, ALE, STR, COM };
  int     d = 0, j;
  string  buf;
  state   CS = H;
  do {
    gc ();
    switch ( CS ) {
      case H:
        if ( c==' ' || c == '\n' || c== '\r' || c == '\t' );
        else if ( isalpha (c) ) {                         
          buf.push_back (c);
          CS  = IDENT;
        }
        else if ( isdigit (c) ) {
          d   = c - '0';
          CS  = NUMB;
        }
        else if (c == EOF)
          return Lex ( LEX_FIN );
        else if ( c == '=' || c == '<' || c == '>' || c == '!') {
          buf.push_back (c);
          CS  = ALE;
        }
        else if ( c == '"') {
          CS = STR;
        }
        else if ( c == '/') {
          gc();
          if ( c == '*')
            CS = COM;
          else {
            ungetc(c, fp);
            buf.push_back('/');
            if ( ( j = look ( buf, TD) ) ){
              return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j );
            }
            else {
              throw c;
            }
          }

        }
        else {         
          buf.push_back (c);

          if ( ( j = look ( buf, TD) ) ){
            return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j );
          }
          else {
            throw c;
          }
        }
        break;
      case IDENT:
        if ( isalpha (c) || isdigit (c) ) {
          buf.push_back (c);
        }
        else {
          ungetc ( c, fp );
          if ( (j = look ( buf, TW) ) ) {
            if ( j == int (LEX_TRUE) )
              return Lex( LEX_BOOL, 1 );
            else if ( j == int (LEX_FALSE) )
              return Lex( LEX_BOOL, 0 );
            else
              return Lex ( (type_of_lex) j, j );
          }
          else {
            j   = put ( buf );
            return Lex ( LEX_ID, j );
          }
        }
        break;
      case NUMB:
        if ( isdigit (c) ) {
          d = d * 10 + ( c - '0' );
        }
        else {
          ungetc ( c, fp );
          return Lex ( LEX_NUM, d );
        }
        break;
      case ALE:
        if ( c == '=' ) {
          buf.push_back ( c );
          j   = look ( buf, TD );
          return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
        }
        else {
          ungetc ( c, fp );
          j   = look ( buf, TD );
          return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
        }
        break;
      case STR:
        if ( c != '"' )
          buf.push_back(c);
        else {
          return Lex (LEX_STR, 0, buf);
        }
      case COM:
        if ( c == EOF )
          throw c;
        else if ( c == '*' ) {
          gc();
          if ( c == '/' )
            CS = H;
          else if ( c == EOF )
            throw c;
        }
    } //end switch
  } while (true);
}

ostream & operator<< ( ostream &s, Lex l ) {
  string t;
  if ( l.t_lex == LEX_NULL )
    t = "NULL";
  else if ( l.t_lex < LEX_FIN )
    t = Scanner::TW[l.t_lex];
  else if ( l.t_lex == LEX_FIN )
    t = "FIN";
  else if ( l.t_lex > LEX_FIN && l.t_lex < LEX_NUM )
    t = Scanner::TD[ l.t_lex - LEX_FIN ];
  else if ( l.t_lex == LEX_NUM )
    t = "NUMB";
  else if (l.t_lex == LEX_STR )
    t = "STR";
  else if ( l.t_lex == LEX_ID )
    t = TID[l.v_lex].get_name ();
  else if ( l.t_lex == LEX_BOOL )
    t = "BOOL";
  else if ( l.t_lex == POLIZ_LABEL )
    t = "Label";
  else if ( l.t_lex == POLIZ_ADDRESS )
    t = "Addr";
  else if ( l.t_lex == POLIZ_GO )
    t = "!";
  else if ( l.t_lex == POLIZ_FGO )
    t = "!F";
  else
    throw l;
  s << '(' << t << ',' << l.v_lex << ");" << endl;
  return s;
}
template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
  i = st.top(); st.pop();
}

class Parser {
    Lex          curr_lex;
    type_of_lex  c_type;
    int          c_val;
    string       c_str;
    Scanner      scan;
    stack < int >           st_while;
    stack < Lex >           st_lex;
    stack < type_of_lex >   st_tlex;

    type_of_lex dec_type;
    bool operat_flag;

    void  program();          
    void  descripts();
    void  descript();
    void  variable();
    void  operats();
    void  operat();
    void expr6();
    void expr5();
    void expr4();
    void expr3();
    void expr2();
    void expr1();

    void  dec ( type_of_lex type);  
    void  check_id ();
    void  check_op ();
    void  check_not ();
    void  eq_type ();
    void  eq_bool ();
    void  check_id_in_read ();

    void  gl () {
      curr_lex  = scan.get_lex ();
      c_type    = curr_lex.get_type ();
      c_val     = curr_lex.get_value ();
      c_str     = curr_lex.get_str();
      //cout << curr_lex;
    }
public:
    vector <Lex> poliz;

    Parser ( const char *program ) : scan (program) { }
    void  analyze();
};

void Parser::analyze () {

  program ();
  if (c_type != LEX_FIN)
    throw curr_lex;
}

void Parser::program() {
  gl();
  if ( c_type == LEX_PROGRAM ) {
    gl ();
  }
  else
    throw curr_lex;

  if ( c_type == LEX_LFIG )
    gl();
  else
    throw curr_lex;

  descripts();
  operats();

  if ( c_type == LEX_RFIG )
    gl();
  else
    throw curr_lex;
}

void Parser::descripts() {
  operat_flag = true;
  while (operat_flag) {
    operat_flag = false;
    descript();
  }
}

void Parser::descript() {
  if ( c_type == LEX_BOOLEAN or c_type == LEX_INT or c_type == LEX_STRING ) {

    if (c_type == LEX_BOOLEAN )
      dec_type = LEX_BOOL;
    else if ( c_type == LEX_INT )
      dec_type = LEX_NUM;
    else
      dec_type = LEX_STR;

    do {
      gl();
      variable();
    } while ( c_type == LEX_COMMA );

    if ( c_type == LEX_SEMICOLON )
      gl();
    else
      throw curr_lex;

    dec(dec_type );

    operat_flag = true;
  }

}

void Parser::variable() {
  if ( c_type == LEX_ID ) {
    st_lex.push ( curr_lex );
    st_tlex.push ( dec_type );
    poliz.push_back (Lex ( POLIZ_ADDRESS, c_val ) );
    gl();
  }
  else
    throw curr_lex;

  if ( c_type == LEX_ASSIGN ) {
    gl();
    if ( c_type == LEX_BOOL or c_type == LEX_NUM or c_type == LEX_STR ) {
      st_tlex.push( c_type );
      eq_type();
      poliz.push_back ( Lex ( curr_lex ) );
      poliz.push_back ( Lex ( LEX_ASSIGN ) );
      gl();
    }
    else
      throw curr_lex;
  }
  else {
    st_tlex.pop();
    poliz.pop_back();
  }

}

void Parser::operats() {
  operat_flag = true;
  while ( operat_flag ) {
    operat_flag = false;
    operat();
  }
}

void Parser::operat() {
  int pl0, pl1, pl2, pl3;

  if ( c_type == LEX_IF ) {
    gl();
    if ( c_type == LEX_LPAREN )
      gl();
    else
      throw curr_lex;

    expr6();
    eq_bool();
    pl2 = poliz.size();
    poliz.push_back ( Lex() );
    poliz.push_back ( Lex(POLIZ_FGO) );

    if ( c_type == LEX_RPAREN )
      gl();
    else
      throw curr_lex;

    operat();


    poliz[pl2] = Lex ( POLIZ_LABEL, poliz.size() );

    operat_flag = true;
  }
  else if ( c_type == LEX_WHILE ) {
    gl();
    if ( c_type == LEX_LPAREN )
      gl();
    else
      throw curr_lex;

    pl0 = poliz.size ();
    expr6();
    eq_bool();
    pl1 = poliz.size ();
    poliz.push_back ( Lex () );
    poliz.push_back ( Lex (POLIZ_FGO) );

    st_while.push(pl0);

    if ( c_type == LEX_RPAREN )
      gl();
    else
      throw curr_lex;


    operat();

    poliz.push_back ( Lex ( POLIZ_LABEL, pl0 ) );
    poliz.push_back ( Lex ( POLIZ_GO) );
    poliz[pl1] = Lex ( POLIZ_LABEL, poliz.size() );

    operat_flag = true;
    st_while.pop();
  }
  else if ( c_type == LEX_CONT ) {
    if ( st_while.empty() )
      throw "continue outside while";

    pl0 = st_while.top();
    poliz.push_back ( Lex ( POLIZ_LABEL, pl0 ) );
    poliz.push_back ( Lex ( POLIZ_GO) );
    gl();
    if ( c_type == LEX_SEMICOLON )
      gl();
    else
      throw curr_lex;

    operat_flag = true;
  }
  else if ( c_type == LEX_READ ) {
    gl();

    if ( c_type == LEX_LPAREN )
      gl();
    else
      throw curr_lex;

    if ( c_type == LEX_ID ) {
      check_id_in_read();
      poliz.push_back ( Lex( POLIZ_ADDRESS, c_val) );
      gl();
    }
    else
      throw curr_lex;

    if ( c_type == LEX_RPAREN )
      gl();
    else
      throw curr_lex;

    if ( c_type == LEX_SEMICOLON )
      gl();
    else
      throw curr_lex;

    poliz.push_back ( Lex (LEX_READ) );

    operat_flag = true;
  }
  else if ( c_type == LEX_WRITE ) {
    gl();

    if ( c_type == LEX_LPAREN)
      gl();
    else
      throw curr_lex;

    do {
      expr6();
    } while ( c_type == LEX_COMMA );

    if ( c_type == LEX_RPAREN )
      gl();
    else
      throw curr_lex;

    if ( c_type == LEX_SEMICOLON )
      gl();
    else
      throw curr_lex;

    poliz.push_back ( Lex ( LEX_WRITE ) );

    operat_flag = true;
  }
  else if ( c_type == LEX_LFIG ) {
    gl();

    operats();

    if ( c_type == LEX_RFIG )
      gl();
    else
      throw curr_lex;

    operat_flag = true;
  }
  else if ( c_type == LEX_ID ) {
    check_id ();
    poliz.push_back (Lex ( POLIZ_ADDRESS, c_val ) );
    gl();
    if ( c_type == LEX_ASSIGN ) {
      gl ();
      expr6 ();
      eq_type ();
      poliz.push_back ( Lex ( LEX_ASSIGN ) );
    }
    else
      throw curr_lex;

    if ( c_type == LEX_SEMICOLON )
      gl();
    else
      throw curr_lex;

    operat_flag = true;
  }

}

void Parser::expr6() {
  expr5();
  if ( c_type == LEX_OR ) {
    st_tlex.push ( c_type );
    gl();
    expr5();
    check_op ();
  }
}

void Parser::expr5() {
  expr4();
  if ( c_type == LEX_AND ) {
    st_tlex.push ( c_type );
    gl();
    expr4();
    check_op ();
  }
}

void Parser::expr4() {
  expr3();
  if ( c_type == LEX_GTR or c_type == LEX_LSS or c_type == LEX_GEQ or c_type == LEX_EQ or c_type ==  LEX_LEQ
        or c_type == LEX_NEQ) {
    st_tlex.push ( c_type );
    gl();
    expr3();
    check_op ();
  }
}

void Parser::expr3() {
  expr2();
  if ( c_type == LEX_PLUS or c_type == LEX_MINUS ) {
    st_tlex.push ( c_type );
    gl();
    expr2();
    check_op ();
  }
}

void Parser::expr2() {
  expr1();
  if ( c_type == LEX_TIMES or c_type == LEX_SLASH or c_type == LEX_PERC ) {
    st_tlex.push ( c_type );
    gl();
    expr1();
    check_op ();
  }
}

void Parser::expr1() {
  if ( c_type == LEX_ID ) {
    check_id ();
    poliz.push_back ( Lex ( LEX_ID, c_val ) );
    gl();
  }
  else if ( c_type == LEX_NUM or c_type == LEX_STR or c_type == LEX_BOOL ) {
    st_tlex.push ( c_type );
    poliz.push_back( curr_lex );
    gl();
  }
  else if ( c_type == LEX_NOT ) {
    gl();
    expr1();
    check_not ();
  }
  else if ( c_type == LEX_LPAREN ) {
    gl();
    expr6();
    if ( c_type == LEX_RPAREN )
      gl();
    else
      throw curr_lex;

  }
}
void Parser::dec ( type_of_lex type ) {
  //cout << "Declaring " << type << endl;
  Lex lexeme;
  int i;
  while ( !st_lex.empty () ) {
    from_st ( st_lex, lexeme );
    //cout << "got " << lexeme;
    i = lexeme.get_value();
    if ( TID[i].get_declare () )
      throw "twice";
    else {
      TID[i].put_declare ();
      TID[i].put_type ( type );
    }
  }
}

void Parser::check_id () {
  if ( TID[c_val].get_declare() )
    st_tlex.push ( TID[c_val].get_type () );
  else
    throw "not declared";
}

void Parser::check_op () {
  type_of_lex t1, t2, op, t, r;

  from_st(st_tlex, t2);
  from_st(st_tlex, op);
  from_st(st_tlex, t1);
  if (t1 != t2) throw "wrong types are in operation";
  else t = t1;
  if (op == LEX_PLUS) {
    if (t == LEX_STR or t == LEX_NUM)
      r = t;
    else
      throw "wrong types are in operation";
  } else if (op == LEX_MINUS or op == LEX_SLASH or op == LEX_TIMES or op == LEX_PERC) {
    if (t == LEX_NUM)
      r = t;
    else
      throw "wrong types are in operation";
  } else if (op == LEX_GTR or op == LEX_LSS or op == LEX_EQ or op == LEX_NEQ) {
    if (t == LEX_STR or t == LEX_NUM)
      r = LEX_BOOL;
    else
      throw "wrong types are in operation";
  } else if (op == LEX_GEQ or op == LEX_LEQ) {
    if (t == LEX_NUM)
      r = LEX_BOOL;
    else
      throw "wrong types are in operation";
  } else if (op == LEX_AND or op == LEX_OR) {
    if (t == LEX_BOOL)
      r = LEX_BOOL;
    else
      throw "wrong types are in operation";
  } else
    throw "i dont know what went wrong";

  st_tlex.push(r);
  poliz.push_back (Lex (op) );
}

void Parser::check_not () {
  if (st_tlex.top() != LEX_BOOL)
    throw "wrong type is in not";
  poliz.push_back (Lex (LEX_NOT) );
}

void Parser::eq_type () {
  type_of_lex t;
  from_st ( st_tlex, t );
  if ( t != st_tlex.top () )
    throw "wrong types are in =";
  st_tlex.pop();
}

void Parser::eq_bool () {
  if (st_tlex.top () != LEX_BOOL )
    throw "expression is not boolean";
  st_tlex.pop ();
}

void Parser::check_id_in_read () {
  if ( !TID [c_val].get_declare() )
    throw "not declared";
}
class Executer {
public:
    void execute ( vector<Lex> & poliz );
};

void Executer::execute ( vector<Lex> & poliz ) {
  Lex pc_el;
  stack < Lex > args;
  int val, index = 0, size = poliz.size();
  Lex i, j;
  while ( index < size ) {
    pc_el = poliz [ index ];
    //cout << index << ' ' << pc_el;
    switch ( pc_el.get_type () ) {
      case LEX_BOOL: case LEX_STR: case LEX_NUM: case POLIZ_ADDRESS: case POLIZ_LABEL:
        args.push ( pc_el );
        break;

      case LEX_ID:
        val = pc_el.get_value ();
        if ( TID[val].get_assign () ) {
          args.push ( Lex ( TID[val].get_type(), TID[val].get_value (), TID[val].get_svalue() ) );
          break;
        }
        else
          throw "POLIZ: indefinite identifier";

      case LEX_NOT:
        from_st ( args, i );
        args.push( Lex( i.get_type(), !i.get_value() ) );
        break;

      case LEX_OR:
        from_st ( args, j );
        from_st ( args, i );
        args.push ( Lex( i.get_type(), i.get_value() || j.get_value() ) );
        break;

      case LEX_AND:
        from_st ( args, j );
        from_st ( args, i );
        args.push ( Lex( i.get_type(), i.get_value() && j.get_value() ) );
        break;

      case POLIZ_GO:
        from_st ( args, i );
        index = i.get_value() - 1;
        break;

      case POLIZ_FGO:
        from_st ( args, j );
        from_st ( args, i );
        if ( !i.get_value() ) index = j.get_value() - 1;
        break;

      case LEX_WRITE:
        from_st ( args, i );
        switch ( i.get_type() ) {
          case LEX_NUM :
            cout << i.get_value();
            break;
          case LEX_BOOL:
            cout << (i.get_value() == 1);
            break;
          case LEX_STR:
            cout << i.get_str();
            break;
        }
        break;

      case LEX_READ:
        from_st ( args, i );
        val = i.get_value();
        if ( TID[val].get_type () == LEX_NUM ) {
          int k;
          cin >> k;
          TID[val].put_value (k);
        }
        else if ( TID[val].get_type () == LEX_BOOL ) {
          int k;
          string s;
          cin >> s;
          if ( s != "true" && s != "false" )
            throw "Error in input:true/false";
          k = ( s == "true" ) ? 1 : 0;
          TID[val].put_value (k);
        }
        else {
          string s;
          cin >> s;
          TID[val].put_svalue(s);
        }
        TID[val].put_assign ();
        break;

      case LEX_PLUS:
        from_st ( args, j );
        from_st ( args, i );
        args.push ( Lex( i.get_type(), i.get_value() + j.get_value(), i.get_str() + j.get_str() ) );
        break;

      case LEX_TIMES:
        from_st ( args, j );
        from_st ( args, i );
        args.push ( Lex( i.get_type(), i.get_value() * j.get_value() ) );
        break;

      case LEX_MINUS:
        from_st ( args, j );
        from_st ( args, i );
        args.push ( Lex( i.get_type(), i.get_value() - j.get_value() ) );
        break;

      case LEX_SLASH:
        from_st ( args, j );
        from_st ( args, i );
        if (j.get_value()) {
          args.push ( Lex( i.get_type(), i.get_value() / j.get_value() ) );
          break;
        }
        else
          throw "POLIZ:divide by zero";

      case LEX_PERC:
        from_st ( args, j );
        from_st ( args, i );
        if (j.get_value()) {
          args.push ( Lex( i.get_type(), i.get_value() % j.get_value() ) );
          break;
        }
        else
          throw "POLIZ:divide by zero";

      case LEX_EQ:
        from_st ( args, j );
        from_st ( args, i );
        if (i.get_type() == LEX_STR)
          args.push ( Lex( LEX_BOOL, i.get_str() == j.get_str() ) );
        else
          args.push ( Lex( LEX_BOOL, i.get_value() == j.get_value() ) );
        break;

      case LEX_LSS:
        from_st ( args, j );
        from_st ( args, i );
        if (i.get_type() == LEX_STR)
          args.push ( Lex( LEX_BOOL, i.get_str() < j.get_str() ) );
        else
          args.push ( Lex( LEX_BOOL, i.get_value() < j.get_value() ) );
        break;

      case LEX_GTR:
        from_st ( args, j );
        from_st ( args, i );
        if (i.get_type() == LEX_STR)
          args.push ( Lex( LEX_BOOL, i.get_str() > j.get_str() ) );
        else
          args.push ( Lex( LEX_BOOL, i.get_value() > j.get_value() ) );
        break;

      case LEX_LEQ:
        from_st ( args, j );
        from_st ( args, i );
        if (i.get_type() == LEX_STR)
          args.push ( Lex( LEX_BOOL, i.get_str() <= j.get_str() ) );
        else
          args.push ( Lex( LEX_BOOL, i.get_value() <= j.get_value() ) );
        break;

      case LEX_GEQ:
        from_st ( args, j );
        from_st ( args, i );
        if (i.get_type() == LEX_STR)
          args.push ( Lex( LEX_BOOL, i.get_str() >= j.get_str() ) );
        else
          args.push ( Lex( LEX_BOOL, i.get_value() >= j.get_value() ) );
        break;

      case LEX_NEQ:
        from_st ( args, j );
        from_st ( args, i );
        if (i.get_type() == LEX_STR)
          args.push ( Lex( LEX_BOOL, i.get_str() != j.get_str() ) );
        else
          args.push ( Lex( LEX_BOOL, i.get_value() != j.get_value() ) );
        break;

      case LEX_ASSIGN:
        from_st ( args, j );
        from_st ( args, i );
        val = i.get_value();
        TID[val].put_value (j.get_value());
        TID[val].put_svalue (j.get_str());
        TID[val].put_assign ();
        break;
      default:
        throw "POLIZ: unexpected elem ";
    }//end of switch
    ++index;
  };//end of while
  cout << "Finish of executing!!!" << endl;
}


class Interpretator {
    Parser   pars;
    Executer E;
public:
    Interpretator ( const char* program ): pars (program) {}
    void     interpretation ();
};

void Interpretator::interpretation () {
  pars.analyze ();
  cout << "syntax analysis complete, all's fine\n";
  E.execute ( pars.poliz );
}
int main () {
  try {
    Interpretator I ( "p.txt" );
    I.interpretation ();

  }
  catch ( char c ) {
    cout << "unexpected symbol " << int(c) << ' ' << c << endl;
    return 1;
  }
  catch ( Lex l ) {
    cout << "unexpected lexeme" << l << endl;
    return 1;
  }
  catch ( const char *source ) {
    cout << source << endl;
    return 1;
  }
  return 0;
}
