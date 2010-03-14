%{

#include <node.hpp>
#include <iostream>
#include <string>
#include <util.hpp>
#ifdef yywrap
   undef yywrap
   int yywrap(){ return 1; }
#endif
extern int yylex();
void yyerror( const char* );
extern CDAO::Node* pTree;
%}
%error-verbose
%glr-parser
%union {
    char tree_marker;
    char delimiter;
    char weight_marker;
    char space;
    char comment_marker;
    double weight;
    char quote;
    std::string* label;
    std::string* extra_meta;
    CDAO::Node* node;
}
%token <tree_marker> '(' ')';
%token <delimiter> ',';
%token <space> ' ';
%token <quote> '\'' '\"';
%token <weight_marker> ':';
%token <comment_marker> '[' ']';
%token <weight> CONST;
%token <label> IDENTIFIER QUOTED_STRING

%type <node> parse_tree tree tree_list label 


%%

parse_tree: tree { pTree = $1; }
    ; 
tree: '(' tree_list ')' label { $$ =  }
    | '(' tree_list ')' {}
    | '(' ',' ')' {}
    ;

tree_list: tree {}
         | label {}
         | tree ',' tree_list {}
         | label ',' tree_list {}
         ;

label: IDENTIFIER { $$ = new Node( *($1) ); }
     | QUOTED_STRING { $$ = new Node( *($1) ); }
     ;

%%

void yyerror( const char* msg){
    std::cerr << "Parse error: " << msg << std::endl; 
}

