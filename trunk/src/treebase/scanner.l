%{

#include "parser.hpp"
#include "util.hpp"
#include <string>
#include <cstdlib>
using namespace std;
%}
%%

= { return '=';}
>|&#62; { return '>';}
\[ { return '[';}
\] { return ']';}
STUDY { return STUDY_TOK;}
AUTHOR { return AUTHOR_TOK; }
HISTORY { return HISTORY_TOK; }
MATRIX { return MATRIX_TOK; }
ANALYSIS { return ANALYSIS_TOK; }
INPUT_MATRIX { return INPUT_MATRIX_TOK; }
OUTPUT_TREE { return OUTPUT_TREE_TOK; }
citation { return CITATION_TOK; }
study_id { return STUDY_ID_TOK;}
abstract { return ABSTRACT_TOK; }
author_id { return AUTHOR_ID; }
first_name { return FIRST_NAME; }
last_name { return LAST_NAME; }
email { return EMAIL; }
date { return DATE; }
time { return TIME; }
person { return PERSON; }
event { return EVENT; }
matrix_id { return MATRIX_ID; }
matrix_name { return MATRIX_NAME; }
data_type { return DATA_TYPE; }
nchar { return NCHAR; }
analysis_id { return ANALYSIS_ID; }
analysis_name { return ANALYSIS_NAME; }
software { return SOFTWARE; }
algorithm { return ALGORITHM; }
tree_id { return TREE_ID;}
tree_label { return TREE_LABEL; }
tree_title { return TREE_TITLE; }
tree_type { return TREE_TYPE;}
\'.*\' { 
        wstring ret = wstring(str_to_wstr( yytext ));
        //cerr << "quoted string: \"" << ret << "\" size: " << ret.size() <<"\n";
        ret = ret.substr(1);
        ret = ret.substr(0, ret.size()-1 );
        //cerr  << "unquoted string: \"" << ret << "\" size: " << ret.size() << "\n";
        yylval.string_literal = new wstring(ret.c_str()); 
        return QUOTED_STRING_TOK; 
}
[1-9][0-9]* { yylval.reference = atol( yytext ); return CONSTANT;}
0 {yylval.reference = 0; return CONSTANT;}
\n 
. 
%%
