%{
#include <cstdlib>
#include <string>
#include <cstring>
#include <strings.h>
#include <node.hpp>
#include <util.hpp>
#include <tree_description_parser2.hpp>
//int yywrap();
%}
%%
[\(\):,] { return yylval.tree_marker = yytext[0];  }
\'[-a-zA-Z0-9_ \t\.{}&#;]+\' { std::string t = yytext; t = t.substr( 1, t.size() - 2 ); yylval.label = new std::wstring( CDAO::str_to_wstr(t).c_str() );  return QUOTED_STRING; }
[a-zA-Z][-a-zA-Z0-9_\.{}]* { yylval.label = new std::wstring( CDAO::str_to_wstr( yytext ).c_str() ); return IDENTIFIER; }
[+-]?[1-9][0-9]*\.?[0-9]* { yylval.weight = strtod( yytext, (char**)NULL ); return CONST; }
[\n \t] {}
%%
//int yywrap(){ return 1; }
