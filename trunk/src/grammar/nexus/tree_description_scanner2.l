%{
#include <cstdlib>
#include <string>
#include <cstring>
#include <cstrings>
#include <node.hpp>
#include <util.hpp>
#include <tree_description_parser2.hpp>
%}
%%
[\(\):,] { return yylval.tree_marker = yytext[0];  }
\'[-a-zA-Z0-9_ \t]+\' { std::string t = yytext; t = t.substr( 1, t.size() - 2 ); yylval.label = new std::string( t.c_str() );  return QUOTED_STRING; }
[a-zA-Z][-a-zA-Z0-9_]* { yylval.label = new std::string( yytext ); return IDENTIFIER; }
[+-]?[1-9][0-9]*\.?[0-9]* { yylval.weight = strtod( yytext, (char**)NULL ); return CONST; }
\n {}
%%
